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

#ifndef __SENSORENABLECHANGEEVENT_H__
#define __SENSORENABLECHANGEEVENT_H__

#include "SafTest.h"
#include "SensorConstants.h"
#include "HpiSessionTestCase.h"
#include "EventHandler.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define STATE_ADD_ASSERT       1
#define STATE_REMOVE_ASSERT    2
#define STATE_ADD_DEASSERT     3
#define STATE_REMOVE_DEASSERT  4

/*****************************************************************************
 * Sensor Enable Change Event Test Case
 *
 * For independent assert and deassert event masks.
 *****************************************************************************/

namespace ns_saHpiSensorEventMasksSet
{
    class SensorEnableChangeEvent : public HpiSessionTestCase, public EventHandler
    {
    private:
        typedef struct {
            SaHpiResourceIdT resourceId;
            SaHpiSensorNumT sensorNum;
            SaHpiBoolT origAssertEventMask;
            SaHpiBoolT origDeassertEventMask;
            int state[4];
            int stateIndex;
        } SensorData;

        int sensorCount;
        int foundEventCount;
        SensorData sensorData[MAX_THRESHOLD_SENSORS];

    public:
        SensorEnableChangeEvent(char *line);

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
                                       SaHpiSensorEventMaskActionT action,
                                       SaHpiEventStateT assertEventMask,
                                       SaHpiEventStateT deassertEventMask);

        HpiTestStatus resetEventMasks(SaHpiSessionIdT sessionId,
                                      SaHpiResourceIdT resourceId,
                                      SaHpiSensorNumT sensorNum,
                                      SaHpiEventStateT assertEventMask,
                                      SaHpiEventStateT deassertEventMask);
    };
}

#endif
