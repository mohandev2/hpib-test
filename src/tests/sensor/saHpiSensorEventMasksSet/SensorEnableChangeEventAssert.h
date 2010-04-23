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

#ifndef __SENSORENABLECHANGEEVENTASSERT_H__
#define __SENSORENABLECHANGEEVENTASSERT_H__

#include "SafTest.h"
#include "SensorConstants.h"
#include "HpiSessionTestCase.h"
#include "EventHandler.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define NONE                 0
#define ADD_EVENT_STATES     1
#define REMOVE_EVENT_STATES  2

/*****************************************************************************
 * Sensor Enable Change Event Assert Test Case
 *****************************************************************************/

namespace ns_saHpiSensorEventMasksSet
{
    class SensorEnableChangeEventAssert : public HpiSessionTestCase, public EventHandler
    {
    private:
        typedef struct {
            SaHpiResourceIdT resourceId;
            SaHpiSensorNumT sensorNum;
            SaHpiBoolT origAssertEventMask;
            SaHpiBoolT origDeassertEventMask;
            int nextAction;
            int foundEventCount;
        } SensorData;

        int sensorCount;
        int foundEventCount;
        SensorData sensorData[MAX_THRESHOLD_SENSORS];

    public:
        SensorEnableChangeEventAssert(char *line);

        HpiTestStatus runSessionTest(SaHpiSessionIdT sessionId);

        const char *getName();

        const char *getDescription();

        const char *getPrecondition();

    private:

        HpiTestStatus testResource(SaHpiSessionIdT sessionId,
                                   SaHpiResourceIdT resourceId);
        bool canTestRdr(SaHpiRdrT *rdr);

        HpiTestStatus testSensor(SaHpiSessionIdT sessionId,
                                 SaHpiResourceIdT resourceId,
                                 SaHpiSensorRecT *sensorRec);

        HpiTestStatus pollEvents(SaHpiSessionIdT sessionId, long timeout);

        HpiTestStatus handleEvent(SaHpiSessionIdT sessionId, SaHpiEventT *event);
        bool isEventProcessingDone();

        HpiTestStatus verifyEvents();

        HpiTestStatus restoreSensors(SaHpiSessionIdT sessionId);

        HpiTestStatus changeEventMasks(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId,
                                       SaHpiSensorNumT sensorNum,
                                       SaHpiSensorEventMaskActionT action);


        HpiTestStatus resetEventMasks(SaHpiSessionIdT sessionId,
                                      SaHpiResourceIdT resourceId,
                                      SaHpiSensorNumT sensorNum,
                                      SaHpiEventStateT assertMask,
                                      SaHpiEventStateT deassertMask);
    };
}

#endif
