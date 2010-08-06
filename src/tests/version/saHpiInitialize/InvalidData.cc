/*
 * (C) Copyright Lars Wetzel, 2009
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
 *     Lars Wetzel <larswetzel@users.sourceforge.net>
 */

#include "InvalidData.h"
#include <stdlib.h>

using namespace ns_saHpiInitialize;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidData::InvalidData(char *line) : HpiLibraryTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidData::getName() {
    return "InvalidData";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidData::getDescription() {
    return "An option has an invalid OptionID or a parameter in an option" 
            " is not valid.\n"
            "FailedOption and ErrorCode should be set properly in this case";
}

extern "C" {
   SaErrorT DummyThreadFunction(void *(*StartFunction)(void *),
                                void *FunctionData) {
      (*StartFunction)(FunctionData);                                       	
      return SA_OK;
   }
}
/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidData::runLibraryTest() {
    HpiTestStatus status;
    SaHpiUint32T numInvalidData = 2;
    SaHpiInitOptionT options[numInvalidData]; 
    SaHpiUint32T failedOption;
    SaHpiCreateThreadFuncT *funcp;
    SaErrorT errorCode;

	funcp = (SaHpiCreateThreadFuncT *)malloc(sizeof(SaHpiCreateThreadFuncT));
    *funcp = *(DummyThreadFunction);
    options[0].OptionId = getInvalidInitOptionId();
    options[0].u.IntVal = 0;
    options[1].OptionId = SA_HPI_INITOPTION_HANDLE_CREATE_THREAD;
    options[1].u.PointerVal = funcp;
    
    SaErrorT error = saHpiInitialize(SAHPI_INTERFACE_VERSION, numInvalidData, 
                                     options, &failedOption, &errorCode);
    if (error == SA_ERR_HPI_INVALID_DATA) {
    	    if (failedOption == 1) {
    	    	   error = saHpiFinalize();
    	    	   if (error != SA_OK) {
    	    	      status.assertFailure(TRACE, FINALIZE, SA_OK, error);
    	    	   } else {
    	    	      options[0].OptionId = SA_HPI_INITOPTION_HANDLE_CREATE_THREAD;
              options[0].u.PointerVal = funcp;
              options[1].OptionId = getInvalidInitOptionId();
              options[1].u.IntVal = 0;
              error = saHpiInitialize(SAHPI_INTERFACE_VERSION, numInvalidData, 
                                      options, &failedOption, &errorCode);
              if (error == SA_ERR_HPI_INVALID_DATA) {
                  if (failedOption == 2) {
                      status.assertPass();
                  } else {
                      status.assertFailure(TRACE, 
    	    	              "Wrong option is marked as failed (%d instead of %d)", failedOption, 2);
                  }
              } else {
    	    	          status.assertFailure(TRACE, INITIALIZE, SA_ERR_HPI_INVALID_DATA, error);
              }
    	    	   }   
    	    } else {
           status.assertFailure(TRACE, 
    	    	   "Wrong option is marked as failed (%d instead of %d)", failedOption, 1);
    	    }
    } else {
        status.assertFailure(TRACE, INITIALIZE, SA_ERR_HPI_INVALID_DATA, error); 
    }
     
    return status;
}


                                          	

