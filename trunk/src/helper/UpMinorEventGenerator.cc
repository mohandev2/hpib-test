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

#include "UpMinorEventGenerator.h"
#include "SensorHelper.h"

/*****************************************************************************
 * Constructor
 *
 * Initially, we will make sure that the sensor and event generation is
 * enabled (disable is false) when it is time to generate the event.
 *****************************************************************************/

UpMinorEventGenerator::UpMinorEventGenerator(SaHpiSessionIdT session_id, 
                                             SaHpiResourceIdT resource_id, 
                                             SaHpiSensorRecT *sensor_rec) {
    this->sessionId = session_id;
    this->resourceId = resource_id;
    this->sensorRec = *sensor_rec;

    disableSensor = false;
    disableEvents = false;
    removeAssertEventMask = false;

    changedEnabled = false;
    changedEventsEnabled = false;
    changedMask = false;
    changedThresholds = false;

    checkSensor();
    if (initStatus.isOkay()) {
        readSensorInfo();
    }
}

/*****************************************************************************
 * Check the sensor to be sure it meets some minimum requirements for 
 * generating Up Minor Events.
 *
 *      1) This must be a threshold sensor.
 *      2) We must be able to read the LowMinor threshold.
 *      3) We must be able to read and write the UpMinor threshold.
 *      4) The sensor must be able to generate UpMinor events.
 *****************************************************************************/

void UpMinorEventGenerator::checkSensor() {
    if (!((sensorRec.ThresholdDefn.IsAccessible) &&
          (sensorRec.ThresholdDefn.ReadThold & SAHPI_STM_LOW_MINOR) &&
          (sensorRec.ThresholdDefn.ReadThold & SAHPI_STM_UP_MINOR) &&
          (sensorRec.ThresholdDefn.WriteThold & SAHPI_STM_UP_MINOR) &&
          (sensorRec.Events & SAHPI_ES_UPPER_MINOR))) {

        initStatus.assertNotSupport();
    }
}

/*****************************************************************************
 * Before we do anything, we must read the current state of the sensor so 
 * that we can restore the values when we are finished.  If any error occurs
 * while reading the current state, canAssert() will return initStatus and
 * no restoration will occur.
 *****************************************************************************/

