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
 *
 * Changes
 * 2009/05/19 - Lars.Wetzel@emerson.com
 *              Allow Sensor->Types > OEM
 * 2010/03/12 - <larswetzel@users.sourceforge.net>
 *              new function setMask for setting reading or writing mask properly
 */

#include "SensorHelper.h"
#include "Report.h"
#include "HpiString.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MIN_INT64 ((SaHpiInt64T) 0xffffffff)
#define MAX_INT64 ((SaHpiInt64T) 0x7fffffff)

#define MIN_UINT64 ((SaHpiUint64T) 0x0)
#define MAX_UINT64 ((SaHpiUint64T) 0xffffffff)

#define MIN_FLOAT64 ((SaHpiFloat64T) 1e-37)
#define MAX_FLOAT64 ((SaHpiFloat64T) 1e+37)

#define SMALL_FLOAT 0.01

/*****************************************************************************
 * Is the Sensor Record valid or not?
 *****************************************************************************/

bool SensorHelper::isValid(SaHpiSensorRecT *rec, Report &report) {
    bool valid = true;

    if (!isValidType(rec->Type)) {
        valid = false;
        report.add("SensorRec->Type is invalid [0x%X].", rec->Type);
    }

    if (!isValidEventCategory(rec->Category)) {
        valid = false;
        report.add("SensorRec->Category is invalid [0x%X].", rec->Category);
    }

    if (!isValidEventCtrl(rec->EventCtrl)) {
        valid = false;
        report.add("SensorRec->EventCtrl is invalid [0x%X].", rec->EventCtrl);
    }

    if (!isValidDataFormat(&rec->DataFormat, report)) {
        valid = false;
    }

    if (!isValidThresholdDefn(&rec->ThresholdDefn, report)) {
        valid = false;
    }

    return valid;
}

/*****************************************************************************
 * Are the two Sensor Records the same or not?
 *****************************************************************************/

bool SensorHelper::isEqual(SaHpiSensorRecT *rec1, SaHpiSensorRecT *rec2) {
    Report report;
    return isEqual(rec1, rec2, report);
}

/*****************************************************************************
 * Are the two Sensor Records the same or not? Include a report of the
 * differences.
 *****************************************************************************/

bool SensorHelper::isEqual(SaHpiSensorRecT *rec1,
                           SaHpiSensorRecT *rec2, Report &report) {
    bool isEqual = true;

    if (rec1->Num != rec2->Num) {
        isEqual = false;
        report.add("Sensor Num fields are not the same [0x%X, 0x%X].",
                   rec1->Num, rec2->Num);
    }

    return isEqual;
}

/*****************************************************************************
 * Fill the sensor thresholds with the IsSupported field set to FALSE for
 * all thresholds.
 *****************************************************************************/

void SensorHelper::fill(SaHpiSensorThresholdsT *sensorThresholds) {
    sensorThresholds->LowCritical.IsSupported = SAHPI_FALSE;
    sensorThresholds->LowMajor.IsSupported = SAHPI_FALSE;
    sensorThresholds->LowMinor.IsSupported = SAHPI_FALSE;
    sensorThresholds->UpCritical.IsSupported = SAHPI_FALSE;
    sensorThresholds->UpMajor.IsSupported = SAHPI_FALSE;
    sensorThresholds->UpMinor.IsSupported = SAHPI_FALSE;
    sensorThresholds->PosThdHysteresis.IsSupported = SAHPI_FALSE;
    sensorThresholds->NegThdHysteresis.IsSupported = SAHPI_FALSE;
}

/*****************************************************************************
 * Fill the sensor thresholds with the IsSupported field set to FALSE for
 * all thresholds and set the reading type.
 *****************************************************************************/

void SensorHelper::fill(SaHpiSensorThresholdsT *sensorThresholds,
                        SaHpiSensorReadingTypeT readingType) {
    fill(sensorThresholds);
    sensorThresholds->LowCritical.Type = readingType;
    sensorThresholds->LowMajor.Type = readingType;
    sensorThresholds->LowMinor.Type = readingType;
    sensorThresholds->UpCritical.Type = readingType;
    sensorThresholds->UpMajor.Type = readingType;
    sensorThresholds->UpMinor.Type = readingType;
    sensorThresholds->PosThdHysteresis.Type = readingType;
    sensorThresholds->NegThdHysteresis.Type = readingType;
}

/*****************************************************************************
 * Copy the thresholds specified by the "thdMask" from a set of thresholds
 * to another set of thresholds.
 *****************************************************************************/

