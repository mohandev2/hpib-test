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
 * 09/11/27 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Add Fumi property
 */

#include "SafTest.h"
#include "HpiTestStatus.h"
#include "HpiTestCase.h"
#include "HpiProperties.h"
#include "Time.h"
#include "HpiString.h"
#include "TextBufferHelper.h"
#include "HpiFunctionId.h"
#include "LocationMgr.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define SEC_TO_NANOSEC  1000000000LL
#define MSEC_TO_NANOSEC 1000000LL

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiTestCase::HpiTestCase() {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiTestCase::HpiTestCase(char *line) : BaseTestCase(line) { 
}

/*****************************************************************************
 * Return the expected error code for calling the HPI function that is
 * currently being tested.  This method must be overridden to return the
 * expected error code if necessary.
 *****************************************************************************/

SaErrorT HpiTestCase::getExpectedReturn() {
    return (SaErrorT) -1;
}

/*****************************************************************************
 * If an HPI function can be expected to return more than one error code,
 * then override this function to return a list of error codes.
 *****************************************************************************/

ErrorList *HpiTestCase::getExpectedReturnList() {
    return NULL;
}

/*****************************************************************************
 * Return an invalid Domain ID.
 *****************************************************************************/

SaHpiDomainIdT HpiTestCase::getInvalidDomainId() {
    return getProperties()->getInvalidDomainId();
}

/*****************************************************************************
 * Return an invalid Version.
 *****************************************************************************/

SaHpiVersionT HpiTestCase::getInvalidVersion() {
    return getProperties()->getInvalidVersion();
}

/*****************************************************************************
 * Return an invalid Initialization OptionId.
 *****************************************************************************/

SaHpiInt32T HpiTestCase::getInvalidInitOptionId() {
    return getProperties()->getInvalidInitOptionId();
}

/*****************************************************************************
 * Return an invalid Session ID.
 *****************************************************************************/

SaHpiSessionIdT HpiTestCase::getInvalidSessionId() {
    return getProperties()->getInvalidSessionId();
}

/*****************************************************************************
 * Return a valid Resource ID.
 *****************************************************************************/

SaHpiResourceIdT HpiTestCase::getValidResourceId() {
    return getProperties()->getValidResourceId();
}

/*****************************************************************************
 * Return an invalid Resource ID.
 *****************************************************************************/

SaHpiResourceIdT HpiTestCase::getInvalidResourceId() {
    return getProperties()->getInvalidResourceId();
}

/*****************************************************************************
 * Return an invalid Entry ID.
 *****************************************************************************/

SaHpiEntryIdT HpiTestCase::getInvalidEntryId() {
    return getProperties()->getInvalidEntryId();
}

/*****************************************************************************
 * Return a valid Instrument ID.
 *****************************************************************************/

SaHpiInstrumentIdT HpiTestCase::getValidInstrumentId() {
    return getProperties()->getValidInstrumentId();
}

/*****************************************************************************
 * Return an invalid Instrument ID.
 *****************************************************************************/

SaHpiInstrumentIdT HpiTestCase::getInvalidInstrumentId() {
    return getProperties()->getInvalidInstrumentId();
}

/*****************************************************************************
 * Return a valid Alarm ID.
 *****************************************************************************/

SaHpiAlarmIdT HpiTestCase::getValidAlarmId() {
    return getProperties()->getValidAlarmId();
}

/*****************************************************************************
 * Return a valid RDR Type.
 *****************************************************************************/

SaHpiRdrTypeT HpiTestCase::getValidRdrType() {
    return getProperties()->getValidRdrType();
}

/*****************************************************************************
 * Return a valid Sensor Num.
 *****************************************************************************/

SaHpiSensorNumT HpiTestCase::getValidSensorNum() {
    return getProperties()->getValidSensorNum();
}

/*****************************************************************************
 * Return an invalid Sensor Num.
 *****************************************************************************/

SaHpiSensorNumT HpiTestCase::getInvalidSensorNum() {
    return getProperties()->getInvalidSensorNum();
}

/*****************************************************************************
 * Return a valid Ctrl Num.
 *****************************************************************************/

SaHpiCtrlNumT HpiTestCase::getValidCtrlNum() {
    return getProperties()->getValidCtrlNum();
}

/*****************************************************************************
 * Return an invalid Ctrl Num.
 *****************************************************************************/

SaHpiCtrlNumT HpiTestCase::getInvalidCtrlNum() {
    return getProperties()->getInvalidCtrlNum();
}

/*****************************************************************************
 * Return an invalid IDR Id.
 *****************************************************************************/

SaHpiIdrIdT HpiTestCase::getInvalidIdrId() {
    return getProperties()->getInvalidIdrId();
}

/*****************************************************************************
 * Return a valid IDR Area Id.
 *****************************************************************************/

SaHpiEntryIdT HpiTestCase::getValidIdrAreaId() {
    return getProperties()->getValidIdrAreaId();
}

/*****************************************************************************
 * Return an invalid IDR Area Id.
 *****************************************************************************/

SaHpiEntryIdT HpiTestCase::getInvalidIdrAreaId() {
    return getProperties()->getInvalidIdrAreaId();
}

/*****************************************************************************
 * Return a valid IDR Field Id.
 *****************************************************************************/

SaHpiEntryIdT HpiTestCase::getValidIdrFieldId() {
    return getProperties()->getValidIdrFieldId();
}

/*****************************************************************************
 * Return an invalid IDR Field Id.
 *****************************************************************************/

SaHpiEntryIdT HpiTestCase::getInvalidIdrFieldId() {
    return getProperties()->getInvalidIdrFieldId();
}

/*****************************************************************************
 * Return a valid Watchdog Num.
 *****************************************************************************/

SaHpiWatchdogNumT HpiTestCase::getValidWatchdogNum() {
    return getProperties()->getValidWatchdogNum();
}

/*****************************************************************************
 * Return an invalid Watchdog Num.
 *****************************************************************************/

SaHpiWatchdogNumT HpiTestCase::getInvalidWatchdogNum() {
    return getProperties()->getInvalidWatchdogNum();
}

/*****************************************************************************
 * Return a valid Annunciator Num.
 *****************************************************************************/

SaHpiAnnunciatorNumT HpiTestCase::getValidAnnunciatorNum() {
    return getProperties()->getValidAnnunciatorNum();
}

/*****************************************************************************
 * Return an invalid Annunciator Num.
 *****************************************************************************/

SaHpiAnnunciatorNumT HpiTestCase::getInvalidAnnunciatorNum() {
    return getProperties()->getInvalidAnnunciatorNum();
}

/*****************************************************************************
 * Return a valid Announcement Id.
 *****************************************************************************/

SaHpiEntryIdT HpiTestCase::getValidAnnouncementId() {
    return getProperties()->getValidAnnouncementId();
}

/*****************************************************************************
 * Return an invalid Announcement Id.
 *****************************************************************************/

SaHpiEntryIdT HpiTestCase::getInvalidAnnouncementId() {
    return getProperties()->getInvalidAnnouncementId();
}

/*****************************************************************************
 * Return a valid Fumi Num.
 *****************************************************************************/

SaHpiFumiNumT HpiTestCase::getValidFumiNum() {
    return getProperties()->getValidFumiNum();
}

/*****************************************************************************
 * Return an invalid Fumi Num.
 *****************************************************************************/

SaHpiFumiNumT HpiTestCase::getInvalidFumiNum() {
    return getProperties()->getInvalidFumiNum();
}

/*****************************************************************************
 * Return the test timeout duration in seconds.
 *****************************************************************************/

int HpiTestCase::getTimeout() {
    return getProperties()->getTestTimeout();
}

/*****************************************************************************
 * Return the maximum size of the Event Queue.
 *****************************************************************************/

int HpiTestCase::getMaxEventQueueSize() {
    return getProperties()->getMaxEventQueueSize();
}

/*****************************************************************************
 * Maximum number of unwanted events to skip over before discontinuing the
 * tests.
 *****************************************************************************/

int HpiTestCase::getMaxEventRetries() {
    return getProperties()->getMaxEventRetries();
}

/*****************************************************************************
 * The number of seconds to wait for an event to show up in the Event Queue.
 *****************************************************************************/

int HpiTestCase::getEventDelayTime() { // seconds
    return getProperties()->getEventDelayTime();
}

/*****************************************************************************
 * Return the Epsilon time in milliseconds that is used for comparing 
 * time values.  See the below description on equalTime().
 *****************************************************************************/

TimeMsec HpiTestCase::getEpsilonTime() {
    return getProperties()->getEpsilonTime();
}

/*****************************************************************************
 * Return whether or not to force a discovery.
 *****************************************************************************/

bool HpiTestCase::isForceDiscovery() {
    return getProperties()->isForceDiscovery();
}

/*****************************************************************************
 * Return the Hot Swap Test Timeout.
 *****************************************************************************/

int HpiTestCase::getHsTestTimeout() {
    return getProperties()->getHsTestTimeout();
}

/*****************************************************************************
 * Return the Hot Swap Auto Insertion Policy Time.
 *****************************************************************************/

TimeMsec HpiTestCase::getAutoInsertionPolicyTime() {
    return getProperties()->getAutoInsertionTime();
}

/*****************************************************************************
 * Return the Hot Swap Auto Extraction Policy Time.
 *****************************************************************************/

TimeMsec HpiTestCase::getAutoExtractionPolicyTime() {
    return getProperties()->getAutoExtractionTime();
}

/*****************************************************************************
 * Pause in order to wait for the event processing to hopefully complete.
 *****************************************************************************/

void HpiTestCase::pauseForEventProcessing() {
    secSleep(getEventDelayTime());
}

/*****************************************************************************
 * Pause for Auto Insertion policy.
 *****************************************************************************/

void HpiTestCase::pauseForAutoInsertionPolicy() {
    msecSleep(getProperties()->getAutoInsertionTime());
}

/*****************************************************************************
 * Pause for Auto Extraction policy.
 *****************************************************************************/

void HpiTestCase::pauseForAutoExtractionPolicy() {
    msecSleep(getProperties()->getAutoExtractionTime());
}

/*****************************************************************************
 * Pause for Power On.
 *****************************************************************************/

void HpiTestCase::pauseForPowerOn() {
    msecSleep(getProperties()->getPowerOnTime());
}

/*****************************************************************************
 * Pause for Power Off.
 *****************************************************************************/

void HpiTestCase::pauseForPowerOff() {
    msecSleep(getProperties()->getPowerOffTime());
}

/*****************************************************************************
 * Pause for Power Cycle Transition.
 *****************************************************************************/

void HpiTestCase::pauseForPowerCycleTransition() {
    msecSleep(getProperties()->getPowerCycleTransitionTime());
}

/*****************************************************************************
 * Return the Power On Time.
 *****************************************************************************/

TimeMsec HpiTestCase::getPowerOnTime() {
    return getProperties()->getPowerOnTime();
}

/*****************************************************************************
 * Return the Power Off Time.
 *****************************************************************************/

TimeMsec HpiTestCase::getPowerOffTime() {
    return getProperties()->getPowerOffTime();
}

/*****************************************************************************
 * Pause for Warm Reset.
 *****************************************************************************/

void HpiTestCase::pauseForWarmReset() {
    msecSleep(getProperties()->getWarmResetTime());
}

/*****************************************************************************
 * Pause for Cold Reset.
 *****************************************************************************/

void HpiTestCase::pauseForColdReset() {
    msecSleep(getProperties()->getColdResetTime());
}

/*****************************************************************************
 * When comparing time, equality between time values can be considered
 * equivalent if they are close enough, i.e. their difference is within
 * the epsilon time.
 *****************************************************************************/

bool HpiTestCase::isMsecTimeEqual(TimeMsec time1, TimeMsec time2) {
    return ABS(time1 - time2) <= getEpsilonTime();
}

/*****************************************************************************
 * When comparing time, equality between time values can be considered
 * equivalent if they are close enough, i.e. their difference is within
 * the epsilon time.
 *****************************************************************************/

bool HpiTestCase::isNanoTimeEqual(SaHpiTimeT time1, SaHpiTimeT time2) {
       return (ABS(time1 - time2) <= (getEpsilonTime() * MSEC_TO_NANOSEC));
}

/*****************************************************************************
 * Determine if two HPI boolean values are equal or not.  For HPI booleans,
 * a TRUE value is any non-zero value.  Thus, integers 2 and 3 are both true
 * and are thus equivalent.
 *****************************************************************************/

bool HpiTestCase::isBoolEqual(SaHpiBoolT b1, SaHpiBoolT b2) {
    if (b1 == SAHPI_FALSE && b2 == SAHPI_FALSE) {
        return true;
    } else if (b1 != SAHPI_FALSE && b2 != SAHPI_FALSE) {
        return true;
    } else {
        return false;
    }
}

/*****************************************************************************
 * Prompt the user.
 *****************************************************************************/

void HpiTestCase::prompt(SaHpiSessionIdT sessionId, const char *message) {
    char buf[1000];
    char domain[100];
    SaHpiDomainInfoT domainInfo;

    pauseTimer();

    SaErrorT error = saHpiDomainInfoGet(sessionId, &domainInfo);
    if (error != SA_OK) {
        sprintf(domain, "UNKNOWN");
    } else {
        sprintf(domain, "0x%X", domainInfo.DomainId);
    } 

    sprintf(buf, "Test Case: %s\nDomain: %s\n\n%s", getPath(), domain, message);
    getHpiExecutive()->prompt(buf);

    resumeTimer();
}

/*****************************************************************************
 * Prompt the user to insert an FRU.
 *****************************************************************************/

void HpiTestCase::promptInsertFru(SaHpiSessionIdT sessionId) {
    prompt(sessionId, "Please insert an FRU.");
    pauseForEventProcessing();
}

/*****************************************************************************
 * Prompt the user to insert an FRU.
 *****************************************************************************/

void HpiTestCase::promptInsertFruNoWait(SaHpiSessionIdT sessionId) {
    prompt(sessionId, "Please insert an FRU.");
}

/*****************************************************************************
 * Prompt to user to remove an FRU.
 *****************************************************************************/

void HpiTestCase::promptRemoveFru(SaHpiSessionIdT sessionId) {
    prompt(sessionId, "Please remove an FRU.");
    pauseForEventProcessing();
}

/*****************************************************************************
 * Prompt for an FRU action.  Provide a user-defined message.
 *****************************************************************************/

void HpiTestCase::promptFru(SaHpiSessionIdT sessionId, const char *message) {
    prompt(sessionId, message);
    pauseForEventProcessing();
}

/*****************************************************************************
 * Does this resource have the given capability?
 *****************************************************************************/

bool HpiTestCase::hasCapability(SaHpiRptEntryT *rptEntry, SaHpiCapabilitiesT capability) {
    return (rptEntry->ResourceCapabilities & capability);
}

/*****************************************************************************
 * Does this resource have the SENSOR capability?
 *****************************************************************************/

bool HpiTestCase::hasSensorCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_SENSOR);
}

