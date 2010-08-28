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
 *     Meghana Srivatsavai <mjn77@unh.edu>
 * 
 * Changes:
 * 09/12/28 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Integrate the test case into hpitest
 *          Add method VerifyDisable
 *          Test transisitions "value is not changed" , "value is changed"
 */

#include "SetDisable.h"

using namespace ns_saHpiFumiAutoRollbackDisableSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetDisable::SetDisable(char *line) : FumiTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetDisable::getName() {
    return "SetDisable";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetDisable::getDescription() {
    return "Used to identify the specification defined framework and the\n"
           "the underlying FUMi implementation.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetDisable::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetDisable::runFumiTest(SaHpiSessionIdT sessionId, SaHpiRptEntryT *rptEntry,
                                      SaHpiFumiRecT *fumiRec) {    

    HpiTestStatus status;
    SaHpiBoolT origValue;
    
    // First check whether a test is possible
    if (!(fumiRec->Capability & SAHPI_FUMI_CAP_AUTOROLLBACK) || 
        !(fumiRec->Capability & SAHPI_FUMI_CAP_AUTOROLLBACK_CAN_BE_DISABLED)) {

        	status.assertNotSupport();
        	return status;
    }
        	
    SaErrorT error = saHpiFumiAutoRollbackDisableGet(sessionId,
                                                     rptEntry->ResourceId,
                                                     fumiRec->Num,
                                                     &origValue);
    if (error != SA_OK) {                                                  
       status.assertFailure(TRACE, FUMI_AUTO_ROLLBACK_DISABLE_GET, SA_OK, error);
    
    } else {                                                 
       error = saHpiFumiAutoRollbackDisableSet(sessionId, rptEntry->ResourceId,
                                               fumiRec->Num, origValue);
	   if (error != SA_OK) {
          status.assertFailure(TRACE, FUMI_AUTO_ROLLBACK_DISABLE_SET, SA_OK, error);
	   } else {
	   	  status.add(TRACE, verifyDisableValue(sessionId, rptEntry, fumiRec, origValue));
	   }
	   
	   error = saHpiFumiAutoRollbackDisableSet(sessionId, rptEntry->ResourceId,
                                               fumiRec->Num, !origValue);
	   if (error != SA_OK) {
          status.assertFailure(TRACE, FUMI_AUTO_ROLLBACK_DISABLE_SET, SA_OK, error);
	   } else {
	   	  status.add(TRACE, verifyDisableValue(sessionId, rptEntry, fumiRec, !origValue));
	   }
	   
	   error = saHpiFumiAutoRollbackDisableSet(sessionId, rptEntry->ResourceId,
                                               fumiRec->Num, origValue);
	   if (error != SA_OK) {
          status.assertFailure(TRACE, FUMI_AUTO_ROLLBACK_DISABLE_SET, SA_OK, error);
	   } else {
	   	  status.add(TRACE, verifyDisableValue(sessionId, rptEntry, fumiRec, origValue));
	   }                                          
    }
    
    return status;
}

HpiTestStatus SetDisable::verifyDisableValue(SaHpiSessionIdT sessionId, SaHpiRptEntryT *rptEntry,
                                        SaHpiFumiRecT *fumiRec, SaHpiBoolT expValue) {
    SaHpiBoolT resValue;
    HpiTestStatus status;
	
    SaErrorT error = saHpiFumiAutoRollbackDisableGet(sessionId, rptEntry->ResourceId,
                                                     fumiRec->Num, &resValue);
    if (error != SA_OK) {
        status.assertFailure(TRACE, FUMI_AUTO_ROLLBACK_DISABLE_GET, SA_OK, error);	

    } else if (!isBoolEqual(expValue, resValue)) { 
    	    status.assertFailure(TRACE, "Wrong value for RollbackDisable: expected %s - get %s\n",
    	                         HpiString::boolean(expValue), HpiString::boolean(resValue));
    } else {
        status.assertPass();	
    }
    
    return status;
}

