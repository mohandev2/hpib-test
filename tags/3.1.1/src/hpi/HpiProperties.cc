/*
 * (C) Copyright University of New Hampshire, 2006
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Hpieral Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Hpieral Public License for
 * more details.
 *
 * You should have received a copy of the GNU Hpieral Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Author(s):
 *     Donald A. Barre <dbarre@unh.edu>
 * 
 * Changes:
 * 09/10/20 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Add Version property
 */
#include <stdlib.h>

#include <iostream>
#include <cstring>

#include "HpiProperties.h"

using namespace std;

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

HpiProperties::HpiProperties() {
}

/*****************************************************************************
 * Set the default values.
 *****************************************************************************/

void HpiProperties::setDefaults() {
    Properties::setDefaults();

    invalidDomainId = INVALID_ID;
    invalidSessionId = INVALID_ID;
    invalidVersion = INVALID_ID;
    invalidInitOptionId = INVALID_ID;

    invalidResourceId = INVALID_ID;
    validResourceId = VALID_ID;

    invalidEntryId = INVALID_ID;

    validInstrumentId = VALID_ID;
    invalidInstrumentId = INVALID_ID;

    validAlarmId = VALID_ID;

    validRdrType = SAHPI_SENSOR_RDR;

    validSensorNum = VALID_ID;
    invalidSensorNum = INVALID_ID;

    validCtrlNum = VALID_ID;
    invalidCtrlNum = INVALID_ID;

    invalidIdrId = INVALID_ID;

    validIdrAreaId = VALID_ID;
    invalidIdrAreaId = INVALID_ID;

    validIdrFieldId = VALID_ID;
    invalidIdrFieldId = INVALID_ID;

    validWatchdogNum = VALID_ID;
    invalidWatchdogNum = INVALID_ID;

    validAnnunciatorNum = VALID_ID;
    invalidAnnunciatorNum = INVALID_ID;

    validAnnouncementId = VALID_ID;
    invalidAnnouncementId = INVALID_ID;

    autoInsertionTime = 5000; // five seconds
    autoExtractionTime = 5000; // five seconds

    powerOnTime = 5000; // five seconds
    powerOffTime = 5000; // five seconds
    powerCycleTransitionTime = 10000; // ten seconds

    coldResetTime = 5000; // five seconds
    warmResetTime = 5000; // five seconds

    maxEventQueueSize = 1000; 
    maxEventRetries = 10;
    eventDelayTime = 6;    // seconds
    epsilonTime = 100;     // msec

    forceDiscovery = false;

    hsTestTimeout = 600; // seconds (10 minutes)
}

/*****************************************************************************
 * Set a property based upon a name/value pair.
 *****************************************************************************/