/*****************************************************************************
 * Does this resource have the EVT_DEASSERTS capability?
 *****************************************************************************/

bool HpiTestCase::hasEvtDeassertsCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_EVT_DEASSERTS);
}

/*****************************************************************************
 * Does this resource have the ANNUNCIATOR capability?
 *****************************************************************************/

bool HpiTestCase::hasAnnunciatorCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_ANNUNCIATOR);
}

/*****************************************************************************
 * Does this resource have the EVENT_LOG capability?
 *****************************************************************************/

bool HpiTestCase::hasEventLogCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_EVENT_LOG);
}

/*****************************************************************************
 * Does this resource have the WATCHDOG capability?
 *****************************************************************************/

bool HpiTestCase::hasWatchdogCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_WATCHDOG);
}

/*****************************************************************************
 * Does this resource have the CONTROL capability?
 *****************************************************************************/

bool HpiTestCase::hasControlCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_CONTROL);
}

/*****************************************************************************
 * Does this resource have the POWER capability?
 *****************************************************************************/

bool HpiTestCase::hasPowerCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_POWER);
}

/*****************************************************************************
 * Does this resource have the RESET capability?
 *****************************************************************************/

bool HpiTestCase::hasResetCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_RESET);
}

/*****************************************************************************
 * Does this resource have the RDR capability?
 *****************************************************************************/

