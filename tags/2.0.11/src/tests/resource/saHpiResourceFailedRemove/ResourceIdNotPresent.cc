/*      
 * (C) Copyright IBM Corp. 2008
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
 *  Suntrupth S Yadav <suntrupth@in.ibm.com>
 *  Lars Wetzel <Lars.Wetzel@emerson.com>
 *
 */

#include "ResourceIdNotPresent.h"

using namespace ns_saHpiResourceFailedRemove;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ResourceIdNotPresent::ResourceIdNotPresent(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ResourceIdNotPresent::getName() {
    return "ResourceIdNotPresent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ResourceIdNotPresent::getDescription() {
    return "The Resource identified by <i>ResourceId</i> is not present.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ResourceIdNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ResourceIdNotPresent::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    
    // How should the error codes HPI_NOT_PRESENT and INVALID_RESOURCE be distinguish?
    // see also Mantis 223

    SaErrorT error = saHpiResourceFailedRemove(sessionId, getInvalidResourceId());
                                      
    if (error == SA_ERR_HPI_NOT_PRESENT) {
      status.assertPass();
      
    } else if (error == SA_ERR_HPI_INVALID_RESOURCE) {
      status.assertPass();

    } else {
      status.assertFailure(TRACE, RESOURCE_FAILED_REMOVE, SA_ERR_HPI_NOT_PRESENT, error);
    } 

    error = saHpiResourceFailedRemove(sessionId,  SAHPI_UNSPECIFIED_RESOURCE_ID);
                                      
    if (error == SA_ERR_HPI_NOT_PRESENT) {
      status.assertPass();
      
    } else if (error == SA_ERR_HPI_INVALID_RESOURCE) {
      status.assertPass();

    } else {
      status.assertFailure(TRACE, RESOURCE_FAILED_REMOVE, SA_ERR_HPI_NOT_PRESENT, error);
    } 

    return status;
}

