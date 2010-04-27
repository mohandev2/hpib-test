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

#ifndef __SENSORHELPER_H__
#define __SENSORHELPER_H__

#include "SafTest.h"

class Report;

/*****************************************************************************
 * Sensor Helper
 *****************************************************************************/

class SensorHelper
{
public:
    static bool isValid(SaHpiSensorRecT *rec, Report &report);

    static bool isEqual(SaHpiSensorRecT *rec1, SaHpiSensorRecT *rec2);
    static bool isEqual(SaHpiSensorRecT *rec1,
                        SaHpiSensorRecT *rec2, Report &report);

    static void fill(SaHpiSensorThresholdsT *sensorThresholds);
    static void fill(SaHpiSensorThresholdsT *sensorThresholds,
                     SaHpiSensorReadingTypeT readingType);

    static void copy(SaHpiSensorThresholdsT *toSensorThresholds,
                     SaHpiSensorThresholdsT *fromSensorThresholds, 
                     SaHpiSensorThdMaskT thdMask);

    static bool isWriteThreshold(SaHpiSensorRecT * sensorRec,
                                 SaHpiSensorThdMaskT mask);
    static bool isReadThreshold(SaHpiSensorRecT * sensorRec,
                                SaHpiSensorThdMaskT mask);
    static bool isReadWriteThreshold(SaHpiSensorRecT * sensorRec,
                                     SaHpiSensorThdMaskT mask);
    static bool hasReadWriteThresholds(SaHpiSensorRecT * sensorRec);
    static bool isFullReadWrite(SaHpiSensorRecT * sensorRec);

    static bool hasMin(SaHpiSensorRecT *sensorRec);
    static bool hasMax(SaHpiSensorRecT *sensorRec);

    static SaHpiSensorReadingUnionT getMinValue(SaHpiSensorRecT *sensorRec);
    static SaHpiSensorReadingUnionT getMaxValue(SaHpiSensorRecT *sensorRec);

    static SaHpiSensorReadingT getMinReading(SaHpiSensorRecT *sensorRec);
    static SaHpiSensorReadingT getMaxReading(SaHpiSensorRecT *sensorRec);

    static void clearSupported(SaHpiSensorThresholdsT *sensorThresholds);

    static SaHpiSensorReadingT &getThreshold(SaHpiSensorThresholdsT *sensorThresholds,
                                             SaHpiSensorThdMaskT mask);

    static void setThreshold(SaHpiSensorThresholdsT *sensorThresholds,
                             SaHpiSensorThdMaskT mask,
                             SaHpiSensorReadingT &reading);

    static bool isValidThresholds(SaHpiSensorThresholdsT *sensorThresholds,
                                  SaHpiSensorRecT *sensorRec, Report &report);

    static bool isThresholdValid(const SaHpiSensorRecT * sensorRec,
                                 const char *name,
                                 SaHpiSensorReadingT &threshold,
                                 SaHpiSensorThdMaskT thdMask,
                                 Report &report);

    static bool isHysteresisValid(const char *name, 
                                  SaHpiSensorReadingT &hysteresis, 
                                  Report &report);

    static bool isOrderValid(SaHpiSensorThresholdsT *sensorThresholds,
                             Report &report);

    static int compare(SaHpiSensorReadingT &r1, SaHpiSensorReadingT &r2);

    static bool isNegative(SaHpiSensorReadingT &r);

    static SaHpiSensorReadingUnionT increment(SaHpiSensorReadingUnionT &value,
                                              SaHpiSensorReadingTypeT type);

    static SaHpiSensorReadingUnionT decrement(SaHpiSensorReadingUnionT &value,
                                              SaHpiSensorReadingTypeT type);

    static bool isAtUpperMachineLimit(SaHpiSensorReadingUnionT &value,
                                      SaHpiSensorReadingTypeT type);

    static bool isAtLowerMachineLimit(SaHpiSensorReadingUnionT &value,
                                      SaHpiSensorReadingTypeT type);

    static bool isIntOrFloat(SaHpiSensorRecT *sensorRec);

    static bool isValidType(SaHpiSensorTypeT type);

    static bool isValidEventCtrl(SaHpiSensorEventCtrlT eventCtrl);

    static bool isValidEventCategory(SaHpiEventCategoryT category);

    static bool isValidDataFormat(SaHpiSensorDataFormatT *dataFormat,
                                  Report &report);

    static bool isValidThresholdDefn(SaHpiSensorThdDefnT *thresholdDefn,
                                     Report &report);

    static bool isValidReadingType(SaHpiSensorReadingTypeT readingType);

    static bool isValidUnits(SaHpiSensorUnitsT units);

    static bool isValidModifierUse(SaHpiSensorModUnitUseT use);

    static bool isValidSensorRange(SaHpiSensorRangeT *range,
                                   SaHpiSensorReadingTypeT readingType,
                                   Report &report);
};

#endif

