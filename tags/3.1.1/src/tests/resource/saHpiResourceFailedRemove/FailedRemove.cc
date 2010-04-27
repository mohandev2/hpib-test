/*      
 * (C) Copyright Emerson Corp. 2009
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 *  Authors:
 *  Lars Wetzel   <Lars.Wetzel@emerson.com>
 * 
 * Changes:
 *  09/10/28 <larswetzel@users.sourceforge.net>
 *           Check also if all nested resources are removed
 *           Complete re-design of test case 
 */

#include "FailedRemove.h"
#include "EventHelper.h"
#include "EventMatcher.h"
#include "ResourceHelper.h"
#include "ResourcesListEntry.h"
#include "ListIterator.h"
#include "HpiString.h"


using namespace ns_saHpiResourceFailedRemove;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

FailedRemove::FailedRemove(char *line) : HpiSessionTestCase(line) {
    origRPT = new ResourcesList();
    failedList = new IdList();
    rootList = new IdList();
    foundFailedResources = new IdList();
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *FailedRemove::getName() {
    return "FailedRemove";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *FailedRemove::getDescription() {
  return "A failed resource is removed from the RPT with function."
    "saHpiResourceFailedRemove(). Afterwards it is checked with "
    "function saHpiRptEntryGetByResourceId() if the rpt entry is "
    "really deleted and nested resources are also removed.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *FailedRemove::getPrecondition() {
    return "Requires one or more resources which are marked as failed.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT FailedRemove::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus FailedRemove::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
     
    // Fill the RPT cache
    origRPT->fill(sessionId);
    
    // Fill IdList with all failed Resources
    ListIterator iter = origRPT->iterator();
    while (iter.hasNext()) {
    	    ResourcesListEntry *rle = (ResourcesListEntry *) iter.next();
    	    SaHpiRptEntryT *rptEntry = rle->getRptEntry();
    	    if (rptEntry->ResourceFailed) {
    	    	    failedList->add(rptEntry->ResourceId);
    	    	}
	}
	
    if (failedList->getCount() == 0) {
    	    status.assertNotSupport();
    	    return status;
    	}
    	
    	findAllRootEntities();
	
	SaErrorT error = saHpiSubscribe(sessionId);
	if (error != SA_OK) {
		status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
		return status;
    } 
    
    status.add(TRACE, testFailedResources(sessionId));
    
    error = saHpiUnsubscribe(sessionId);
    status.checkError(TRACE, UNSUBSCRIBE, error);
    
    return status;
}
/**
 * Please see also HPI-B Spec 03.02, p. 31:
 * The entity path in the RPT entry of a resource that supports hot swap 
 * is the entity path for a FRU ...
 * - Nested resources for this FRU will have their own RPT entries. 
 * Each of these RPT entries either will be associated with that same FRU, ...
 * so will have the same entity path in their RPT entries, 
 * or ... will have an entity path that identifies that contained entity.
 * */
void FailedRemove::findAllRootEntities() {
    SaHpiEntityPathT rootCanditate, element;
    bool isRoot;
    
    for (int i = 0; i < failedList->getCount(); i++) {
    	    origRPT->isHotSwapFRU(failedList->getId(i), &isRoot);
    	    origRPT->getEntityPath(failedList->getId(i), &rootCanditate);
    	    if (isRoot) {
    	    	    for (int j = 0; j < failedList->getCount(); j++) {
    	    	    	    if (i != j) {
    	    	    	    	    origRPT->getEntityPath(failedList->getId(j), &element);
    	    	    	    	    if (ResourceHelper::isChildOf(&rootCanditate, &element)) {
    	    	    	    	    	    isRoot = false;
    	    	    	    	    	    break;
    	    	    	    	    	}
    	    	    	    	}
    	    	    	}
    	    	    	if (isRoot) {
    	    	    	    SaHpiUint32T rid = failedList->getId(i);
    	    	    	    rootList->add(rid);
    	    	    	}
    	    	}
    	}
}

HpiTestStatus FailedRemove::testFailedResources(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEntityPathT entityPath;
    SaHpiRptEntryT rptEntry;
    SaErrorT error;
    int i;
    
    for (i = 0; i < rootList->getCount(); i++) {
    	    pushLocation("Resource", rootList->getId(i));
    	    error = saHpiResourceFailedRemove(sessionId, rootList->getId(i));
    	    if (error != SA_OK) {
    	    	    status.assertFailure(TRACE, RESOURCE_FAILED_REMOVE, SA_OK, error);
    	    	    return status;
    	    } else {
            status.add(TRACE, pollEvents(sessionId, MAX_EVENTWAITTIME));
    	    }
    	    popLocation();	
    }
    
    // Did we get all events
    if (failedList->getCount() > foundFailedResources->getCount()) {
      for (i = 0; i < failedList->getCount(); i++) {
      	  pushLocation("Resource", failedList->getId(i));
          if (!foundFailedResources->contains(failedList->getId(i))) {
          	origRPT->getEntityPath(failedList->getId(i), &entityPath);
          	status.assertFailure(TRACE, "No removal event was generated for %s.",
          	                     HpiString::entityPath(&entityPath));
          }
          popLocation();                    
      }	
    } else if (failedList->getCount() < foundFailedResources->getCount()) {
      for (i = 0; i < foundFailedResources->getCount(); i++) {
          pushLocation("Resource", foundFailedResources->getId(i));
          if (!failedList->contains(foundFailedResources->getId(i))) {
          	origRPT->getEntityPath(foundFailedResources->getId(i), &entityPath);
          	status.assertFailure(TRACE, "Was removed but not marked as failed:\n%s.",
          	                     HpiString::entityPath(&entityPath));
          }
          popLocation();	                     
      } 
    }
    
    sleep(MAX_RPTWAITTIME);
    // Are the right resources really removed from the RPT?
    for (i = 0; i < failedList->getCount(); i++) {
    	    pushLocation("Resource", failedList->getId(i));
        if (foundFailedResources->contains(failedList->getId(i))) {
            error = saHpiRptEntryGetByResourceId(sessionId,
                                                 failedList->getId(i), &rptEntry);
            if (error == SA_ERR_HPI_INVALID_RESOURCE) {
            	    status.assertPass();
            	} else {
            		status.assertFailure(TRACE,
            	        "Valid ResourceId exists although the resource should be removed",
                    error);
            }
        } else {
        	    error = saHpiRptEntryGetByResourceId(sessionId,
                                                 failedList->getId(i), &rptEntry);
            if (error != SA_ERR_HPI_INVALID_RESOURCE) {
        	        origRPT->getEntityPath(failedList->getId(i), &entityPath);
        	        status.assertFailure(TRACE, "Was not removed from the RPT:\n%s.",
          	                     HpiString::entityPath(&entityPath));
            } else {
                origRPT->getEntityPath(failedList->getId(i), &entityPath);
        	        status.assertFailure(TRACE, "No removal event was generated for %s.",
          	                     HpiString::entityPath(&entityPath));
            }
        }
        popLocation();	 
    }		
    
    return status;	
}
/*****************************************************************************
 * Poll for events.
 * 
 * The "timeout" is in seconds.  If it is -1, then the timeout is immediate.  
 * The Poller will look for HotSwap (for FRUs) and Resource Events (for not 
 * FRUs). When one is found, the below handleEvent() method will be invoked.
 *****************************************************************************/

HpiTestStatus FailedRemove::pollEvents(SaHpiSessionIdT sessionId, long timeout) {
    HpiTestStatus status;

    EventMatcher matcher;
    matcher.setHotSwapState(SAHPI_HS_STATE_NOT_PRESENT);
    matcher.setResourceEventType(SAHPI_RESE_RESOURCE_REMOVED);
    
    status.add(TRACE, EventHelper::pollEvents(sessionId, timeout,
                                              matcher, this));

    return status;
}

/*****************************************************************************
 * If the Poller encounters a Resource or HotSwap Event, this handleEvent() 
 * method will be invoked.  
 * When invoked, we will put the resourceId into the foundFailedResource list
 * and make a short check whether the correct event Type was sent
 *****************************************************************************/

HpiTestStatus FailedRemove::handleEvent(SaHpiSessionIdT sessionId, SaHpiEventT *event) {
    HpiTestStatus status;
    bool isHotSwap;

    foundFailedResources->add(event->Source);
    SaErrorT error = origRPT->isHotSwapFRU(event->Source, &isHotSwap);
    
    if (error != SA_OK) {
        status.assertError(TRACE, "Receive Event from unknown Resource with Id %d",
                           event->Source);
    }
    if (isHotSwap && (event->EventType != SAHPI_ET_HOTSWAP)) {
    	    status.assertFailure(TRACE,	"Receive Event of type %s instead of type SAHPI_ET_HOTSWAP",
    	                                HpiString::eventType(event->EventType));
    } else if (!isHotSwap && (event->EventType != SAHPI_ET_RESOURCE)) {
        status.assertFailure(TRACE,	"Receive Event of type %s instead of type SAHPI_ET_RESOURCE",
    	                                HpiString::eventType(event->EventType));
    }
	
	// To overcome a warning of unused sessionId
	error = saHpiDiscover(sessionId); 
	if (error != SA_OK) {
        status.assertError(TRACE, SESSION_DISCOVER, SA_OK, error);  
    }
    
    return status;
}

/*****************************************************************************
 * The isEventProcessingDone() method is used to prematurely stop polling
 * for events.  
 *****************************************************************************/

bool FailedRemove::isEventProcessingDone() {
    return false;
}

