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

#ifndef __UPMINOREVENTGENERATOR_H__
#define __UPMINOREVENTGENERATOR_H__

#include "SafTest.h"
#include "HpiTestStatus.h"

/*****************************************************************************
 * UpMinor Event Generator
 *
 * The UpMinor Event Generator is, as its name implies, used to generate 
 * an UpMinor event.  Given a sensor, it can determine if it is possible to
 * generate the event.  If it is possible, this generator will do all the
 * necessary work to make sure everything is setup properly to assert the
 * event.  
 *
 * Also, some HPI tests may require the sensor to be disabled or event 
 * generation to be disabled before trying to assert the UpMinor event.  
 * For those tests, it will be expected that the event will not be assserted.
 * If that kind of test is needed, use setDisableSensor() and setdDsableEvents()
 * before calling assertEvent().
 *****************************************************************************/

class UpMinorEventGenerator
{
private:
    SaHpiSessionIdT sessionId;
    SaHpiResourceIdT resourceId;
    SaHpiSensorRecT sensorRec;

    HpiTestStatus initStatus;
    SaHpiBoolT origSensorEnabled;
    SaHpiBoolT origSensorEventEnabled;
    SaHpiEventStateT origAssertMask;
    SaHpiEventStateT origDeassertMask;
    SaHpiSensorThresholdsT origSensorThresholds;

    bool changedEnabled;
    bool changedEventsEnabled;
    bool changedMask;
    bool changedThresholds;

    bool disableSensor;
    bool disableEvents;
    bool removeAssertEventMask;

public:
    UpMinorEventGenerator(SaHpiSessionIdT sessionId,
                          SaHpiResourceIdT resourceId,
                          SaHpiSensorRecT *sensorRec);

    void setDisableSensor();
    void setDisableEvents();
    void removeAssertEventMaskBit();

    HpiTestStatus canAssertEvent(bool *canAssert);
    HpiTestStatus assertEvent();
    HpiTestStatus deassertEvent();

    HpiTestStatus restoreSensor();

    SaHpiResourceIdT getResourceId();
    SaHpiSensorNumT getSensorNum();
    bool matches(SaHpiResourceIdT resourceId, SaHpiSensorNumT sensorNum);

private:
    void checkSensor();
    void readSensorInfo();

    HpiTestStatus enableSensor();
    HpiTestStatus enableEvents();

    HpiTestStatus setUpMinorEventMask();
    HpiTestStatus getThresholds();

    HpiTestStatus isReadingWithinMinorLimits(bool *isWithin);
};

#endif

