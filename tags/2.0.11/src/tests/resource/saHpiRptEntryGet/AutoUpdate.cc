/*
 * (C) Copyright University of New Hampshire, 2006
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Author(s):
 *     Donald A. Barre <dbarre@unh.edu>
 */

#include "AutoUpdate.h"

using namespace ns_saHpiRptEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AutoUpdate::AutoUpdate(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AutoUpdate::getName() {
    return "AutoUpdate";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AutoUpdate::getDescription() {
    return "When an FRU is inserted or removed, a resource entry will be\n"
           "inserted/removed from the Resource Table and the domain info's\n"
           "<i>RptUpdateCount</i> will be incremented.  In this test, a user\n"
           "will be asked to remove and then insert an FRU.  The Resource\n"
           "Table will be checked along with the <i>RptUpdateCount</i>.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AutoUpdate::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * This is a manual test.
 *****************************************************************************/

bool AutoUpdate::isManual() {
    return true;
}

/*****************************************************************************
 * Run the test.
 *
 * Have the user remove an FRU and then ask the user to insert the FRU.
 *****************************************************************************/

HpiTestStatus AutoUpdate::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    status.add(TRACE, removeFRU(sessionId));
    if (status.isOkay()) {
        status.add(TRACE, insertFRU(sessionId));
    } 

    return status;
}

/*****************************************************************************
 * Have the user remove an FRU.  The number of entries in the RPT should
 * have decreased by one and the domain's RptUpdateCount should have been
 * incremented.  NOTE: We only not equal instead of greater than in the test
 * due to the very small chance of roll-over in the counter.
 *****************************************************************************/

HpiTestStatus AutoUpdate::removeFRU(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    int count;
    int originalCount;
    SaHpiDomainInfoT domainInfo;
    SaHpiDomainInfoT originalDomainInfo;

    status.add(TRACE, getResourceCount(sessionId, &originalCount));
    if (status.isOkay()) {
        SaErrorT error = saHpiDomainInfoGet(sessionId, &originalDomainInfo);
        if (error != SA_OK) {
            status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
        } else {
            promptRemoveFru(sessionId);

            status.add(TRACE, getResourceCount(sessionId, &count));
            if (status.isOkay()) {
                if (count != originalCount - 1) {
                    status.assertFailure(TRACE, "The number of resources in the "
                                                "RPT should have decreased by one.");
                } else {
                    error = saHpiDomainInfoGet(sessionId, &domainInfo);
                    if (error != SA_OK) {
                        status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
                    } else if (originalDomainInfo.RptUpdateCount == domainInfo.RptUpdateCount) {
                           status.assertFailure(TRACE, 
                                   "The RptUpdateCount (%u) was not updated.", 
                                   domainInfo.RptUpdateCount);
                    }
                }
            }
        }
    } 

    return status;
}

/*****************************************************************************
 * Have the user insert an FRU.  The number of entries in the RPT should
 * have increased by one and the domain's RptUpdateCount should have been
 * incremented.  NOTE: We only not equal instead of greater than in the test
 * due to the very small chance of roll-over in the counter.
 *****************************************************************************/

HpiTestStatus AutoUpdate::insertFRU(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    int count;
    int originalCount;
    SaHpiDomainInfoT domainInfo;
    SaHpiDomainInfoT originalDomainInfo;

    status.add(TRACE, getResourceCount(sessionId, &originalCount));
    if (status.isOkay()) {
        SaErrorT error = saHpiDomainInfoGet(sessionId, &originalDomainInfo);
        if (error != SA_OK) {
            status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
        } else {
            promptInsertFru(sessionId);

            status.add(TRACE, getResourceCount(sessionId, &count));
            if (status.isOkay()) {
                if (count != originalCount + 1) {
                    status.assertFailure(TRACE, "The number of resources in the RPT "
                                                "should have increased by one.");
                } else {
                    error = saHpiDomainInfoGet(sessionId, &domainInfo);
                    if (error != SA_OK) {
                        status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
                    } else if (originalDomainInfo.RptUpdateCount == domainInfo.RptUpdateCount) {
                        status.assertFailure(TRACE, "The RptUpdateCount (%u) was not updated.", 
                                             domainInfo.RptUpdateCount);
                    } else {
                        status.assertPass();
                    }
                }
            }
        }
    } 

    return status;
}

/*****************************************************************************
 * Return a count of the resources in the RPT table.
 *****************************************************************************/

HpiTestStatus AutoUpdate::getResourceCount(SaHpiSessionIdT sessionId, int *count) {
    HpiTestStatus status;
    SaHpiRptEntryT rptEntry;
    
    *count = 0;
    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (nextEntryId != SAHPI_LAST_ENTRY) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaErrorT error = saHpiRptEntryGet(sessionId, entryId, &nextEntryId, &rptEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error == SA_OK) {
            (*count)++;
        } else {
            status.assertError(TRACE, RPT_ENTRY_GET, SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
            break;
        }
    }

    return status;
}