void UpMinorEventGenerator::readSensorInfo() {
    
    // Get the Enabled state of the sensor

    SaErrorT error = saHpiSensorEnableGet(sessionId, resourceId,
                                          sensorRec.Num, &origSensorEnabled);
    if (error == SA_ERR_HPI_BUSY) {
        initStatus.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        initStatus.assertNotSupport();
    } else if (error != SA_OK) {
        initStatus.assertError(TRACE, SENSOR_ENABLE_GET, SA_OK, error);
    } 

    // Get the Event Enabled state of the sensor

    error = saHpiSensorEventEnableGet(sessionId, resourceId,
                                      sensorRec.Num, &origSensorEventEnabled);
    if (error == SA_ERR_HPI_BUSY) {
        initStatus.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        initStatus.assertNotSupport();
    } else if (error != SA_OK) {
        initStatus.assertError(TRACE, SENSOR_EVENT_ENABLE_GET, SA_OK, error);
    } 

    // Get the event masks.

    error = saHpiSensorEventMasksGet(sessionId, resourceId, sensorRec.Num, 
                                     &origAssertMask, &origDeassertMask);
    if (error == SA_ERR_HPI_BUSY) {
        initStatus.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        initStatus.assertNotSupport();
    } else if (error != SA_OK) {
        initStatus.assertError(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
    } 

    // Get the original Thresholds

    error = saHpiSensorThresholdsGet(sessionId, resourceId, 
                                     sensorRec.Num, &origSensorThresholds);
    if (error != SA_OK) {
        initStatus.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
    }
}

/*****************************************************************************
 * Force the sensor to be disabled when asserting the event.  This is useful
 * for verifying that the event doesn't get generated.
 *****************************************************************************/

void UpMinorEventGenerator::setDisableSensor() {
    disableSensor = true;
}

/*****************************************************************************
 * Force event generation to be disabled when asserting the event.
 * This is useful for verifying that the event doesn't get generated.
 *****************************************************************************/

void UpMinorEventGenerator::setDisableEvents() {
    disableEvents = true;
}

/*****************************************************************************
 * Force the sensor to be turn off the assertion for the UpMinor event.
 * This is useful for verifying that the event doesn't get generated.
 *****************************************************************************/

void UpMinorEventGenerator::removeAssertEventMaskBit() {
    removeAssertEventMask = true;
}

/*****************************************************************************
 * Return the resource id.
 *****************************************************************************/

SaHpiResourceIdT UpMinorEventGenerator::getResourceId() {
    return resourceId;
}

/*****************************************************************************
 * Return the sensor num.
 *****************************************************************************/

SaHpiSensorNumT UpMinorEventGenerator::getSensorNum() {
    return sensorRec.Num;
}

/*****************************************************************************
 * Does this UpMinorEventGenerator match the given resource and sensor num?
 *****************************************************************************/

bool UpMinorEventGenerator::matches(SaHpiResourceIdT resource_id, SaHpiSensorNumT num) {
    return (this->resourceId == resource_id) && (sensorRec.Num == num);
}

/*****************************************************************************
 * Can we assert the event?  In other words, in order to assert the UpMinor
 * event, the following conditions must be met:
 *
 *      1) The sensor must be enabled. (The sensor must be enabled
 *         in order to read the current reading.)
 *      2) Event Generation must be enabled.
 *      3) The Assert/Deassert Event Mask bit for UpMinor must be set.
 *      4) The current reading must be within the LowMinor and UpMinor
 *         thresholds.  It doesn't help if the current value has already
 *         caused an assert event to be generated.
 *****************************************************************************/

HpiTestStatus UpMinorEventGenerator::canAssertEvent(bool *canAssert) {
    HpiTestStatus status;
    bool isWithin;

    *canAssert = false;

    if (!initStatus.isOkay()) {
        return initStatus;
    } else {
        status.add(TRACE, enableSensor());
        if (status.isOkay()) {
            status.add(TRACE, enableEvents());
            if (status.isOkay()) {
                status.add(TRACE, setUpMinorEventMask());
                if (status.isOkay()) {
                    status.add(TRACE, isReadingWithinMinorLimits(&isWithin));
                    if (status.isOkay() && isWithin) {
                        *canAssert = true;
                    }
                }
            }
        }
     }

     return status;
}

/*****************************************************************************
 * Enable the sensor if it isn't already enabled.
 *****************************************************************************/

HpiTestStatus UpMinorEventGenerator::enableSensor() {
    HpiTestStatus status;

    if (!origSensorEnabled) {
        if (!sensorRec.EnableCtrl) {
            status.assertNotSupport();
        } else {
            SaErrorT error = saHpiSensorEnableSet(sessionId, resourceId,
                                                  sensorRec.Num, SAHPI_TRUE);
            if (error != SA_OK) {
                status.assertError(TRACE, SENSOR_ENABLE_SET, SA_OK, error);
            } else {
                changedEnabled = true;
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Enable event generation if it isn't already enabled.
 *****************************************************************************/

HpiTestStatus UpMinorEventGenerator::enableEvents() {
    HpiTestStatus status;

    if (!origSensorEventEnabled) {
        if (sensorRec.EventCtrl == SAHPI_SEC_READ_ONLY) {
            status.assertNotSupport();
        } else {
            SaErrorT error = saHpiSensorEventEnableSet(sessionId, resourceId,
                                                       sensorRec.Num, SAHPI_TRUE);
            if (error != SA_OK) {
                status.assertError(TRACE, SENSOR_EVENT_ENABLE_SET, SA_OK, error);
            } else {
                changedEventsEnabled = true;
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Set up the Assert/Deasset Event Mask for UpMinor if necessary.
 *****************************************************************************/

HpiTestStatus UpMinorEventGenerator::setUpMinorEventMask() {
    HpiTestStatus status;

    if ((origAssertMask & SAHPI_ES_UPPER_MINOR) &&
        (origDeassertMask & SAHPI_ES_UPPER_MINOR)) {
        // do nothing; we can generate events
    } else if (sensorRec.EventCtrl == SAHPI_SEC_PER_EVENT) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorRec.Num, 
                                                  SAHPI_SENS_ADD_EVENTS_TO_MASKS,
                                                  SAHPI_ES_UPPER_MINOR, SAHPI_ES_UPPER_MINOR);
        if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
        } else {
            changedMask = true;
        }
    } 

    return status;
}

/*****************************************************************************
 * Is the current reading within the LowMinor and UpMinor thresholds?
 * Hysteresis must also be factored in so that we have a current reading
 * that hasn't caused an assert event.
 *****************************************************************************/

HpiTestStatus UpMinorEventGenerator::isReadingWithinMinorLimits(bool *isWithin) {

    HpiTestStatus status;
    SaHpiSensorReadingT reading;
    SaHpiSensorReadingT low, high;

    *isWithin = false;
    SaErrorT error = saHpiSensorReadingGet(sessionId, resourceId, sensorRec.Num, &reading, NULL);
    if (error == SA_ERR_HPI_BUSY) {
        *isWithin = false;
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        *isWithin = false;
    } else if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_READING_GET, SA_OK, error);
    } else {
        switch (reading.Type) {
        case SAHPI_SENSOR_READING_TYPE_INT64:
            low.Value.SensorInt64 = origSensorThresholds.LowMinor.Value.SensorInt64;
            if (origSensorThresholds.NegThdHysteresis.IsSupported) {
                low.Value.SensorInt64 += origSensorThresholds.NegThdHysteresis.Value.SensorInt64;
            }
            high.Value.SensorInt64 = origSensorThresholds.UpMinor.Value.SensorInt64;
            if (origSensorThresholds.PosThdHysteresis.IsSupported) {
                high.Value.SensorInt64 -= origSensorThresholds.PosThdHysteresis.Value.SensorInt64;
            }

            *isWithin = (reading.Value.SensorInt64 > low.Value.SensorInt64) && 
                        (reading.Value.SensorInt64 < high.Value.SensorInt64);
            break;

        case SAHPI_SENSOR_READING_TYPE_UINT64:
            low.Value.SensorUint64 = origSensorThresholds.LowMinor.Value.SensorUint64;
            if (origSensorThresholds.NegThdHysteresis.IsSupported) {
                low.Value.SensorUint64 += origSensorThresholds.NegThdHysteresis.Value.SensorUint64;
            }
            high.Value.SensorUint64 = origSensorThresholds.UpMinor.Value.SensorUint64;
            if (origSensorThresholds.PosThdHysteresis.IsSupported) {
                high.Value.SensorUint64 -= origSensorThresholds.PosThdHysteresis.Value.SensorUint64;
            }

            *isWithin = (reading.Value.SensorUint64 > low.Value.SensorUint64) &&
                           (reading.Value.SensorUint64 < high.Value.SensorUint64);
            break;

        case SAHPI_SENSOR_READING_TYPE_FLOAT64:
            low.Value.SensorFloat64 = origSensorThresholds.LowMinor.Value.SensorFloat64;
            if (origSensorThresholds.NegThdHysteresis.IsSupported) {
                low.Value.SensorFloat64 += origSensorThresholds.NegThdHysteresis.Value.SensorFloat64;
            }

            high.Value.SensorFloat64 = origSensorThresholds.UpMinor.Value.SensorFloat64;
            if (origSensorThresholds.PosThdHysteresis.IsSupported) {
                high.Value.SensorFloat64 -= origSensorThresholds.PosThdHysteresis.Value.SensorFloat64;
            }

            *isWithin = (reading.Value.SensorFloat64 > low.Value.SensorFloat64) &&
                        (reading.Value.SensorFloat64 < high.Value.SensorFloat64);
            break;

        case SAHPI_SENSOR_READING_TYPE_BUFFER:
            break;
        }
    }

    return status;
}

/*****************************************************************************
 * Assert the UpMinor event.  This is done by changing the UpMinor threshold
 * to be the same as the LowMinor threshold.  
 *
 * For some tests, it is necessary to first disable the sensor and/or 
 * the event generation.  
 *****************************************************************************/

HpiTestStatus UpMinorEventGenerator::assertEvent() {
    HpiTestStatus status;
    SaHpiSensorThresholdsT newSensorThresholds;

    if (disableSensor) {
        SaErrorT error = saHpiSensorEnableSet(sessionId, resourceId,
                                              sensorRec.Num, SAHPI_FALSE);
        status.checkError(TRACE, SENSOR_ENABLE_SET, error);
        changedEnabled = true;
    }

    if (disableEvents) {
        SaErrorT error = saHpiSensorEventEnableSet(sessionId, resourceId,
                                                   sensorRec.Num, SAHPI_FALSE);
        status.checkError(TRACE, SENSOR_EVENT_ENABLE_SET, error);
        changedEventsEnabled = true;
    }

    if (removeAssertEventMask) {
        SaErrorT error = saHpiSensorEventMasksSet(sessionId, resourceId,
                                sensorRec.Num, SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS,
                                SAHPI_ES_UPPER_MINOR, 0x0);
        status.checkError(TRACE, SENSOR_EVENT_MASKS_SET, error);
        changedMask = true;
    }

    if (status.isOkay()) {
        SensorHelper::fill(&newSensorThresholds, sensorRec.DataFormat.ReadingType);
        newSensorThresholds.UpMinor.IsSupported = SAHPI_TRUE;
        SensorHelper::setThreshold(&newSensorThresholds,
                                   SAHPI_STM_UP_MINOR, origSensorThresholds.LowMinor);

        SaErrorT error = saHpiSensorThresholdsSet(sessionId, resourceId,
                                                  sensorRec.Num, &newSensorThresholds);
        if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_THRESHOLDS_SET, SA_OK, error);
        } else {
            changedThresholds = true;
        }
    }

    return status;
}

/*****************************************************************************
 * Deassert the UpMinor event.  This is done by changing the UpMinor
 * threshold back to its original value.
 *****************************************************************************/

HpiTestStatus UpMinorEventGenerator::deassertEvent() {
    HpiTestStatus status;
    SaHpiSensorThresholdsT newSensorThresholds;

    if (changedThresholds) {
        SensorHelper::fill(&newSensorThresholds, sensorRec.DataFormat.ReadingType);
        newSensorThresholds.UpMinor.IsSupported = SAHPI_TRUE;
        SensorHelper::setThreshold(&newSensorThresholds,
                                   SAHPI_STM_UP_MINOR, origSensorThresholds.UpMinor);
/*Original Call
** This call can fail if the reading mask allows more values as the writing mask(in origSensorThreshold the IsSupport flags from the reading are still set)
  
SaErrorT error = saHpiSensorThresholdsSet(sessionId, resourceId,
                                                  sensorRec.Num, &origSensorThresholds);
*/
SaErrorT error = saHpiSensorThresholdsSet(sessionId, resourceId,
                                                  sensorRec.Num, &newSensorThresholds);
        status.checkError(TRACE, SENSOR_THRESHOLDS_SET, error);
        changedThresholds = false;
    }

    return status;
}

/*****************************************************************************
 * Restore the sensor to it's original settings.
 *****************************************************************************/

HpiTestStatus UpMinorEventGenerator::restoreSensor() {
    HpiTestStatus status;
    SaErrorT error;

    if (initStatus.isOkay()) {

        // In case the application didn't deassert the Event, we must
        // do it to be sure that the thresholds are restored.

        deassertEvent(); 

        if (changedMask) {
            error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorRec.Num, 
                                             SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS ,
                                             SAHPI_ALL_EVENT_STATES, SAHPI_ALL_EVENT_STATES);
            if (error != SA_OK) {
                status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
            } else {
                error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorRec.Num, 
                                                 SAHPI_SENS_ADD_EVENTS_TO_MASKS,
                                                 origAssertMask, origDeassertMask);
                status.checkError(TRACE, SENSOR_EVENT_MASKS_SET, error);
            }
        } 

        if (changedEventsEnabled) {
            error = saHpiSensorEventEnableSet(sessionId, resourceId,
                                              sensorRec.Num, origSensorEventEnabled);
            status.checkError(TRACE, SENSOR_EVENT_ENABLE_SET, error);
        } 

        if (changedEnabled) {
            error = saHpiSensorEnableSet(sessionId, resourceId,
                                         sensorRec.Num, origSensorEnabled);
            status.checkError(TRACE, SENSOR_ENABLE_SET, error);
        } 
    }

    return status;
}