void SensorHelper::copy(SaHpiSensorThresholdsT *toSensorThresholds,
                        SaHpiSensorThresholdsT *fromSensorThresholds, 
                        SaHpiSensorThdMaskT thdMask) {

    if (thdMask & SAHPI_STM_LOW_MINOR) {
        toSensorThresholds->LowMinor = fromSensorThresholds->LowMinor;
    }

    if (thdMask & SAHPI_STM_LOW_MAJOR) {
        toSensorThresholds->LowMajor = fromSensorThresholds->LowMajor;
    }

    if (thdMask & SAHPI_STM_LOW_CRIT) {
        toSensorThresholds->LowCritical = fromSensorThresholds->LowCritical;
    }

    if (thdMask & SAHPI_STM_UP_MINOR) {
        toSensorThresholds->UpMinor = fromSensorThresholds->UpMinor;
    }

    if (thdMask & SAHPI_STM_UP_MAJOR) {
        toSensorThresholds->UpMajor = fromSensorThresholds->UpMajor;
    }

    if (thdMask & SAHPI_STM_UP_CRIT) {
        toSensorThresholds->UpCritical = fromSensorThresholds->UpCritical;
    }

    if (thdMask & SAHPI_STM_UP_HYSTERESIS) { 
        toSensorThresholds->PosThdHysteresis = fromSensorThresholds->PosThdHysteresis;
    }

    if (thdMask & SAHPI_STM_LOW_HYSTERESIS) {
        toSensorThresholds->NegThdHysteresis = fromSensorThresholds->NegThdHysteresis;
    }
}

/*****************************************************************************
 * Is the threshold, specified by "mask", a write threshold?
 *****************************************************************************/

bool SensorHelper::isWriteThreshold(SaHpiSensorRecT * sensorRec,
                                    SaHpiSensorThdMaskT mask) {
    return sensorRec->ThresholdDefn.WriteThold & mask;
}

/*****************************************************************************
 * Is the threshold, specified by "mask", a read threshold?
 *****************************************************************************/

bool SensorHelper::isReadThreshold(SaHpiSensorRecT * sensorRec,
                                   SaHpiSensorThdMaskT mask) {
    return sensorRec->ThresholdDefn.ReadThold & mask;
}

/*****************************************************************************
 * Is the threshold, specified by "mask", a read/write threshold?
 *****************************************************************************/

bool SensorHelper::isReadWriteThreshold(SaHpiSensorRecT * sensorRec,
                                        SaHpiSensorThdMaskT mask) {
    return (sensorRec->ThresholdDefn.WriteThold & mask) &&
           (sensorRec->ThresholdDefn.ReadThold & mask);
}

/*****************************************************************************
 * Does the sensor have at least one read/write threshold?
 *****************************************************************************/

bool SensorHelper::hasReadWriteThresholds(SaHpiSensorRecT * sensorRec) {
    return (sensorRec->ThresholdDefn.ReadThold && sensorRec->ThresholdDefn.WriteThold);
}

/*****************************************************************************
 * Does the sensor have full support for reading and writing all of the
 * six thresholds?
 *****************************************************************************/

bool SensorHelper::isFullReadWrite(SaHpiSensorRecT * sensorRec) {
    return (isReadWriteThreshold(sensorRec, SAHPI_STM_LOW_MINOR) &&
            isReadWriteThreshold(sensorRec, SAHPI_STM_LOW_MAJOR) &&
            isReadWriteThreshold(sensorRec, SAHPI_STM_LOW_CRIT) &&
            isReadWriteThreshold(sensorRec, SAHPI_STM_UP_MINOR) &&
            isReadWriteThreshold(sensorRec, SAHPI_STM_UP_MAJOR) &&
            isReadWriteThreshold(sensorRec, SAHPI_STM_UP_CRIT));
}

/*****************************************************************************
 * Is there a min value for the sensor threshold?
 *****************************************************************************/

bool SensorHelper::hasMin(SaHpiSensorRecT * sensorRec) {
    return sensorRec->DataFormat.Range.Flags & SAHPI_SRF_MIN;
}

/*****************************************************************************
 * Is there a max value for the sensor threshold?
 *****************************************************************************/

bool SensorHelper::hasMax(SaHpiSensorRecT * sensorRec) {
    return sensorRec->DataFormat.Range.Flags & SAHPI_SRF_MAX;
}

/*****************************************************************************
 * Get the minimum value for a threshold.
 *****************************************************************************/

SaHpiSensorReadingUnionT SensorHelper::getMinValue(SaHpiSensorRecT * sensorRec) {
    SaHpiSensorReadingUnionT minValue;

    if (sensorRec->DataFormat.Range.Flags & SAHPI_SRF_MIN) {
        return sensorRec->DataFormat.Range.Min.Value;
    } else {

        switch (sensorRec->DataFormat.ReadingType) {
        case SAHPI_SENSOR_READING_TYPE_INT64:
            minValue.SensorInt64 = MIN_INT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_UINT64:
            minValue.SensorUint64 = MIN_UINT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_FLOAT64:
            minValue.SensorFloat64 = MIN_FLOAT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_BUFFER:
            break;
        }

        return minValue;
    }
}

