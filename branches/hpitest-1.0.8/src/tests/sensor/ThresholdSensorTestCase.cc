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

#include "ThresholdSensorTestCase.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ThresholdSensorTestCase::ThresholdSensorTestCase()
: SensorTestCase() {
    this->onlyReadable = true;
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ThresholdSensorTestCase::ThresholdSensorTestCase(char *line)
: SensorTestCase(line) {
    this->onlyReadable = true;
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ThresholdSensorTestCase::ThresholdSensorTestCase(char *line, bool onlyRead)
: SensorTestCase(line) {
    this->onlyReadable = onlyRead;
}

/*****************************************************************************
 * This must be a threshold sensor in order to perform the test.  Optionally,
 * it can be required that there must be one or more readable thresholds.
 *****************************************************************************/

HpiTestStatus ThresholdSensorTestCase::runSensorTest(SaHpiSessionIdT sessionId,
                                                     SaHpiRptEntryT *rptEntry,
                                                     SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;

    if (!sensorRec->ThresholdDefn.IsAccessible) {
        status.assertNotSupport();
    } else if (onlyReadable && sensorRec->ThresholdDefn.ReadThold == 0) {
        status.assertNotSupport();
    } else {
        status = runThresholdTest(sessionId, rptEntry, sensorRec);
    }

    return status;
}
