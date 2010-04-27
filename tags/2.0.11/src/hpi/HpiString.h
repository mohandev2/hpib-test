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

#ifndef __HPISTRING_H__
#define __HPISTRING_H__

#include <string>

#include "SafTest.h"
#include "HpiFunctionId.h"

using namespace std;

/*****************************************************************************
 * HpiString
 *
 * Convert HPI constants and enumerations to strings.
 *****************************************************************************/

class HpiString
{
public:
    static const char *function(HpiFunctionId funcId);

    static const char *error(SaErrorT error);
    static const char *boolean(SaHpiBoolT b);
    static const char *entryId(SaHpiEntryIdT entryId);
    static const char *timeout(SaHpiTimeoutT timeout);

    static const char *dataType(SaHpiTextTypeT dataType);
    static const char *language(SaHpiLanguageT language);
    static const char *textBuffer(SaHpiTextBufferT &buffer);

    static const char *entityType(SaHpiEntityTypeT entityType);
    static const char *entityPath(SaHpiEntityPathT *entityPath);

    static const char *eventCategory(SaHpiEventCategoryT category);
    static const char *eventType(SaHpiEventTypeT eventType);
    static const char *severity(SaHpiSeverityT severity);

    static const char *sensorType(SaHpiSensorTypeT type);
    static const char *sensorReadingType(SaHpiSensorReadingTypeT readingType);
    static const char *sensorUnits(SaHpiSensorUnitsT units);
    static const char *sensorModUnitUse(SaHpiSensorModUnitUseT modifier);
    static const char *sensorEventCtrl(SaHpiSensorEventCtrlT eventCtrl);

    static const char *ctrlType(SaHpiCtrlTypeT ctrlType);
    static const char *ctrlStateDigital(SaHpiCtrlStateDigitalT state);
    static const char *ctrlMode(SaHpiCtrlModeT ctrlMode);
    static const char *ctrlOutputType(SaHpiCtrlOutputTypeT ctrlOutputType);

    static const char *idrAreaType(SaHpiIdrAreaTypeT areaType);
    static const char *idrFieldType(SaHpiIdrFieldTypeT fieldType);

    static const char *watchdogAction(SaHpiWatchdogActionT action);
    static const char *watchdogActionEvent(SaHpiWatchdogActionEventT actionEvent);
    static const char *watchdogPretimerInterrupt(SaHpiWatchdogPretimerInterruptT pretimerInterrupt);
    static const char *watchdogTimerUse(SaHpiWatchdogTimerUseT timerUse);
    static const char *watchdogTimerUseExpFlag(SaHpiWatchdogExpFlagsT flag);

    static const char *hotSwapIndicatorState(SaHpiHsIndicatorStateT state);
    static const char *hotSwapAction(SaHpiHsActionT action);
    static const char *hotSwapState(SaHpiHsStateT state);

    static const char *name(SaHpiNameT *name);
    static const char *statusCondType(SaHpiStatusCondTypeT condType);
    static const char *annunciatorMode(SaHpiAnnunciatorModeT annunciatorMode);
    static const char *annunciatorType(SaHpiAnnunciatorTypeT annunciatorType);

    static const char *rdrType(SaHpiRdrTypeT rdrType);

    static const char *resetAction(SaHpiResetActionT action);

    static const char *powerState(SaHpiPowerStateT powerState);

};

#endif