/*****************************************************************************
 * Get the maximum value for a threshold.
 *****************************************************************************/

SaHpiSensorReadingUnionT SensorHelper::getMaxValue(SaHpiSensorRecT * sensorRec) {
    SaHpiSensorReadingUnionT maxValue;

    if (sensorRec->DataFormat.Range.Flags & SAHPI_SRF_MAX) {
        return sensorRec->DataFormat.Range.Max.Value;
    } else {

        switch (sensorRec->DataFormat.ReadingType) {
        case SAHPI_SENSOR_READING_TYPE_INT64:
            maxValue.SensorInt64 = MAX_INT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_UINT64:
            maxValue.SensorUint64 = MAX_UINT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_FLOAT64:
            maxValue.SensorFloat64 = MAX_FLOAT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_BUFFER:
            break;
        }

        return maxValue;
    }
}

/*****************************************************************************
 * Get the minimum reading for a threshold.
 *****************************************************************************/

SaHpiSensorReadingT SensorHelper::getMinReading(SaHpiSensorRecT * sensorRec) {
    SaHpiSensorReadingT minValue;

    if (sensorRec->DataFormat.Range.Flags & SAHPI_SRF_MIN) {
        return sensorRec->DataFormat.Range.Min;
    } else {

        minValue.IsSupported = SAHPI_TRUE;
        minValue.Type = sensorRec->DataFormat.ReadingType;

        switch (sensorRec->DataFormat.ReadingType) {
        case SAHPI_SENSOR_READING_TYPE_INT64:
            minValue.Value.SensorInt64 = MIN_INT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_UINT64:
            minValue.Value.SensorUint64 = MIN_UINT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_FLOAT64:
            minValue.Value.SensorFloat64 = MIN_FLOAT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_BUFFER:
            break;
        }

        return minValue;
    }
}

/*****************************************************************************
 * Get the maximum reading for a threshold.
 *****************************************************************************/

SaHpiSensorReadingT SensorHelper::getMaxReading(SaHpiSensorRecT * sensorRec) {
    SaHpiSensorReadingT maxValue;

    if (sensorRec->DataFormat.Range.Flags & SAHPI_SRF_MAX) {
        return sensorRec->DataFormat.Range.Max;
    } else {

        maxValue.IsSupported = SAHPI_TRUE;
        maxValue.Type = sensorRec->DataFormat.ReadingType;

        switch (sensorRec->DataFormat.ReadingType) {
        case SAHPI_SENSOR_READING_TYPE_INT64:
            maxValue.Value.SensorInt64 = MAX_INT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_UINT64:
            maxValue.Value.SensorUint64 = MAX_UINT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_FLOAT64:
            maxValue.Value.SensorFloat64 = MAX_FLOAT64;
            break;

        case SAHPI_SENSOR_READING_TYPE_BUFFER:
            break;
        }

        return maxValue;
    }
}

/*****************************************************************************
 * Clear all of the IsSupported fields in a set of thresholds.
 *****************************************************************************/

void SensorHelper::clearSupported(SaHpiSensorThresholdsT *sensorThresholds) {
    sensorThresholds->LowCritical.IsSupported = SAHPI_FALSE;
    sensorThresholds->LowMajor.IsSupported = SAHPI_FALSE;
    sensorThresholds->LowMinor.IsSupported = SAHPI_FALSE;
    sensorThresholds->UpCritical.IsSupported = SAHPI_FALSE;
    sensorThresholds->UpMajor.IsSupported = SAHPI_FALSE;
    sensorThresholds->UpMinor.IsSupported = SAHPI_FALSE;
    sensorThresholds->PosThdHysteresis.IsSupported = SAHPI_FALSE;
    sensorThresholds->NegThdHysteresis.IsSupported = SAHPI_FALSE;
}

/*****************************************************************************
 * Get a reference to specific threshold from a set of thresholds based upon
 * the threshold mask.
 *****************************************************************************/

SaHpiSensorReadingT &SensorHelper::getThreshold(SaHpiSensorThresholdsT *sensorThresholds,
                                                SaHpiSensorThdMaskT mask) {
    if (mask & SAHPI_STM_LOW_MINOR) {
        return sensorThresholds->LowMinor;
    } else if (mask & SAHPI_STM_LOW_MAJOR) {
        return sensorThresholds->LowMajor;
    } else if (mask & SAHPI_STM_LOW_CRIT) {
        return sensorThresholds->LowCritical;
    } else if (mask & SAHPI_STM_UP_MINOR) {
        return sensorThresholds->UpMinor;
    } else if (mask & SAHPI_STM_UP_MAJOR) {
        return sensorThresholds->UpMajor;
    } else if (mask & SAHPI_STM_UP_CRIT) {
        return sensorThresholds->UpCritical;
    } else if (mask & SAHPI_STM_UP_HYSTERESIS) {
        return sensorThresholds->PosThdHysteresis;
    } else { // mask & SAHPI_STM_LOW_HYSTERESIS
        return sensorThresholds->NegThdHysteresis;
    }
}

