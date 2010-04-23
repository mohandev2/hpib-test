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

#ifndef __HPITESTCASE_H__
#define __HPITESTCASE_H__

#include <string>
#include "SafTest.h"
#include "ErrorList.h"
#include "BaseTestCase.h"
#include "HpiString.h"
#include "UserPrompter.h"
#include "HpiTestStatus.h"
#include "HpiExecutive.h"
#include "IdList.h"
#include "Util.h"

using namespace std;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define SESSION_TIMEOUT_OFFSET 5  // seconds

/*****************************************************************************
 * HPI Test Case
 *****************************************************************************/

class HpiTestCase : public BaseTestCase
{
protected:
    HpiTestCase();
    HpiTestCase(char *line);

    virtual SaErrorT getExpectedReturn();
    virtual ErrorList *getExpectedReturnList();

    /*********************************************************
     * Get the Valid and Invalid values.
     *********************************************************/

    SaHpiDomainIdT getInvalidDomainId();
    SaHpiVersionT getInvalidVersion();
    SaHpiInt32T getInvalidInitOptionId();
    SaHpiSessionIdT getInvalidSessionId();

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

    /*********************************************************
     * Get general configuration values.
     *********************************************************/

    int getTimeout();
    int getMaxEventQueueSize();
    int getMaxEventRetries();
    int getEventDelayTime();
    TimeMsec getEpsilonTime();
    bool isForceDiscovery();

    /*********************************************************
     * Convenience equality methods.
     *********************************************************/

    bool isMsecTimeEqual(TimeMsec time1, TimeMsec time2);
    bool isNanoTimeEqual(SaHpiTimeT time1, SaHpiTimeT time2);
    bool isBoolEqual(SaHpiBoolT b1, SaHpiBoolT b2);

    /*********************************************************
     * Prompt the user.
     *********************************************************/

    void prompt(SaHpiSessionIdT sessionId, const char *message);
    void promptInsertFru(SaHpiSessionIdT sessionId);
    void promptInsertFruNoWait(SaHpiSessionIdT sessionId);
    void promptRemoveFru(SaHpiSessionIdT sessionId);
    void promptFru(SaHpiSessionIdT sessionId, const char *message);

    /*********************************************************
     * Capability methods.
     *********************************************************/

    bool hasCapability(SaHpiRptEntryT *rptEntry, SaHpiCapabilitiesT capability);
    bool hasSensorCapability(SaHpiRptEntryT *rptEntry);
    bool hasEvtDeassertsCapability(SaHpiRptEntryT *rptEntry);
    bool hasAnnunciatorCapability(SaHpiRptEntryT *rptEntry);
    bool hasEventLogCapability(SaHpiRptEntryT *rptEntry);
    bool hasWatchdogCapability(SaHpiRptEntryT *rptEntry);
    bool hasControlCapability(SaHpiRptEntryT *rptEntry);
    bool hasPowerCapability(SaHpiRptEntryT *rptEntry);
    bool hasResetCapability(SaHpiRptEntryT *rptEntry);
    bool hasRdrCapability(SaHpiRptEntryT *rptEntry);
    bool hasInventoryCapability(SaHpiRptEntryT *rptEntry);
    bool hasFruCapability(SaHpiRptEntryT *rptEntry);
    bool hasManagedHotSwapCapability(SaHpiRptEntryT *rptEntry);
    bool hasConfigurationCapability(SaHpiRptEntryT *rptEntry);

    /*********************************************************
     * Miscellaneous methods
     *********************************************************/

    TimeMsec getCurrentTime();
    SaHpiTimeT msecToNano(TimeMsec msec);
    SaHpiTimeT secToNano(long sec);

    int getHsTestTimeout();
    TimeMsec getAutoInsertionPolicyTime();
    TimeMsec getAutoExtractionPolicyTime();

    void pauseForEventProcessing();

    void pauseForAutoInsertionPolicy();
    void pauseForAutoExtractionPolicy();

    void pauseForPowerOn();
    void pauseForPowerOff();
    void pauseForPowerCycleTransition();
    TimeMsec getPowerOnTime();
    TimeMsec getPowerOffTime();

    void pauseForWarmReset();
    void pauseForColdReset();

    SaHpiDomainIdT getDomainId(SaHpiSessionIdT sessionId);
    SaErrorT getHpiUserResource(SaHpiSessionIdT sessionId,
                                SaHpiResourceIdT *hpiResourceId);

    void fillWebPage(ofstream &out);

    IdList &getDomainIdList();

private:
    HpiExecutive *getHpiExecutive();
    HpiProperties *getProperties();

    void addHtmlExpectedReturn(ofstream &out);
};

#endif
