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
 * Changes:
 * 09/10/20 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Add Version property
 * 09/11/27 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Add Fumi property
 */

#ifndef __HPIPROPERTIES_H__
#define __HPIPROPERTIES_H__

#include "Properties.h"
#include "Time.h"
#include "SafTest.h"

/*****************************************************************************
 * Constants 
 *****************************************************************************/

#define VALID_ID    0x1
#define INVALID_ID  0xDEADBEEF

/*****************************************************************************
 * HPI Properties
 *****************************************************************************/

class HpiProperties : public Properties
{
private:
    SaHpiDomainIdT invalidDomainId;
    SaHpiSessionIdT invalidSessionId;
	SaHpiVersionT invalidVersion;
	SaHpiInt32T invalidInitOptionId;

    SaHpiResourceIdT invalidResourceId;
    SaHpiResourceIdT validResourceId;

    SaHpiEntryIdT invalidEntryId;

    SaHpiInstrumentIdT validInstrumentId;
    SaHpiInstrumentIdT invalidInstrumentId;

    SaHpiAlarmIdT validAlarmId;

    SaHpiRdrTypeT validRdrType;

    SaHpiSensorNumT validSensorNum;
    SaHpiSensorNumT invalidSensorNum;

    SaHpiCtrlNumT validCtrlNum;
    SaHpiCtrlNumT invalidCtrlNum;

    SaHpiIdrIdT invalidIdrId;

    SaHpiEntryIdT validIdrAreaId;
    SaHpiEntryIdT invalidIdrAreaId;

    SaHpiEntryIdT validIdrFieldId;
    SaHpiEntryIdT invalidIdrFieldId;

    SaHpiWatchdogNumT validWatchdogNum;
    SaHpiWatchdogNumT invalidWatchdogNum;

    SaHpiAnnunciatorNumT validAnnunciatorNum;
    SaHpiAnnunciatorNumT invalidAnnunciatorNum;

    SaHpiEntryIdT validAnnouncementId;
    SaHpiEntryIdT invalidAnnouncementId;
    
    SaHpiFumiNumT validFumiNum;
    SaHpiFumiNumT invalidFumiNum;

    int hsTestTimeout;
    TimeMsec autoInsertionTime;
    TimeMsec autoExtractionTime;

    TimeMsec powerOnTime;
    TimeMsec powerOffTime;
    TimeMsec powerCycleTransitionTime;

    TimeMsec warmResetTime;
    TimeMsec coldResetTime;

    int maxEventQueueSize;
    int maxEventRetries;
    int eventDelayTime;
    TimeMsec epsilonTime;

    bool forceDiscovery;

public:
    HpiProperties();

    /*********************************************************
     * Get the Valid and Invalid values.
     *********************************************************/

    SaHpiDomainIdT getInvalidDomainId();
    SaHpiSessionIdT getInvalidSessionId();
    SaHpiVersionT getInvalidVersion();
    SaHpiInt32T getInvalidInitOptionId();

    SaHpiResourceIdT getValidResourceId();
    SaHpiResourceIdT getInvalidResourceId();

    SaHpiEntryIdT getInvalidEntryId();

    SaHpiInstrumentIdT getValidInstrumentId();
    SaHpiInstrumentIdT getInvalidInstrumentId();

    SaHpiAlarmIdT getValidAlarmId();

    SaHpiRdrTypeT getValidRdrType();

    SaHpiSensorNumT getValidSensorNum();
    SaHpiSensorNumT getInvalidSensorNum();

    SaHpiCtrlNumT getValidCtrlNum();
    SaHpiCtrlNumT getInvalidCtrlNum();

    SaHpiIdrIdT getInvalidIdrId();

    SaHpiEntryIdT getValidIdrAreaId();
    SaHpiEntryIdT getInvalidIdrAreaId();

    SaHpiEntryIdT getValidIdrFieldId();
    SaHpiEntryIdT getInvalidIdrFieldId();

    SaHpiWatchdogNumT getValidWatchdogNum();
    SaHpiWatchdogNumT getInvalidWatchdogNum();

    SaHpiAnnunciatorNumT getValidAnnunciatorNum();
    SaHpiAnnunciatorNumT getInvalidAnnunciatorNum();

    SaHpiEntryIdT getValidAnnouncementId();
    SaHpiEntryIdT getInvalidAnnouncementId();

	SaHpiFumiNumT getValidFumiNum();
	SaHpiFumiNumT getInvalidFumiNum();
	
    /*********************************************************
     * HotSwap properties.
     *********************************************************/

    int getHsTestTimeout();
    TimeMsec getAutoInsertionTime();
    TimeMsec getAutoExtractionTime();

    /*********************************************************
     * Power properties.
     *********************************************************/

    TimeMsec getPowerOnTime();
    TimeMsec getPowerOffTime();
    TimeMsec getPowerCycleTransitionTime();

    /*********************************************************
     * Reset properties.
     *********************************************************/

    TimeMsec getWarmResetTime();
    TimeMsec getColdResetTime();

    /*********************************************************
     * Get general configuration values.
     *********************************************************/

    int getMaxEventQueueSize();
    int getMaxEventRetries();
    int getEventDelayTime();
    TimeMsec getEpsilonTime();
    bool isForceDiscovery();

protected:
    void setDefaults();
    void setPropertyValue(const char *name, const char *value);

private:
    unsigned long toUlong(const char *name, const char *value);
};

#endif