bool HpiTestCase::hasRdrCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_RDR);
}

/*****************************************************************************
 * Does this resource have the INVENTORY_DATA capability?
 *****************************************************************************/

bool HpiTestCase::hasInventoryCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_INVENTORY_DATA);
}

/*****************************************************************************
 * Does this resource have the FRU capability?
 *****************************************************************************/

bool HpiTestCase::hasFruCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_FRU);
}

/*****************************************************************************
 * Does this resource have the MANAGED_HOTSWAP capability?
 *****************************************************************************/

bool HpiTestCase::hasManagedHotSwapCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_MANAGED_HOTSWAP);
}

/*****************************************************************************
 * Does this resource have the FRU HOTSWAP capability?
 *****************************************************************************/

bool HpiTestCase::hasFruHotSwapCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_MANAGED_HOTSWAP);
}

/*****************************************************************************
 * Does this resource have the CONFIGURATION capability?
 *****************************************************************************/

bool HpiTestCase::hasConfigurationCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_CONFIGURATION);
}

/*****************************************************************************
 * Does this resource have the FUMI capability?
 *****************************************************************************/

bool HpiTestCase::hasFumiCapability(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_FUMI);
}

/*****************************************************************************
 * Return the current time in milliseconds.
 *****************************************************************************/

TimeMsec HpiTestCase::getCurrentTime() {
    return Time::getCurrent();
}