void HpiProperties::setPropertyValue(const char *name, const char *value) {
    if (strcmp(name, "InvalidDomainId") == 0) {
        invalidDomainId = toUlong(name, value);
    } else if (strcmp(name, "InvalidVersion") == 0) { 
    		invalidVersion = toUlong(name, value);
    } else if (strcmp(name, "InvalidInitOptionId") == 0) {
    		invalidInitOptionId = atoi(value);  
    } else if (strcmp(name, "InvalidSessionId") == 0) {
        invalidSessionId = toUlong(name, value); 
    } else if (strcmp(name, "InvalidResourceId") == 0) {
        invalidResourceId = toUlong(name, value);
    } else if (strcmp(name, "ValidResourceId") == 0) {
        validResourceId = toUlong(name, value);
    } else if (strcmp(name, "InvalidEntryId") == 0) {
        invalidEntryId = toUlong(name, value);
    } else if (strcmp(name, "ValidInstrumentId") == 0) {
        validInstrumentId = toUlong(name, value);
    } else if (strcmp(name, "InvalidInstrumentId") == 0) {
        invalidInstrumentId = toUlong(name, value);
    } else if (strcmp(name, "ValidAlarmId") == 0) {
        validAlarmId = toUlong(name, value);
    } else if (strcmp(name, "ValidRdrType") == 0) {
        if (strcmp(value, "SAHPI_CTRL_RDR") == 0) {
            validRdrType = SAHPI_CTRL_RDR;
        } else if (strcmp(value, "SAHPI_SENSOR_RDR") == 0) {
            validRdrType = SAHPI_SENSOR_RDR;
        } else if (strcmp(value, "SAHPI_INVENTORY_RDR") == 0) {
            validRdrType = SAHPI_INVENTORY_RDR;
        } else if (strcmp(value, "SAHPI_WATCHDOG_RDR") == 0) {
            validRdrType = SAHPI_WATCHDOG_RDR;
        } else if (strcmp(value, "SAHPI_ANNUNCIATOR_RDR") == 0) {
            validRdrType = SAHPI_ANNUNCIATOR_RDR;
        } else {
            cout << "Bad ValidRdrType Value: " << value << endl;
            exit(1);
        }
    } else if (strcmp(name, "ValidSensorNum") == 0) {
        validSensorNum = toUlong(name, value);
    } else if (strcmp(name, "InvalidSensorNum") == 0) {
        invalidSensorNum = toUlong(name, value);
    } else if (strcmp(name, "ValidCtrlNum") == 0) {
        validCtrlNum = toUlong(name, value);
    } else if (strcmp(name, "InvalidCtrlNum") == 0) {
        invalidCtrlNum = toUlong(name, value);
    } else if (strcmp(name, "InvalidIdrId") == 0) {
        invalidIdrId = toUlong(name, value);
    } else if (strcmp(name, "ValidIdrAreaId") == 0) {
        validIdrAreaId = toUlong(name, value);
    } else if (strcmp(name, "InvalidIdrAreaId") == 0) {
        invalidIdrAreaId = toUlong(name, value);
    } else if (strcmp(name, "ValidIdrFieldId") == 0) {
        validIdrFieldId = toUlong(name, value);
    } else if (strcmp(name, "InvalidIdrFieldId") == 0) {
        invalidIdrFieldId = toUlong(name, value);
    } else if (strcmp(name, "ValidWatchdogNum") == 0) {
        validWatchdogNum = toUlong(name, value);
    } else if (strcmp(name, "InvalidWatchdogNum") == 0) {
        invalidWatchdogNum = toUlong(name, value);
    } else if (strcmp(name, "ValidAnnunciatorNum") == 0) {
        validAnnunciatorNum = toUlong(name, value);
    } else if (strcmp(name, "InvalidAnnunciatorNum") == 0) {
        invalidAnnunciatorNum = toUlong(name, value);
    } else if (strcmp(name, "ValidAnnouncementId") == 0) {
        validAnnouncementId = toUlong(name, value);
    } else if (strcmp(name, "InvalidAnnouncementId") == 0) {
        invalidAnnouncementId = toUlong(name, value);
    } else if (strcmp(name, "MaxEventQueueSize") == 0) {
        maxEventQueueSize = atoi(value);
    } else if (strcmp(name, "MaxEventRetries") == 0) {
        maxEventRetries = atoi(value);
    } else if (strcmp(name, "EventDelayTime") == 0) {
        eventDelayTime = atoi(value);    // seconds
    } else if (strcmp(name, "HsTestTimeout") == 0) {
        hsTestTimeout = atoi(value);     // seconds
    } else if (strcmp(name, "AutoInsertPolicyTime") == 0) {
        autoInsertionTime = atoi(value);     // msec
    } else if (strcmp(name, "AutoExtractPolicyTime") == 0) {
        autoExtractionTime = atoi(value);     // msec
    } else if (strcmp(name, "PowerOnTime") == 0) {
        powerOnTime = atoi(value);     // msec
    } else if (strcmp(name, "PowerOffTime") == 0) {
        powerOffTime = atoi(value);     // msec
    } else if (strcmp(name, "PowerCycleTransitionTime") == 0) {
        powerCycleTransitionTime = atoi(value);     // msec
    } else if (strcmp(name, "ColdResetTime") == 0) {
        coldResetTime = atoi(value);     // msec
    } else if (strcmp(name, "WarmResetTime") == 0) {
        warmResetTime = atoi(value);     // msec
    } else if (strcmp(name, "EpsilonTime") == 0) {
        epsilonTime = atoi(value);     // msec
    } else if (strcmp(name, "ForceDiscovery") == 0) {
        forceDiscovery = (strcasecmp(value, "true") == 0);
    } else {
        Properties::setPropertyValue(name, value);
    }
}

