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

#ifndef __DISABLEDSENSOR_H__
#define __DISABLEDSENSOR_H__

#include "SafTest.h"
#include "SensorConstants.h"
#include "HpiSessionTestCase.h"
#include "EventHandler.h"
#include "UpMinorEventGenerator.h"

/*****************************************************************************
 * Disabled Sensor Test Case
 *****************************************************************************/

namespace ns_saHpiSensorEnableSet
{
    class DisabledSensor : public HpiSessionTestCase, public EventHandler
    {
    private:
        typedef struct {
            UpMinorEventGenerator *upMinorEventGenerator;
        } SensorData;

        int sensorCount;
        SensorData sensorData[MAX_THRESHOLD_SENSORS];

    public:
        DisabledSensor(char *line);

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

        HpiTestStatus restoreSensors();
    };
}

#endif