/*****************************************************************************
 * Convert msec to nanoseconds.
 *****************************************************************************/

SaHpiTimeT HpiTestCase::msecToNano(TimeMsec msec) {
    return msec * ((SaHpiTimeT) MSEC_TO_NANOSEC);
}

/*****************************************************************************
 * Convert sec to nanoseconds.
 *****************************************************************************/

SaHpiTimeT HpiTestCase::secToNano(long sec) {
    return sec * ((SaHpiTimeT) SEC_TO_NANOSEC);
}

/*****************************************************************************
 * Return the domain id for the given session.
 *****************************************************************************/

SaHpiDomainIdT HpiTestCase::getDomainId(SaHpiSessionIdT sessionId) {
    SaHpiDomainInfoT info;
    SaErrorT error = saHpiDomainInfoGet(sessionId, &info);
    if (error != SA_OK) {
        return SAHPI_UNSPECIFIED_DOMAIN_ID;
    } else {
        return info.DomainId;
    }
}

/*****************************************************************************
 * Get the resource that the HPI User is running on.
 *****************************************************************************/

SaErrorT HpiTestCase::getHpiUserResource(SaHpiSessionIdT sessionId,
                                         SaHpiResourceIdT *hpiResourceId) {
    SaErrorT error = saHpiResourceIdGet(sessionId, hpiResourceId);
    if ((error == SA_ERR_HPI_NOT_PRESENT) || (error == SA_ERR_HPI_UNKNOWN)) {
        error = SA_OK;
        *hpiResourceId = SAHPI_UNSPECIFIED_RESOURCE_ID;
    }

    return error;
}