/*****************************************************************************
 * Set the threshold in a set of thresholds based upon the threshold mask.
 *****************************************************************************/

void SensorHelper::setThreshold(SaHpiSensorThresholdsT *sensorThresholds,
                                SaHpiSensorThdMaskT mask,
                                SaHpiSensorReadingT &reading) {
    if (mask & SAHPI_STM_LOW_MINOR) {
        sensorThresholds->LowMinor = reading;
    } else if (mask & SAHPI_STM_LOW_MAJOR) {
        sensorThresholds->LowMajor = reading;
    } else if (mask & SAHPI_STM_LOW_CRIT) {
        sensorThresholds->LowCritical = reading;
    } else if (mask & SAHPI_STM_UP_MINOR) {
        sensorThresholds->UpMinor = reading;
    } else if (mask & SAHPI_STM_UP_MAJOR) {
        sensorThresholds->UpMajor = reading;
    } else if (mask & SAHPI_STM_UP_CRIT) {
        sensorThresholds->UpCritical = reading;
    } else if (mask & SAHPI_STM_UP_HYSTERESIS) {
        sensorThresholds->PosThdHysteresis = reading;
    } else if (mask & SAHPI_STM_LOW_HYSTERESIS) {
        sensorThresholds->NegThdHysteresis = reading;
    }
}

/*****************************************************************************
 * Are the thresholds valid or not?  Include a report of anything that is
 * invalid.  
 *****************************************************************************/

bool SensorHelper::isValidThresholds(SaHpiSensorThresholdsT *sensorThresholds,
                                     SaHpiSensorRecT *sensorRec, Report &report) {
    bool valid = true;

    char * name[] = { "LowCritical","LowMajor", "LowMinor", 
                      "UpMinor", "UpMajor", "UpCritical",
                      "PosThdHysteresis", "NegThdHysteresis" };

    SaHpiSensorThdMaskT mask[] = { SAHPI_STM_LOW_CRIT, SAHPI_STM_LOW_MAJOR,
                                   SAHPI_STM_LOW_MINOR, SAHPI_STM_UP_MINOR,
                                   SAHPI_STM_UP_MAJOR, SAHPI_STM_UP_CRIT,
                                   SAHPI_STM_UP_HYSTERESIS, SAHPI_STM_LOW_HYSTERESIS };

    // Each threshold (and hysteresis) must be valid.

    for (int i = 0; i < 8; i++) {
        SaHpiSensorReadingT &reading = getThreshold(sensorThresholds, mask[i]);
        if (!isThresholdValid(sensorRec, name[i], reading, mask[i], report)) {
                 valid = false;
         }
    }

    // The hysteresis must be zero or positive, i.e. not negative.

    SaHpiSensorReadingT &reading = getThreshold(sensorThresholds, SAHPI_STM_UP_HYSTERESIS);
    if (!isHysteresisValid("PosThdHysteresis", reading, report)) {
        valid = false;
    }

    reading = getThreshold(sensorThresholds, SAHPI_STM_LOW_HYSTERESIS);
    if (!isHysteresisValid("NegThdHysteresis", reading, report)) {
        valid = false;
    }

    if (!isOrderValid(sensorThresholds, report)) {
        valid = false;
    }

    return valid;
}

/*****************************************************************************
 * Is the threshold valid or not?  
 *****************************************************************************/

bool SensorHelper::isThresholdValid(const SaHpiSensorRecT * sensorRec,
                                    const char *name,
                                    SaHpiSensorReadingT &threshold,
                                    SaHpiSensorThdMaskT thdMask,
                                    Report &report) {
     bool valid = true;

     if (threshold.IsSupported) {
          if (threshold.Type != sensorRec->DataFormat.ReadingType) {
               valid = false;
               report.add("%s Threshold Type [%s] is not the same as the "
                          "Sensor RDR Reading Type [%s].", name, 
                          HpiString::sensorReadingType(threshold.Type),
                          HpiString::sensorReadingType(sensorRec->DataFormat.ReadingType));
          }

          if (!(sensorRec->ThresholdDefn.ReadThold & thdMask)) {
               valid = false;
               report.add("%s is not supported in the RDR ReadThold field [0x%X].", 
                          name, sensorRec->ThresholdDefn.ReadThold);
          }
     } else if (sensorRec->ThresholdDefn.ReadThold & thdMask) {
          valid = false;
          report.add("%s is not supported but it's bit is set in the "
                     "RDR ReadThold field [0x%X].",
                     name, sensorRec->ThresholdDefn.ReadThold);
     }

     return valid;
}

