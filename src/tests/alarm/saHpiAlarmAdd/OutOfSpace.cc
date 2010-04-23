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

#include "OutOfSpace.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

OutOfSpace::OutOfSpace(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *OutOfSpace::getName() {
    return "OutOfSpace";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *OutOfSpace::getDescription() {
    return "The DAT is not able to add an additional User Alarm due to space\n"
           "limits imposed on the number of User Alarms permitted in the DAT.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *OutOfSpace::getPrecondition() {
    return "The <i>DatUserAlarmLimit</i> in the domain's info record\n"
           "must not be equal to zero.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT OutOfSpace::getExpectedReturn() {
    return SA_ERR_HPI_OUT_OF_SPACE;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus OutOfSpace::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT newAlarm;
    SaHpiDomainInfoT domainInfo;

    SaErrorT error = saHpiDomainInfoGet(sessionId, &domainInfo);
    if (error != SA_OK) {
        status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
    } else if (domainInfo.DatUserAlarmLimit == 0) { // no fixed limit of user alarms
        status.assertNotSupport();
    } else {
        bool outOfSpace = false;
        for (SaHpiUint32T i = 0; i < domainInfo.DatUserAlarmLimit + 1; i++) {
            AlarmHelper::fill(&newAlarm);
            error = saHpiAlarmAdd(sessionId, &newAlarm);
            if (error == SA_ERR_HPI_OUT_OF_SPACE) {
                status.assertPass();
                outOfSpace = true;
                break;
            } else if (error != SA_OK) {
                status.assertFailure(TRACE, ALARM_ADD, SA_OK,
                                     SA_ERR_HPI_OUT_OF_SPACE, error);
                break;
            }
        }

        if (!outOfSpace) {
            status.assertFailure(TRACE, 
                        "saHpiAlarmAdd() never returned SA_ERR_HPI_OUT_OF_SPACE.");
        }

        // Clean up
        error = saHpiAlarmDelete(sessionId, SAHPI_ENTRY_UNSPECIFIED, SAHPI_MINOR);
        status.checkError(TRACE, ALARM_DELETE, error);
    }

    return status;
}
