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

#include "InvalidSeverity.h"

using namespace ns_saHpiResourceSeveritySet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidSeverity::InvalidSeverity(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidSeverity::getName() {
    return "InvalidSeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidSeverity::getDescription() {
    return "The <i>Severity</i> parameter is not one of the valid enumerated "
           "values for this type.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidSeverity::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidSeverity::runResourceTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    int invalidSeverity[] = { (int) SAHPI_OK + 1, 
                              (int) SAHPI_DEBUG - 1, 
                              (int) SAHPI_DEBUG + 1, 
                              (int) SAHPI_ALL_SEVERITIES - 1, 
                              (int) SAHPI_ALL_SEVERITIES, 
                              (int) SAHPI_ALL_SEVERITIES - 1 };

    // test all invalid severities
      
    for (int i = 0; i < 6; i++) {
        SaErrorT error = saHpiResourceSeveritySet(sessionId,
                                                  rptEntry->ResourceId,
                                                  (SaHpiSeverityT) invalidSeverity[i]);
  
if (error == SA_ERR_HPI_INVALID_PARAMS ) 
     {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, RESOURCE_SEVERITY_SET,
                                 SA_ERR_HPI_INVALID_PARAMS, error,
                                 "The invalid severity is %s.",
                                 HpiString::severity((SaHpiSeverityT) invalidSeverity[i]));
        }
    }
      
    // Restore the original severity

    SaErrorT error = saHpiResourceSeveritySet(sessionId,
                                              rptEntry->ResourceId,
                                              rptEntry->ResourceSeverity);
    status.checkError(TRACE, RESOURCE_SEVERITY_SET, error,
                      "The severity is %s.",
                      HpiString::severity(rptEntry->ResourceSeverity));

    return status;
}