/*****************************************************************************
 * Is the hysteresis valid?
 *****************************************************************************/

bool SensorHelper::isHysteresisValid(const char *name, 
                                     SaHpiSensorReadingT &hysteresis, 
                                     Report &report) {
    bool valid = true;

    if (hysteresis.IsSupported && isNegative(hysteresis)) {
        valid = false;
        report.add("%s value is negative!", name);
    }

    return valid;
}

/*****************************************************************************
 * Is the order of the thresholds valid or not?
 *   lowCritical <= lowMajor <= lowMinor <= upMinor <= UpMajor <= UpCritical
 *****************************************************************************/

bool SensorHelper::isOrderValid(SaHpiSensorThresholdsT *sensorThresholds,
                                Report &report) {
    bool valid = true;
    SaHpiSensorReadingT *cur = NULL;
    SaHpiSensorThdMaskT mask[] = { SAHPI_STM_LOW_CRIT, SAHPI_STM_LOW_MAJOR,
                                   SAHPI_STM_LOW_MINOR, SAHPI_STM_UP_MINOR,
                                   SAHPI_STM_UP_MAJOR, SAHPI_STM_UP_CRIT };

    for (int i = 0; i < 6; i++) {
        SaHpiSensorReadingT &reading = getThreshold(sensorThresholds, mask[i]);
        if (reading.IsSupported) {
            if (cur == NULL) {
                cur = &reading;
            } else if (compare(reading, *cur) >= 0) {
                cur = &reading;
            } else {
                valid = false;
                report.add("Thresholds are out-of-order.");
                break;
            }
        }
    }

    return valid;
}

/*****************************************************************************
 * Compare two sensor readings and return one of the following:
 *
 *     -1 : r1 < r2
 *      0 : r1 == r2 (the two readings are equivalent)
 *     +1 : r1 > r2
 *
 * NOTE: For a BUFFER, -1 will never be returned.  If the two buffer are
 *       different, +1 is returned.
 *****************************************************************************/

int SensorHelper::compare(SaHpiSensorReadingT &r1, SaHpiSensorReadingT &r2) {
    if (r1.Type != r2.Type) {
        return -2;  // should never get here!!
    } else {
        switch (r1.Type) {
            case SAHPI_SENSOR_READING_TYPE_INT64 :
                if (r1.Value.SensorInt64 == r2.Value.SensorInt64) {
                    return 0;
                } else if (r1.Value.SensorInt64 > r2.Value.SensorInt64) {
                    return 1;
                } else {
                    return -1;
                }

            case SAHPI_SENSOR_READING_TYPE_UINT64 :
                if (r1.Value.SensorUint64 == r2.Value.SensorUint64) {
                    return 0;
                } else if (r1.Value.SensorUint64 > r2.Value.SensorUint64) {
                    return 1;
                } else {
                    return -1;
                }

            case SAHPI_SENSOR_READING_TYPE_FLOAT64 :
                if (r1.Value.SensorFloat64 == r2.Value.SensorFloat64) {
                    return 0;
                } else if (r1.Value.SensorFloat64 > r2.Value.SensorFloat64) {
                    return 1;
                } else {
                    return -1;
                }

            case SAHPI_SENSOR_READING_TYPE_BUFFER :
                for (int i = 0; i < SAHPI_SENSOR_BUFFER_LENGTH; i++) {
                    if (r1.Value.SensorBuffer[i] != r2.Value.SensorBuffer[i]) {
                        return 1;
                    }
                }
                return 0;
        }

        return -1;
    }
}

/*****************************************************************************
 * Is the sensor reading negative or not?
 *****************************************************************************/

bool SensorHelper::isNegative(SaHpiSensorReadingT &r) {
    switch (r.Type) {
        case SAHPI_SENSOR_READING_TYPE_INT64 :
            return r.Value.SensorInt64 < 0;

        case SAHPI_SENSOR_READING_TYPE_UINT64 :
            return false; // impossible to be negative

        case SAHPI_SENSOR_READING_TYPE_FLOAT64 :
            return r.Value.SensorFloat64 < 0;

        case SAHPI_SENSOR_READING_TYPE_BUFFER :
            return false;
    }

    return false;
}

/*****************************************************************************
 * Increment the sensor reading by a small amount.
 *****************************************************************************/

SaHpiSensorReadingUnionT SensorHelper::increment(SaHpiSensorReadingUnionT &value,
                                                 SaHpiSensorReadingTypeT type) {
    SaHpiSensorReadingUnionT newValue;

    switch (type) {
    case SAHPI_SENSOR_READING_TYPE_INT64:
           newValue.SensorInt64 = value.SensorInt64 + 1;
           break;

    case SAHPI_SENSOR_READING_TYPE_UINT64:
        newValue.SensorUint64 = value.SensorUint64 + 1;
        break;

    case SAHPI_SENSOR_READING_TYPE_FLOAT64:
        newValue.SensorFloat64 = value.SensorFloat64 + SMALL_FLOAT;
        break;

    case SAHPI_SENSOR_READING_TYPE_BUFFER:
        break;
    }

    return newValue;
}