/*****************************************************************************
 *
 *****************************************************************************/

unsigned long HpiProperties::toUlong(const char *name, const char *value) {
    bool error;
    unsigned long ulongValue = hexToLong(value, &error);
    if (error) {
        cout << name << " Bad Value: " << value << endl;
        exit(1);
    }
    return ulongValue;
}

/*****************************************************************************
 * Return the invalid Domain Id.
 *****************************************************************************/

SaHpiDomainIdT HpiProperties::getInvalidDomainId() {
    return invalidDomainId;
}

/*****************************************************************************
 * Return the invalid Version.
 *****************************************************************************/

SaHpiVersionT HpiProperties::getInvalidVersion() {
    return invalidVersion;
}

/*****************************************************************************
 * Return the invalid Initialization OptionId.
 *****************************************************************************/

SaHpiInt32T HpiProperties::getInvalidInitOptionId() {
	return invalidInitOptionId;
}

/*****************************************************************************
 * Return the invalid Session Id.
 *****************************************************************************/

SaHpiSessionIdT HpiProperties::getInvalidSessionId() {
    return invalidSessionId;
}

/*****************************************************************************
 * Return the valid Resource Id.
 *****************************************************************************/

SaHpiResourceIdT HpiProperties::getValidResourceId() {
    return validResourceId;
}

/*****************************************************************************
 * Return the invalid Resource Id.
 *****************************************************************************/

SaHpiResourceIdT HpiProperties::getInvalidResourceId() {
    return invalidResourceId;
}

/*****************************************************************************
 * Return the invalid Entry Id.
 *****************************************************************************/

SaHpiEntryIdT HpiProperties::getInvalidEntryId() {
    return invalidEntryId;
}

/*****************************************************************************
 * Return the valid Instrument Id.
 *****************************************************************************/

SaHpiInstrumentIdT HpiProperties::getValidInstrumentId() {
    return validInstrumentId;
}

/*****************************************************************************
 * Return the invalid Instrument Id.
 *****************************************************************************/

SaHpiInstrumentIdT HpiProperties::getInvalidInstrumentId() {
    return invalidInstrumentId;
}

/*****************************************************************************
 * Return the valid Alarm Id.
 *****************************************************************************/

SaHpiAlarmIdT HpiProperties::getValidAlarmId() {
    return validAlarmId;
}

/*****************************************************************************
 * Return the valid RDR Type.
 *****************************************************************************/

SaHpiRdrTypeT HpiProperties::getValidRdrType() {
    return validRdrType;
}

/*****************************************************************************
 * Return the valid Sensor Num.
 *****************************************************************************/

SaHpiSensorNumT HpiProperties::getValidSensorNum() {
    return validSensorNum;
}

/*****************************************************************************
 * Return the invalid Sensor Num.
 *****************************************************************************/

SaHpiSensorNumT HpiProperties::getInvalidSensorNum() {
    return invalidSensorNum;
}

/*****************************************************************************
 * Return the valid Ctrl Num.
 *****************************************************************************/

SaHpiCtrlNumT HpiProperties::getValidCtrlNum() {
    return validCtrlNum;
}

/*****************************************************************************
 * Return the invalid Ctrl Num.
 *****************************************************************************/

SaHpiCtrlNumT HpiProperties::getInvalidCtrlNum() {
    return invalidCtrlNum;
}

/*****************************************************************************
 * Return the invalid IDR Id.
 *****************************************************************************/

SaHpiIdrIdT HpiProperties::getInvalidIdrId() {
    return invalidIdrId;
}

/*****************************************************************************
 * Return the valid IDR Area Id.
 *****************************************************************************/

SaHpiEntryIdT HpiProperties::getValidIdrAreaId() {
    return validIdrAreaId;
}

/*****************************************************************************
 * Return the invalid IDR Area Id.
 *****************************************************************************/

SaHpiEntryIdT HpiProperties::getInvalidIdrAreaId() {
    return invalidIdrAreaId;
}

