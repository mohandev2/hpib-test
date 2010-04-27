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

#ifndef __SENSORTESTCASE_H___
#define __SENSORTESTCASE_H___

#include "SafTest.h"
#include "HpiRdrTestCase.h"

/*****************************************************************************
 * Sensor Test Case
 *****************************************************************************/

class SensorTestCase : public HpiRdrTestCase
{
private:
    SaHpiEventStateT savedAssertEventMask;
    SaHpiEventStateT savedDeassertEventMask;

public:
    SensorTestCase();
    SensorTestCase(char* line);

protected:
    HpiTestStatus runRdrTest(SaHpiSessionIdT sessionId,
                             SaHpiRptEntryT *rptEntry, 
                             SaHpiRdrT *rdr);

    virtual HpiTestStatus runSensorTest(SaHpiSessionIdT sessionId, 
                                        SaHpiRptEntryT *rptEntry, 
                                        SaHpiSensorRecT *sensorRec) = 0;

    HpiTestStatus saveEventMasks(SaHpiSessionIdT sessionId,
                                 SaHpiResourceIdT resourceId,
                                 SaHpiSensorNumT sensorNum);

    HpiTestStatus restoreEventMasks(SaHpiSessionIdT sessionId,
                                    SaHpiResourceIdT resourceId,
                                    SaHpiSensorNumT sensorNum);
};

#endif