/*****************************************************************************
 * Decrement the sensor reading by a small amount.
 *****************************************************************************/

SaHpiSensorReadingUnionT SensorHelper::decrement(SaHpiSensorReadingUnionT &value,
                                                 SaHpiSensorReadingTypeT type) {
    SaHpiSensorReadingUnionT newValue;

    switch (type) {
    case SAHPI_SENSOR_READING_TYPE_INT64:
           newValue.SensorInt64 = value.SensorInt64 - 1;
           break;

    case SAHPI_SENSOR_READING_TYPE_UINT64:
        newValue.SensorUint64 = value.SensorUint64 - 1;
        break;

    case SAHPI_SENSOR_READING_TYPE_FLOAT64:
        newValue.SensorFloat64 = value.SensorFloat64 - SMALL_FLOAT;
        break;

    case SAHPI_SENSOR_READING_TYPE_BUFFER:
        break;
    }

    return newValue;
}

/*****************************************************************************
 * Is the value at the upper limit of the numeric machine representation?  
 *****************************************************************************/

bool SensorHelper::isAtUpperMachineLimit(SaHpiSensorReadingUnionT &value,
                                         SaHpiSensorReadingTypeT type) {
    switch (type) {
    case SAHPI_SENSOR_READING_TYPE_INT64:
        return (value.SensorInt64 == MAX_INT64);

    case SAHPI_SENSOR_READING_TYPE_UINT64:
        return (value.SensorUint64 == MAX_UINT64);

    case SAHPI_SENSOR_READING_TYPE_FLOAT64:
        return (value.SensorFloat64 >= MAX_FLOAT64 - SMALL_FLOAT);

    case SAHPI_SENSOR_READING_TYPE_BUFFER:
        return false;
    }

    return false;
}

/*****************************************************************************
 * Is the value at the lower limit of the numeric machine representation?  
 *****************************************************************************/

bool SensorHelper::isAtLowerMachineLimit(SaHpiSensorReadingUnionT &value,
                                         SaHpiSensorReadingTypeT type) {
    switch (type) {
    case SAHPI_SENSOR_READING_TYPE_INT64:
        return (value.SensorInt64 == MIN_INT64);

    case SAHPI_SENSOR_READING_TYPE_UINT64:
        return (value.SensorUint64 == MIN_UINT64);

    case SAHPI_SENSOR_READING_TYPE_FLOAT64:
        return (value.SensorFloat64 <= MIN_FLOAT64 + SMALL_FLOAT);

    case SAHPI_SENSOR_READING_TYPE_BUFFER:
        return false;
    }

    return false;
}

/*****************************************************************************
 * Does this sensor support either INT64 or FLOAT64?  This function is needed
 * when it is necessary to test a negative value for a threshold.
 *****************************************************************************/

bool SensorHelper::isIntOrFloat(SaHpiSensorRecT *sensorRec) {
    return ((sensorRec->DataFormat.ReadingType == SAHPI_SENSOR_READING_TYPE_INT64) ||
            (sensorRec->DataFormat.ReadingType == SAHPI_SENSOR_READING_TYPE_FLOAT64));
}

/*****************************************************************************
 * Is this a valid sensor type?
 * 2009/05/19 
 * Allow all values (header file: consider new types 'valid but unknown')
 * The test makes only sense if the SAF type is compared to the original value, 
 * like e.g. the corresponding IPMI value (s.a. Mapping Spec)
 *****************************************************************************/

bool SensorHelper::isValidType(SaHpiSensorTypeT type) {
    return ((type >= SAHPI_TEMPERATURE && type <= SAHPI_VERSION_CHANGE) ||
            (type == SAHPI_OPERATIONAL) || 
            (type >= SAHPI_OEM_SENSOR)  ||
            (type == SAHPI_COMM_CHANNEL_LINK_STATE)  ||
            (type == SAHPI_MANAGEMENT_BUS_STATE));
}

/*****************************************************************************
 * Is this a valid EventCtrl?
 *****************************************************************************/

bool SensorHelper::isValidEventCtrl(SaHpiSensorEventCtrlT eventCtrl) {
    return ((eventCtrl == SAHPI_SEC_PER_EVENT) || 
            (eventCtrl == SAHPI_SEC_READ_ONLY_MASKS) ||
            (eventCtrl == SAHPI_SEC_READ_ONLY));
}


/*****************************************************************************
 * Is this a valid Event Category?
 *****************************************************************************/