/*****************************************************************************
 * Return the valid IDR Field Id.
 *****************************************************************************/

SaHpiEntryIdT HpiProperties::getValidIdrFieldId() {
    return validIdrFieldId;
}

/*****************************************************************************
 * Return the invalid IDR Field Id.
 *****************************************************************************/

SaHpiEntryIdT HpiProperties::getInvalidIdrFieldId() {
    return invalidIdrFieldId;
}

/*****************************************************************************
 * Return the valid Watchdog Num.
 *****************************************************************************/

SaHpiWatchdogNumT HpiProperties::getValidWatchdogNum() {
    return validWatchdogNum;
}

/*****************************************************************************
 * Return the invalid Watchdog Num.
 *****************************************************************************/

SaHpiWatchdogNumT HpiProperties::getInvalidWatchdogNum() {
    return invalidWatchdogNum;
}

/*****************************************************************************
 * Return the valid Annunciator Num.
 *****************************************************************************/

SaHpiAnnunciatorNumT HpiProperties::getValidAnnunciatorNum() {
    return validAnnunciatorNum;
}

/*****************************************************************************
 * Return the invalid Annunciator Num.
 *****************************************************************************/

SaHpiAnnunciatorNumT HpiProperties::getInvalidAnnunciatorNum() {
    return invalidAnnunciatorNum;
}

/*****************************************************************************
 * Return the valid Announcement Id.
 *****************************************************************************/

SaHpiEntryIdT HpiProperties::getValidAnnouncementId() {
    return validAnnouncementId;
}

/*****************************************************************************
 * Return the invalid Announcement Id.
 *****************************************************************************/

SaHpiEntryIdT HpiProperties::getInvalidAnnouncementId() {
    return invalidAnnouncementId;
}

/*****************************************************************************
 * Return the Hot Swap Test Timeout.
 *****************************************************************************/

int HpiProperties::getHsTestTimeout() {
    return hsTestTimeout;
}

/*****************************************************************************
 * Return the Auto Insertion Time.
 *****************************************************************************/

TimeMsec HpiProperties::getAutoInsertionTime() {
    return autoInsertionTime;
}

/*****************************************************************************
 * Return the Auto Extraction Time.
 *****************************************************************************/

TimeMsec HpiProperties::getAutoExtractionTime() {
    return autoExtractionTime;
}

/*****************************************************************************
 * Return the Power On Time.
 *****************************************************************************/

TimeMsec HpiProperties::getPowerOnTime() {
    return powerOnTime;
}

/*****************************************************************************
 * Return the Power Off Time.
 *****************************************************************************/

TimeMsec HpiProperties::getPowerOffTime() {
    return powerOffTime;
}

/*****************************************************************************
 * Return the Power Cycle Time.
 *****************************************************************************/

TimeMsec HpiProperties::getPowerCycleTransitionTime() {
    return powerCycleTransitionTime;
}

/*****************************************************************************
 * Return the Warm Reset Time.
 *****************************************************************************/

TimeMsec HpiProperties::getWarmResetTime() {
    return warmResetTime;
}

/*****************************************************************************
 * Return the Cold Reset Time.
 *****************************************************************************/

TimeMsec HpiProperties::getColdResetTime() {
    return coldResetTime;
}

/*****************************************************************************
 * Return the Max Event Queue Size.
 *****************************************************************************/

int HpiProperties::getMaxEventQueueSize() {
    return maxEventQueueSize;
}

/*****************************************************************************
 * Return the Max Event Retries.
 *****************************************************************************/

int HpiProperties::getMaxEventRetries() {
    return maxEventRetries;
}

/*****************************************************************************
 * Return the Event Delay Time.
 *****************************************************************************/

int HpiProperties::getEventDelayTime() {
    return eventDelayTime;
}

/*****************************************************************************
 * Return the Epsilon Time.
 *****************************************************************************/

TimeMsec HpiProperties::getEpsilonTime() {
    return epsilonTime;
}

/*****************************************************************************
 * Return whether or not to force a domain discovery.
 *****************************************************************************/

bool HpiProperties::isForceDiscovery() {
    return forceDiscovery;
}