/*****************************************************************************
 * Return the HPI Executive.
 *****************************************************************************/

HpiExecutive *HpiTestCase::getHpiExecutive() {
    return (HpiExecutive *) getExecutive();
}

/*****************************************************************************
 * Return the Properties.
 *****************************************************************************/

HpiProperties *HpiTestCase::getProperties() {
    return getHpiExecutive()->getProperties();
}

/*****************************************************************************
 * Return the list of Domain IDs.
 *****************************************************************************/

IdList &HpiTestCase::getDomainIdList() {
    return getHpiExecutive()->getDomainIdList();
}

/*****************************************************************************
 * Fill part of a web page with the info for this test case.
 *****************************************************************************/

void HpiTestCase::fillWebPage(ofstream &out) {
    addHtmlLineNumbers(out);
    addHtmlDescription(out);
    addHtmlPrecondition(out);
    addHtmlExpectedReturn(out);
    addHtmlManual(out);
}

/*****************************************************************************
 * Return the expected return codes in HTML format.
 *****************************************************************************/

void HpiTestCase::addHtmlExpectedReturn(ofstream &out) {
    SaErrorT expected = getExpectedReturn();
    if (expected != -1) {
        out << "Expected return: <code>" << HpiString::error(expected) << "</code><br />\n";
    } else {
        ErrorList *elist = getExpectedReturnList();
        if (elist != NULL && elist->getCount() > 0) {
            out << "Expected return: ";

            expected = elist->get(0);
            out << "<code>" << HpiString::error(expected) << "</code>";

            for (int i = 1; i < elist->getCount(); i++) {
                out << " or ";
                expected = elist->get(i);
                out << "<code>" << HpiString::error(expected) << "</code>";
            }
            out << "<br />\n";
        }
    }
}