bool SensorHelper::isValidEventCategory(SaHpiEventCategoryT category) {
    return ((category == SAHPI_EC_UNSPECIFIED) ||
            (category == SAHPI_EC_THRESHOLD) ||
            (category == SAHPI_EC_USAGE) ||
            (category == SAHPI_EC_STATE) ||
            (category == SAHPI_EC_PRED_FAIL) ||
            (category == SAHPI_EC_LIMIT) ||
            (category == SAHPI_EC_PERFORMANCE) ||
            (category == SAHPI_EC_SEVERITY) ||
            (category == SAHPI_EC_PRESENCE) ||
            (category == SAHPI_EC_ENABLE) ||
            (category == SAHPI_EC_AVAILABILITY) || 
            (category == SAHPI_EC_REDUNDANCY) ||
            (category == SAHPI_EC_SENSOR_SPECIFIC) ||
            (category == SAHPI_EC_GENERIC));
}

/*****************************************************************************
 * Is this a valid DataFormat?
 *****************************************************************************/

bool SensorHelper::isValidDataFormat(SaHpiSensorDataFormatT *dataFormat,
                                     Report &report) {
    bool valid = true;

    if (dataFormat->IsSupported) {
        if (!isValidReadingType(dataFormat->ReadingType)) {
            valid = false;
            report.add("DataFormat->ReadingType is invalid [0x%X].",
                       dataFormat->ReadingType);
        }

        if (!isValidUnits(dataFormat->BaseUnits)) {
            valid = false;
            report.add("DataFormat->BaseUnits is invalid [0x%X].",
                       dataFormat->BaseUnits);
        }

        if (!isValidUnits(dataFormat->ModifierUnits)) {
            valid = false;
            report.add("DataFormat->ModifierUnits is invalid [0x%X].",
                       dataFormat->ModifierUnits);
        }

        if (!isValidModifierUse(dataFormat->ModifierUse)) {
            valid = false;
            report.add("DataFormat->ModifierUse is invalid [0x%X].",
                       dataFormat->ModifierUse);
        }

        if (!isValidSensorRange(&dataFormat->Range, dataFormat->ReadingType, report)) {
            valid = false;
        }
    }

    return valid;
}

/*****************************************************************************
 * Is this a valid Threshold Definition? (Currently, always true.)
 *****************************************************************************/

bool SensorHelper::isValidThresholdDefn(SaHpiSensorThdDefnT*, Report&) {
    // maybe check for thresholds we can write, but not read, but that
    // that is not against the spec.
    return true;
}

/*****************************************************************************
 * Is this a valid Reading Type?
 *****************************************************************************/

bool SensorHelper::isValidReadingType(SaHpiSensorReadingTypeT readingType) {
    return ((readingType == SAHPI_SENSOR_READING_TYPE_INT64) ||
            (readingType == SAHPI_SENSOR_READING_TYPE_UINT64) ||
            (readingType == SAHPI_SENSOR_READING_TYPE_FLOAT64) ||
            (readingType == SAHPI_SENSOR_READING_TYPE_BUFFER));
}

/*****************************************************************************
 * Is this a valid Data Units of Measurement?
 *****************************************************************************/

bool SensorHelper::isValidUnits(SaHpiSensorUnitsT units) {
    return (units >= SAHPI_SU_UNSPECIFIED && units < SAHPI_SU_UNCORRECTABLE_ERRORS);
}

/*****************************************************************************
 * Is this a valid Modifier Use for Units of Measurement?
 *****************************************************************************/

bool SensorHelper::isValidModifierUse(SaHpiSensorModUnitUseT use) {
    return ((use == SAHPI_SMUU_NONE) ||
            (use == SAHPI_SMUU_BASIC_OVER_MODIFIER) ||
            (use == SAHPI_SMUU_BASIC_TIMES_MODIFIER));
}

/*****************************************************************************
 * Is the sensor range valid?
 *****************************************************************************/

bool SensorHelper::isValidSensorRange(SaHpiSensorRangeT *range,
                                      SaHpiSensorReadingTypeT readingType,
                                      Report &report) {
    bool valid = true;

    // NOTE: The sensor Range Flags use the first 5 bits in a bit mask.
    //       It is illegal for any upper bits to be set.  

    if (range->Flags > 0x1F) {  
        valid = false;
        report.add("Range->Flags is invalid [0x%X].", range->Flags);
    }

    if (range->Flags & SAHPI_SRF_MIN) {
        if (!range->Min.IsSupported) {
            valid = false;
            report.add("Range->Min.IsSupported is FALSE, but Range->Flags "
                       "has the SAHPI_SRF_MIN bit set.");
        }

        if (range->Min.Type != readingType) {
            valid = false;
            report.add("Range->Min.Type (%s) is not equal to the DataFormat's ReadingType (%s).",
                        HpiString::sensorReadingType(range->Min.Type),
                        HpiString::sensorReadingType(readingType));
        }
    }

    if (range->Flags & SAHPI_SRF_MAX) {
        if (!range->Max.IsSupported) {
            valid = false;
            report.add("Range->Max.IsSupported is FALSE, but Range->Flags "
                       "has the SAHPI_SRF_MAX bit set.");
        }

        if (range->Max.Type != readingType) {
            valid = false;
            report.add("Range->Max.Type (%s) is not equal to the DataFormat's ReadingType (%s).",
                        HpiString::sensorReadingType(range->Max.Type),
                        HpiString::sensorReadingType(readingType));
        }
    }

    if (range->Flags & SAHPI_SRF_NORMAL_MIN) {
        if (!range->NormalMin.IsSupported) {
            valid = false;
            report.add("Range->NormalMin.IsSupported is FALSE, but Range->Flags "
                       "has the SAHPI_SRF_NORMAL_MIN bit set.");
        }

        if (range->NormalMin.Type != readingType) {
            valid = false;
            report.add("Range->NormalMin.Type (%s) is not equal to the DataFormat's ReadingType (%s).",
                        HpiString::sensorReadingType(range->NormalMin.Type),
                        HpiString::sensorReadingType(readingType));
        }
    }

    if (range->Flags & SAHPI_SRF_NORMAL_MAX) {
        if (!range->NormalMax.IsSupported) {
            valid = false;
            report.add("Range->NormalMax.IsSupported is FALSE, but Range->Flags "
                       "has the SAHPI_SRF_NORMAL_MAX bit set.");
        }

        if (range->NormalMax.Type != readingType) {
            valid = false;
            report.add("Range->NormalMax.Type (%s) is not equal to the DataFormat's ReadingType (%s).",
                        HpiString::sensorReadingType(range->NormalMax.Type), 
                        HpiString::sensorReadingType(readingType));
        }
    }

    if (range->Flags & SAHPI_SRF_NOMINAL) {
        if (!range->Nominal.IsSupported) {
            valid = false;
            report.add("Range->Nominal.IsSupported is FALSE, but Range->Flags "
                       "has the SAHPI_SRF_NOMINAL bit set.");
        }

        if (range->Nominal.Type != readingType) {
            valid = false;
            report.add("Range->Nominal.Type (%s) is not equal to the DataFormat's ReadingType (%s).",
                        HpiString::sensorReadingType(range->Nominal.Type),
                        HpiString::sensorReadingType(readingType));
        }
    }

    return valid;
}

/*****************************************************************************
 * Set the IsSupport flags as it is given by the mask
 *****************************************************************************/
void SensorHelper::setMask(SaHpiSensorThresholdsT *sensorThresholds, 
                            SaHpiSensorThdMaskT mask) {

   if (mask & SAHPI_STM_UP_CRIT) {
      sensorThresholds->UpCritical.IsSupported = SAHPI_TRUE;
   } else {
   	  sensorThresholds->UpCritical.IsSupported = SAHPI_FALSE;
   }
   if (mask & SAHPI_STM_UP_MAJOR) {
      sensorThresholds->UpMajor.IsSupported = SAHPI_TRUE;
   } else {
   	  sensorThresholds->UpMajor.IsSupported = SAHPI_FALSE;
   }
   if (mask & SAHPI_STM_UP_MINOR) {
   	  sensorThresholds->UpMinor.IsSupported = SAHPI_TRUE;
   } else {
   	  sensorThresholds->UpMinor.IsSupported = SAHPI_FALSE;
   }
   if (mask & SAHPI_STM_LOW_MINOR) {
   	  sensorThresholds->LowMinor.IsSupported = SAHPI_TRUE;
   } else {
   	  sensorThresholds->LowMinor.IsSupported = SAHPI_FALSE;
   }
   if (mask & SAHPI_STM_LOW_MAJOR) {
   	  sensorThresholds->LowMajor.IsSupported = SAHPI_TRUE;
   } else {
   	  sensorThresholds->LowMajor.IsSupported = SAHPI_FALSE;
   }
   if (mask & SAHPI_STM_LOW_CRIT) {
   	  sensorThresholds->LowCritical.IsSupported = SAHPI_TRUE;
   } else {
   	  sensorThresholds->LowCritical.IsSupported = SAHPI_FALSE;
   }
   if (mask & SAHPI_STM_UP_HYSTERESIS) {
   	  sensorThresholds->PosThdHysteresis.IsSupported = SAHPI_TRUE;
   } else {
   	  sensorThresholds->PosThdHysteresis.IsSupported = SAHPI_FALSE;
   }
   if (mask & SAHPI_STM_LOW_HYSTERESIS) {
   	  sensorThresholds->NegThdHysteresis.IsSupported = SAHPI_TRUE;
   } else {
   	  sensorThresholds->NegThdHysteresis.IsSupported = SAHPI_FALSE;
   }

}
