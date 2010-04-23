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
 * 2009/05/19 - Lars.Wetzel@emerson.com / Anton.Pak@pigeonpoint.com
 *              enhancements to be HPI-B.02.01 compliant
 */

#include <cstring>

#include "HpiString.h"
#include "StringBuffer.h"

/*****************************************************************************
 * Convert an HPI Function ID to a string.
 *****************************************************************************/

const char *HpiString::function(HpiFunctionId funcId) {

    switch (funcId) {
        case VERSION_GET                  : return "saHpiVersionGet()";

        case SESSION_OPEN                 : return "saHpiSessionOpen()";
        case SESSION_CLOSE                : return "saHpiSessionClose()";
        case SESSION_DISCOVER             : return "saHpiDiscover()";

        case DOMAIN_INFO_GET              : return "saHpiDomainInfoGet()";
        case DRT_ENTRY_GET                : return "saHpiDrtEntryGet()";
        case DOMAIN_TAG_SET               : return "saHpiDomainTagSet()";

        case RPT_ENTRY_GET                : return "saHpiRptEntryGet()";
        case RPT_ENTRY_GET_BY_RESOURCE_ID : return "saHpiRptEntryGetByResourceId()";
        case RESOURCE_SEVERITY_SET        : return "saHpiResourceSeveritySet()";
        case RESOURCE_TAG_SET             : return "saHpiResourceTagSet()";
        case RESOURCE_ID_GET              : return "saHpiResourceIdGet()";
        case RESOURCE_FAILED_REMOVE       : return "saHpiResourceFailedRemove()";
        case GET_ID_BY_ENTITY_PATH        : return "saHpiGetIdByEntityPath()";
        case GET_CHILD_ENTITY_PATH        : return "saHpiGetChildEntityPath()";

        case EVENT_LOG_INFO_GET           : return "saHpiEventLogInfoGet()";
        case EVENT_LOG_ENTRY_GET          : return "saHpiEventLogEntryGet()";
        case EVENT_LOG_ENTRY_ADD          : return "saHpiEventLogEntryAdd()";
        case EVENT_LOG_CLEAR              : return "saHpiEventLogClear()";
        case EVENT_LOG_TIME_GET           : return "saHpiEventLogTimeGet()";
        case EVENT_LOG_TIME_SET           : return "saHpiEventLogTimeSet()";
        case EVENT_LOG_STATE_GET          : return "saHpiEventLogStateGet()";
        case EVENT_LOG_STATE_SET          : return "saHpiEventLogStateSet()";
        case EVENT_LOG_OVERFLOW_RESET     : return "saHpiEventLogOverflowReset()";
        case EVENT_LOG_CAPABILITIES_GET   : return "saHpiEventLogCapabilitiesGet()";

        case SUBSCRIBE                    : return "saHpiSubscribe()";
        case UNSUBSCRIBE                  : return "saHpiUnsubscribe()";
        case EVENT_GET                    : return "saHpiEventGet()";
        case EVENT_ADD                    : return "saHpiEventAdd()";

        case ALARM_GET_NEXT               : return "saHpiAlarmGetNext()";
        case ALARM_GET                    : return "saHpiAlarmGet()";
        case ALARM_ACKNOWLEDGE            : return "saHpiAlarmAcknowledge()";
        case ALARM_ADD                    : return "saHpiAlarmAdd()";
        case ALARM_DELETE                 : return "saHpiAlarmDelete()";

        case RDR_GET                      : return "saHpiRdrGet()";
        case RDR_GET_BY_INSTRUMENT_ID     : return "saHpiRdrGetByInstrumentId()";

        case SENSOR_READING_GET           : return "saHpiSensorReadingGet()";
        case SENSOR_THRESHOLDS_GET        : return "saHpiSensorThresholdsGet()";
        case SENSOR_THRESHOLDS_SET        : return "saHpiSensorThresholdsSet()";
        case SENSOR_TYPE_GET              : return "saHpiSensorTypeGet()";
        case SENSOR_ENABLE_GET            : return "saHpiSensorEnableGet()";
        case SENSOR_ENABLE_SET            : return "saHpiSensorEnableSet()";
        case SENSOR_EVENT_ENABLE_GET      : return "saHpiSensorEventEnableGet()";
        case SENSOR_EVENT_ENABLE_SET      : return "saHpiSensorEventEnableGet()";
        case SENSOR_EVENT_MASKS_GET       : return "saHpiSensorEventMasksGet()";
        case SENSOR_EVENT_MASKS_SET       : return "saHpiSensorEventMasksSet()";

        case CONTROL_TYPE_GET             : return "saHpiControlTypeGet()";
        case CONTROL_GET                  : return "saHpiControlGet()";
        case CONTROL_SET                  : return "saHpiControlSet()";

        case IDR_INFO_GET                 : return "saHpiIdrInfoGet()";
        case IDR_AREA_HEADER_GET          : return "saHpiIdrAreaHeaderGet()";
        case IDR_AREA_ADD                 : return "saHpiIdrAreaAdd()";
        case IDR_AREA_ADD_BY_ID           : return "saHpiIdrAreaAddById()";
        case IDR_AREA_DELETE              : return "saHpiIdrAreaDelete()";
        case IDR_FIELD_GET                : return "saHpiIdrFieldGet()";
        case IDR_FIELD_ADD                : return "saHpiIdrFieldAdd()";
        case IDR_FIELD_ADD_BY_ID          : return "saHpiIdrFieldAddById()";
        case IDR_FIELD_SET                : return "saHpiIdrFieldSet()";
        case IDR_FIELD_DELETE             : return "saHpiIdrFieldDelete()";

        case WATCHDOG_TIMER_GET           : return "saHpiWatchdogTimerGet()";
        case WATCHDOG_TIMER_SET           : return "saHpiWatchdogTimerSet()";
        case WATCHDOG_TIMER_RESET         : return "saHpiWatchdogTimerReset()";

        case ANNUNCIATOR_GET_NEXT         : return "saHpiAnnunciatorGetNext()";
        case ANNUNCIATOR_GET              : return "saHpiAnnunciatorGet()";
        case ANNUNCIATOR_ACKNOWLEDGE      : return "saHpiAnnunciatorAcknowledge()";
        case ANNUNCIATOR_ADD              : return "saHpiAnnunciatorAdd()";
        case ANNUNCIATOR_DELETE           : return "saHpiAnnunciatorDelete()";
        case ANNUNCIATOR_MODE_GET         : return "saHpiAnnunciatorModeGet()";
        case ANNUNCIATOR_MODE_SET         : return "saHpiAnnunciatorModeSet()";

        case HOT_SWAP_POLICY_CANCEL       : return "saHpiHotSwapPolicyCancel()";
        case RESOURCE_ACTIVE_SET          : return "saHpiResourceActiveSet()";
        case RESOURCE_INACTIVE_SET        : return "saHpiResourceInactiveSet()";
        case AUTO_INSERT_TIMEOUT_GET      : return "saHpiAutoInsertTimeoutGet()";
        case AUTO_INSERT_TIMEOUT_SET      : return "saHpiAutoInsertTimeoutSet()";
        case AUTO_EXTRACT_TIMEOUT_GET     : return "saHpiAutoExtractTimeoutGet()";
        case AUTO_EXTRACT_TIMEOUT_SET     : return "saHpiAutoExtractTimeoutSet()";
        case HOT_SWAP_STATE_GET           : return "saHpiHotSwapStateGet()";
        case HOT_SWAP_ACTION_REQUEST      : return "saHpiHotSwapActionRequest()";
        case HOT_SWAP_INDICATOR_STATE_GET : return "saHpiHotSwapIndicatorStateGet()";
        case HOT_SWAP_INDICATOR_STATE_SET : return "saHpiHotSwapIndicatorStateSet()";

        case PARM_CONTROL                 : return "saHpiParmControl()";

        case RESOURCE_RESET_STATE_GET     : return "saHpiResourceResetStateGet()";
        case RESOURCE_RESET_STATE_SET     : return "saHpiResourceResetStateSet()";

        case RESOURCE_POWER_STATE_GET     : return "saHpiResourcePowerStateGet()";
        case RESOURCE_POWER_STATE_SET     : return "saHpiResourcePowerStateSet()";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN HPI FUNCTION [0x%X]", funcId);
    return buf;
}

/*****************************************************************************
 * Convert an HPI Error Code to string format.
 *****************************************************************************/

const char *HpiString::error(SaErrorT error) {

    switch (error) {
        case SA_OK                         : return "SA_OK";
        case SA_ERR_HPI_ERROR              : return "SA_ERR_HPI_ERROR";
        case SA_ERR_HPI_UNSUPPORTED_API    : return "SA_ERR_HPI_UNSUPPORTED_API";
        case SA_ERR_HPI_BUSY               : return "SA_ERR_HPI_BUSY";
        case SA_ERR_HPI_INTERNAL_ERROR     : return "SA_ERR_HPI_INTERNAL_ERROR";
        case SA_ERR_HPI_INVALID_CMD        : return "SA_ERR_HPI_INVALID_CMD";
        case SA_ERR_HPI_TIMEOUT            : return "SA_ERR_HPI_TIMEOUT";
        case SA_ERR_HPI_OUT_OF_SPACE       : return "SA_ERR_HPI_OUT_OF_SPACE";
        case SA_ERR_HPI_OUT_OF_MEMORY      : return "SA_ERR_HPI_OUT_OF_MEMORY";
        case SA_ERR_HPI_INVALID_PARAMS     : return "SA_ERR_HPI_INVALID_PARAMS";
        case SA_ERR_HPI_INVALID_DATA       : return "SA_ERR_HPI_INVALID_DATA";
        case SA_ERR_HPI_NOT_PRESENT        : return "SA_ERR_HPI_NOT_PRESENT";
        case SA_ERR_HPI_NO_RESPONSE        : return "SA_ERR_HPI_NO_RESPONSE";
        case SA_ERR_HPI_DUPLICATE          : return "SA_ERR_HPI_DUPLICATE";
        case SA_ERR_HPI_INVALID_SESSION    : return "SA_ERR_HPI_INVALID_SESSION";
        case SA_ERR_HPI_INVALID_DOMAIN     : return "SA_ERR_HPI_INVALID_DOMAIN";
        case SA_ERR_HPI_INVALID_RESOURCE   : return "SA_ERR_HPI_INVALID_RESOURCE";
        case SA_ERR_HPI_INVALID_REQUEST    : return "SA_ERR_HPI_INVALID_REQUEST";
        case SA_ERR_HPI_ENTITY_NOT_PRESENT : return "SA_ERR_HPI_ENTITY_NOT_PRESENT";
        case SA_ERR_HPI_READ_ONLY          : return "SA_ERR_HPI_READ_ONLY";
        case SA_ERR_HPI_CAPABILITY         : return "SA_ERR_HPI_CAPABILITY";
        case SA_ERR_HPI_UNKNOWN            : return "SA_ERR_HPI_UNKNOWN";
        case SA_ERR_HPI_INVALID_STATE      : return "SA_ERR_HPI_INVALID_STATE";
        case SA_ERR_HPI_UNSUPPORTED_PARAMS : return "SA_ERR_HPI_UNSUPPORTED_PARAMS";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN ERROR CODE [0x%X]", error);
    return buf;
}

/*****************************************************************************
 * Convert an HPI Boolean value to a string.
 *****************************************************************************/

const char *HpiString::boolean(SaHpiBoolT b) {
    return (b) ? "SAHPI_TRUE" : "SAHPI_FALSE";
}

/*****************************************************************************
 * Convert an EntryId to a string.
 *****************************************************************************/

const char *HpiString::entryId(SaHpiEntryIdT entryId) {
    if (entryId == SAHPI_FIRST_ENTRY) {
        return "SAHPI_FIRST_ENTRY";
    } else if (entryId == SAHPI_LAST_ENTRY) {
        return "SAHPI_LAST_ENTRY";
    } else {
        char *buf = StringBuffer::next();
        sprintf(buf, "0x%X", entryId);
        return buf;
    }
}

/*****************************************************************************
 * Convert a Timeout to a string.
 *****************************************************************************/

const char *HpiString::timeout(SaHpiTimeoutT timeout) {
    if (timeout == SAHPI_TIMEOUT_IMMEDIATE) {
        return "SAHPI_TIMEOUT_IMMEDIATE";
    } else if (timeout == SAHPI_TIMEOUT_BLOCK) {
        return "SAHPI_TIMEOUT_BLOCK";
    } else {
        char *buf = StringBuffer::next();
        sprintf(buf, "%lld", timeout);
        return buf;
    }
}

/*****************************************************************************
 * Convert a Data Type to a string.
 *****************************************************************************/

const char *HpiString::dataType(SaHpiTextTypeT dataType) {

    switch (dataType) {
        case SAHPI_TL_TYPE_UNICODE : return "SAHPI_TL_TYPE_UNICODE";
        case SAHPI_TL_TYPE_BCDPLUS : return "SAHPI_TL_TYPE_BCDPLUS";
        case SAHPI_TL_TYPE_ASCII6  : return "SAHPI_TL_TYPE_ASCII6";
        case SAHPI_TL_TYPE_TEXT    : return "SAHPI_TL_TYPE_TEXT";
        case SAHPI_TL_TYPE_BINARY  : return "SAHPI_TL_TYPE_BINARY";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN DATA TYPE [0x%X]", dataType);
    return buf;
}

/*****************************************************************************
 * Convert a Language to a string.
 *****************************************************************************/

const char *HpiString::language(SaHpiLanguageT language) {

    switch (language) {
        case SAHPI_LANG_UNDEF          : return "SAHPI_LANG_UNDEF";
        case SAHPI_LANG_AFAR           : return "SAHPI_LANG_AFAR";
        case SAHPI_LANG_ABKHAZIAN      : return "SAHPI_LANG_ABKHAZIAN";
        case SAHPI_LANG_AFRIKAANS      : return "SAHPI_LANG_AFRIKAANS";
        case SAHPI_LANG_AMHARIC        : return "SAHPI_LANG_AMHARIC";
        case SAHPI_LANG_ARABIC         : return "SAHPI_LANG_ARABIC";
        case SAHPI_LANG_ASSAMESE       : return "SAHPI_LANG_ASSAMESE";
        case SAHPI_LANG_AYMARA         : return "SAHPI_LANG_AYMARA";
        case SAHPI_LANG_AZERBAIJANI    : return "SAHPI_LANG_AZERBAIJANI";
        case SAHPI_LANG_BASHKIR        : return "SAHPI_LANG_BASHKIR";
        case SAHPI_LANG_BYELORUSSIAN   : return "SAHPI_LANG_BYELORUSSIAN";
        case SAHPI_LANG_BULGARIAN      : return "SAHPI_LANG_BULGARIAN";
        case SAHPI_LANG_BIHARI         : return "SAHPI_LANG_BIHARI";
        case SAHPI_LANG_BISLAMA        : return "SAHPI_LANG_BISLAMA";
        case SAHPI_LANG_BENGALI        : return "SAHPI_LANG_BENGALI";
        case SAHPI_LANG_TIBETAN        : return "SAHPI_LANG_TIBETAN";
        case SAHPI_LANG_BRETON         : return "SAHPI_LANG_BRETON";
        case SAHPI_LANG_CATALAN        : return "SAHPI_LANG_CATALAN";
        case SAHPI_LANG_CORSICAN       : return "SAHPI_LANG_CORSICAN";
        case SAHPI_LANG_CZECH          : return "SAHPI_LANG_CZECH";
        case SAHPI_LANG_WELSH          : return "SAHPI_LANG_WELSH";
        case SAHPI_LANG_DANISH         : return "SAHPI_LANG_DANISH";
        case SAHPI_LANG_GERMAN         : return "SAHPI_LANG_GERMAN";
        case SAHPI_LANG_BHUTANI        : return "SAHPI_LANG_BHUTANI";
        case SAHPI_LANG_GREEK          : return "SAHPI_LANG_GREEK";
        case SAHPI_LANG_ENGLISH        : return "SAHPI_LANG_ENGLISH";
        case SAHPI_LANG_ESPERANTO      : return "SAHPI_LANG_ESPERANTO";
        case SAHPI_LANG_SPANISH        : return "SAHPI_LANG_SPANISH";
        case SAHPI_LANG_ESTONIAN       : return "SAHPI_LANG_ESTONIAN";
        case SAHPI_LANG_BASQUE         : return "SAHPI_LANG_BASQUE";
        case SAHPI_LANG_PERSIAN        : return "SAHPI_LANG_PERSIAN";
        case SAHPI_LANG_FINNISH        : return "SAHPI_LANG_FINNISH";
        case SAHPI_LANG_FIJI           : return "SAHPI_LANG_FIJI";
        case SAHPI_LANG_FAEROESE       : return "SAHPI_LANG_FAEROESE";
        case SAHPI_LANG_FRENCH         : return "SAHPI_LANG_FRENCH";
        case SAHPI_LANG_FRISIAN        : return "SAHPI_LANG_FRISIAN";
        case SAHPI_LANG_IRISH          : return "SAHPI_LANG_IRISH";
        case SAHPI_LANG_SCOTSGAELIC    : return "SAHPI_LANG_SCOTSGAELIC";
        case SAHPI_LANG_GALICIAN       : return "SAHPI_LANG_GALICIAN";
        case SAHPI_LANG_GUARANI        : return "SAHPI_LANG_GUARANI";
        case SAHPI_LANG_GUJARATI       : return "SAHPI_LANG_GUJARATI";
        case SAHPI_LANG_HAUSA          : return "SAHPI_LANG_HAUSA";
        case SAHPI_LANG_HINDI          : return "SAHPI_LANG_HINDI";
        case SAHPI_LANG_CROATIAN       : return "SAHPI_LANG_CROATIAN";
        case SAHPI_LANG_HUNGARIAN      : return "SAHPI_LANG_HUNGARIAN";
        case SAHPI_LANG_ARMENIAN       : return "SAHPI_LANG_ARMENIAN";
        case SAHPI_LANG_INTERLINGUA    : return "SAHPI_LANG_INTERLINGUA";
        case SAHPI_LANG_INTERLINGUE    : return "SAHPI_LANG_INTERLINGUE";
        case SAHPI_LANG_INUPIAK        : return "SAHPI_LANG_INUPIAK";
        case SAHPI_LANG_INDONESIAN     : return "SAHPI_LANG_INDONESIAN";
        case SAHPI_LANG_ICELANDIC      : return "SAHPI_LANG_ICELANDIC";
        case SAHPI_LANG_ITALIAN        : return "SAHPI_LANG_ITALIAN";
        case SAHPI_LANG_HEBREW         : return "SAHPI_LANG_HEBREW";
        case SAHPI_LANG_JAPANESE       : return "SAHPI_LANG_JAPANESE";
        case SAHPI_LANG_YIDDISH        : return "SAHPI_LANG_YIDDISH";
        case SAHPI_LANG_JAVANESE       : return "SAHPI_LANG_JAVANESE";
        case SAHPI_LANG_GEORGIAN       : return "SAHPI_LANG_GEORGIAN";
        case SAHPI_LANG_KAZAKH         : return "SAHPI_LANG_KAZAKH";
        case SAHPI_LANG_GREENLANDIC    : return "SAHPI_LANG_GREENLANDIC";
        case SAHPI_LANG_CAMBODIAN      : return "SAHPI_LANG_CAMBODIAN";
        case SAHPI_LANG_KANNADA        : return "SAHPI_LANG_KANNADA";
        case SAHPI_LANG_KOREAN         : return "SAHPI_LANG_KOREAN";
        case SAHPI_LANG_KASHMIRI       : return "SAHPI_LANG_KASHMIRI";
        case SAHPI_LANG_KURDISH        : return "SAHPI_LANG_KURDISH";
        case SAHPI_LANG_KIRGHIZ        : return "SAHPI_LANG_KIRGHIZ";
        case SAHPI_LANG_LATIN          : return "SAHPI_LANG_LATIN";
        case SAHPI_LANG_LINGALA        : return "SAHPI_LANG_LINGALA";
        case SAHPI_LANG_LAOTHIAN       : return "SAHPI_LANG_LAOTHIAN";
        case SAHPI_LANG_LITHUANIAN     : return "SAHPI_LANG_LITHUANIAN";
        case SAHPI_LANG_LATVIANLETTISH : return "SAHPI_LANG_LATVIANLETTISH";
        case SAHPI_LANG_MALAGASY       : return "SAHPI_LANG_MALAGASY";
        case SAHPI_LANG_MAORI          : return "SAHPI_LANG_MAORI";
        case SAHPI_LANG_MACEDONIAN     : return "SAHPI_LANG_MACEDONIAN";
        case SAHPI_LANG_MALAYALAM      : return "SAHPI_LANG_MALAYALM";
        case SAHPI_LANG_MONGOLIAN      : return "SAHPI_LANG_MONGOLIAN";
        case SAHPI_LANG_MOLDAVIAN      : return "SAHPI_LANG_MOLDAVIAN";
        case SAHPI_LANG_MARATHI        : return "SAHPI_LANG_MARATHI";
        case SAHPI_LANG_MALAY          : return "SAHPI_LANG_MALAY";
        case SAHPI_LANG_MALTESE        : return "SAHPI_LANG_MALTESE";
        case SAHPI_LANG_BURMESE        : return "SAHPI_LANG_BURMESE";
        case SAHPI_LANG_NAURU          : return "SAHPI_LANG_NAURU";
        case SAHPI_LANG_NEPALI         : return "SAHPI_LANG_NEPALI";
        case SAHPI_LANG_DUTCH          : return "SAHPI_LANG_DUTCH";
        case SAHPI_LANG_NORWEGIAN      : return "SAHPI_LANG_NORWEGIAN";
        case SAHPI_LANG_OCCITAN        : return "SAHPI_LANG_OCCITAN";
        case SAHPI_LANG_AFANOROMO      : return "SAHPI_LANG_AFANOROMO";
        case SAHPI_LANG_ORIYA          : return "SAHPI_LANG_ORIYA";
        case SAHPI_LANG_PUNJABI        : return "SAHPI_LANG_PUNJABI";
        case SAHPI_LANG_POLISH         : return "SAHPI_LANG_POLISH";
        case SAHPI_LANG_PASHTOPUSHTO   : return "SAHPI_LANG_PASHTOPUSHTO";
        case SAHPI_LANG_PORTUGUESE     : return "SAHPI_LANG_PORTUGUESE";
        case SAHPI_LANG_QUECHUA        : return "SAHPI_LANG_QUECHUA";
        case SAHPI_LANG_RHAETOROMANCE  : return "SAHPI_LANG_RHAETOROMANCE";
        case SAHPI_LANG_KIRUNDI        : return "SAHPI_LANG_KIRUNDI";
        case SAHPI_LANG_ROMANIAN       : return "SAHPI_LANG_ROMANIAN";
        case SAHPI_LANG_RUSSIAN        : return "SAHPI_LANG_RUSSIAN";
        case SAHPI_LANG_KINYARWANDA    : return "SAHPI_LANG_KINYARWANDA";
        case SAHPI_LANG_SANSKRIT       : return "SAHPI_LANG_SANSKRIT";
        case SAHPI_LANG_SINDHI         : return "SAHPI_LANG_SINDI";
        case SAHPI_LANG_SANGRO         : return "SAHPI_LANG_SANGRO";
        case SAHPI_LANG_SERBOCROATIAN  : return "SAHPI_LANG_SERBOCROATION";
        case SAHPI_LANG_SINGHALESE     : return "SAHPI_LANG_SINGHALESE";
        case SAHPI_LANG_SLOVAK         : return "SAHPI_LANG_SLOVAK";
        case SAHPI_LANG_SLOVENIAN      : return "SAHPI_LANG_SLOVENIAN";
        case SAHPI_LANG_SAMOAN         : return "SAHPI_LANG_SAMOAN";
        case SAHPI_LANG_SHONA          : return "SAHPI_LANG_SHONA";
        case SAHPI_LANG_SOMALI         : return "SAHPI_LANG_SOMALI";
        case SAHPI_LANG_ALBANIAN       : return "SAHPI_LANG_ALBANIAN";
        case SAHPI_LANG_SERBIAN        : return "SAHPI_LANG_SERBIAN";
        case SAHPI_LANG_SISWATI        : return "SAHPI_LANG_SISWATI";
        case SAHPI_LANG_SESOTHO        : return "SAHPI_LANG_SESOTHO";
        case SAHPI_LANG_SUDANESE       : return "SAHPI_LANG_SUDANESE";
        case SAHPI_LANG_SWEDISH        : return "SAHPI_LANG_SWEDISH";
        case SAHPI_LANG_SWAHILI        : return "SAHPI_LANG_SWAHILI";
        case SAHPI_LANG_TAMIL          : return "SAHPI_LANG_TAMIL";
        case SAHPI_LANG_TELUGU         : return "SAHPI_LANG_TELUGU";
        case SAHPI_LANG_TAJIK          : return "SAHPI_LANG_TAJIK";
        case SAHPI_LANG_THAI           : return "SAHPI_LANG_THAI";
        case SAHPI_LANG_TIGRINYA       : return "SAHPI_LANG_TIGRINYA";
        case SAHPI_LANG_TURKMEN        : return "SAHPI_LANG_TURKMEN";
        case SAHPI_LANG_TAGALOG        : return "SAHPI_LANG_TAGALOG";
        case SAHPI_LANG_SETSWANA       : return "SAHPI_LANG_SETSWANA";
        case SAHPI_LANG_TONGA          : return "SAHPI_LANG_TONGA";
        case SAHPI_LANG_TURKISH        : return "SAHPI_LANG_TURKISH";
        case SAHPI_LANG_TSONGA         : return "SAHPI_LANG_TSONGA";
        case SAHPI_LANG_TATAR          : return "SAHPI_LANG_TATAR";
        case SAHPI_LANG_TWI            : return "SAHPI_LANG_TWI";
        case SAHPI_LANG_UKRAINIAN      : return "SAHPI_LANG_UKRAINIAN";
        case SAHPI_LANG_URDU           : return "SAHPI_LANG_URDU";
        case SAHPI_LANG_UZBEK          : return "SAHPI_LANG_UZBEK";
        case SAHPI_LANG_VIETNAMESE     : return "SAHPI_LANG_VIETNAMESE";
        case SAHPI_LANG_VOLAPUK        : return "SAHPI_LANG_VOLAPUK";
        case SAHPI_LANG_WOLOF          : return "SAHPI_LANG_WOLOF";
        case SAHPI_LANG_XHOSA          : return "SAHPI_LANG_XHOSA";
        case SAHPI_LANG_YORUBA         : return "SAHPI_LANG_YORUBA";
        case SAHPI_LANG_CHINESE        : return "SAHPI_LANG_CHINESE";
        case SAHPI_LANG_ZULU           : return "SAHPI_LANG_ZULU";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN LANGUAGE [0x%X]", language);
    return buf;
}

/*****************************************************************************
 * Convert a Text Buffer to a string.
 *****************************************************************************/

const char *HpiString::textBuffer(SaHpiTextBufferT &buffer) {
    char tmpBuf[20];

    char *buf = StringBuffer::next();
    strcpy(buf, "[");
    strcat(buf, dataType(buffer.DataType));
    strcat(buf, ",");
    if (buffer.DataType == SAHPI_TL_TYPE_TEXT || buffer.DataType == SAHPI_TL_TYPE_UNICODE) {
        strcat(buf, language(buffer.Language));
        strcat(buf, ",");
    }
    sprintf(tmpBuf, "%d", buffer.DataLength);
    strcat(buf, tmpBuf);
    strcat(buf, ",{");
    if (buffer.DataLength > 0) {
        sprintf(tmpBuf, "%x", buffer.Data[0]);
        strcat(buf, tmpBuf);
        for (int i = 1; i < buffer.DataLength; i++) {
            strcat(buf, ",");
            sprintf(tmpBuf, "%x", buffer.Data[i]);
            strcat(buf, tmpBuf);
        }
    }
    strcat(buf, "}]");

    return buf;
}

/*****************************************************************************
 * Convert an Entity Type to a string.
 *****************************************************************************/

const char *HpiString::entityType(SaHpiEntityTypeT entityType) {

    switch (entityType) {
        case SAHPI_ENT_UNSPECIFIED              : return "SAHPI_ENT_UNSPECIFIED";
        case SAHPI_ENT_OTHER                    : return "SAHPI_ENT_OTHER";
        case SAHPI_ENT_UNKNOWN                  : return "SAHPI_ENT_UNKNOWN";
        case SAHPI_ENT_PROCESSOR                : return "SAHPI_ENT_PROCESSOR";
        case SAHPI_ENT_DISK_BAY                 : return "SAHPI_ENT_DISK_BAY";           
        case SAHPI_ENT_PERIPHERAL_BAY           : return "SAHPI_ENT_PERIPHERAL_BAY";
        case SAHPI_ENT_SYS_MGMNT_MODULE         : return "SAHPI_ENT_SYS_MGMNT_MODULE";
        case SAHPI_ENT_SYSTEM_BOARD             : return "SAHPI_ENT_SYSTEM_BOARD";
        case SAHPI_ENT_MEMORY_MODULE            : return "SAHPI_ENT_MEMORY_MODULE";
        case SAHPI_ENT_PROCESSOR_MODULE         : return "SAHPI_ENT_PROCESSOR_MODULE";
        case SAHPI_ENT_POWER_SUPPLY             : return "SAHPI_ENT_POWER_SUPPLY";
        case SAHPI_ENT_ADD_IN_CARD              : return "SAHPI_ENT_ADD_IN_CARD";
        case SAHPI_ENT_FRONT_PANEL_BOARD        : return "SAHPI_ENT_FRONT_PANEL_BOARD";
        case SAHPI_ENT_BACK_PANEL_BOARD         : return "SAHPI_ENT_BACK_PANEL_BOARD";
        case SAHPI_ENT_POWER_SYSTEM_BOARD       : return "SAHPI_ENT_POWER_SYSTEM_BOARD";
        case SAHPI_ENT_DRIVE_BACKPLANE          : return "SAHPI_ENT_DRIVE_BACKPLANE";
        case SAHPI_ENT_SYS_EXPANSION_BOARD      : return "SAHPI_ENT_SYS_EXPANSION_BOARD";
        case SAHPI_ENT_OTHER_SYSTEM_BOARD       : return "SAHPI_ENT_OTHER_SYSTEM_BOARD";
        case SAHPI_ENT_PROCESSOR_BOARD          : return "SAHPI_ENT_PROCESSOR_BOARD";
        case SAHPI_ENT_POWER_UNIT               : return "SAHPI_ENT_POWER_UNIT";
        case SAHPI_ENT_POWER_MODULE             : return "SAHPI_ENT_POWER_MODULE";
        case SAHPI_ENT_POWER_MGMNT              : return "SAHPI_ENT_POWER_MGMNT";
        case SAHPI_ENT_CHASSIS_BACK_PANEL_BOARD : return "SAHPI_ENT_CHASSIS_BACK_PANEL_BOARD";
        case SAHPI_ENT_SYSTEM_CHASSIS           : return "SAHPI_ENT_SYSTEM_CHASSIS";
        case SAHPI_ENT_SUB_CHASSIS              : return "SAHPI_ENT_SUB_CHASSIS";
        case SAHPI_ENT_OTHER_CHASSIS_BOARD      : return "SAHPI_ENT_OTHER_CHASSIS_BOARD";
        case SAHPI_ENT_DISK_DRIVE_BAY           : return "SAHPI_ENT_DISK_DRIVE_BAY";
        case SAHPI_ENT_PERIPHERAL_BAY_2         : return "SAHPI_ENT_PERIPHERAL_BAY_2";
        case SAHPI_ENT_DEVICE_BAY               : return "SAHPI_ENT_DEVICE_BAY";
        case SAHPI_ENT_COOLING_DEVICE           : return "SAHPI_ENT_COOLING_DEVICE";
        case SAHPI_ENT_COOLING_UNIT             : return "SAHPI_ENT_COOLING_UNIT";
        case SAHPI_ENT_INTERCONNECT             : return "SAHPI_ENT_INTERCONNECT";
        case SAHPI_ENT_MEMORY_DEVICE            : return "SAHPI_ENT_MEMORY_DEVICE";
        case SAHPI_ENT_SYS_MGMNT_SOFTWARE       : return "SAHPI_ENT_SYS_MGMNT_SOFTWARE";
        case SAHPI_ENT_BIOS                     : return "SAHPI_ENT_BIOS";
        case SAHPI_ENT_OPERATING_SYSTEM         : return "SAHPI_ENT_OPERATING_SYSTEM";
        case SAHPI_ENT_SYSTEM_BUS               : return "SAHPI_ENT_SYSTEM_BUS";
        case SAHPI_ENT_GROUP                    : return "SAHPI_ENT_GROUP";
        case SAHPI_ENT_REMOTE                   : return "SAHPI_ENT_REMOTE";
        case SAHPI_ENT_EXTERNAL_ENVIRONMENT     : return "SAHPI_ENT_EXTERNAL_ENVIRONMENT";
        case SAHPI_ENT_BATTERY                  : return "SAHPI_ENT_BATTERY";
	case SAHPI_ENT_RESERVED_1               : return "SAHPI_ENT_RESERVED_1";
        case SAHPI_ENT_RESERVED_2               : return "SAHPI_ENT_RESERVED_2";
        case SAHPI_ENT_RESERVED_3               : return "SAHPI_ENT_RESERVED_3";
        case SAHPI_ENT_RESERVED_4               : return "SAHPI_ENT_RESERVED_4";
        case SAHPI_ENT_RESERVED_5               : return "SAHPI_ENT_RESERVED_5";
        case SAHPI_ENT_MC_FIRMWARE              : return "SAHPI_ENT_MC_FIRMWARE";
        case SAHPI_ENT_IPMI_CHANNEL             : return "SAHPI_ENT_IPMI_CHANNEL";
        case SAHPI_ENT_PCI_BUS                  : return "SAHPI_ENT_PCI_BUS";
        case SAHPI_ENT_PCI_EXPRESS_BUS          : return "SAHPI_ENT_PCI_EXPRESS_BUS";
        case SAHPI_ENT_SCSI_BUS                 : return "SAHPI_ENT_SCSI_BUS";
        case SAHPI_ENT_SATA_BUS                 : return "SAHPI_ENT_SATA_BUS";
        case SAHPI_ENT_PROC_FSB                 : return "SAHPI_ENT_PROC_FSB";
        case SAHPI_ENT_CLOCK                    : return "SAHPI_ENT_CLOCK";
        case SAHPI_ENT_SYSTEM_FIRMWARE          : return "SAHPI_ENT_SYSTEM_FIRMWARE";
        case SAHPI_ENT_CHASSIS_SPECIFIC         : return "SAHPI_ENT_CHASSIS_SPECIFIC";
        case SAHPI_ENT_BOARD_SET_SPECIFIC       : return "SAHPI_ENT_BOARD_SET_SPECIFIC";
        case SAHPI_ENT_OEM_SYSINT_SPECIFIC      : return "SAHPI_ENT_OEM_SYSINT_SPECIFIC";
        case SAHPI_ENT_ROOT                     : return "SAHPI_ENT_ROOT";
        case SAHPI_ENT_RACK                     : return "SAHPI_ENT_RACK";
        case SAHPI_ENT_SUBRACK                  : return "SAHPI_ENT_SUBRACK";
        case SAHPI_ENT_COMPACTPCI_CHASSIS       : return "SAHPI_ENT_COMPACTPCI_CHASSIS";
        case SAHPI_ENT_ADVANCEDTCA_CHASSIS      : return "SAHPI_ENT_ADVANCEDTCA_CHASSIS";
        case SAHPI_ENT_RACK_MOUNTED_SERVER      : return "SAHPI_ENT_RACK_MOUNTED_SERVER";
        case SAHPI_ENT_SYSTEM_BLADE             : return "SAHPI_ENT_SYSTEM_BLADE";
        case SAHPI_ENT_SWITCH                   : return "SAHPI_ENT_SWITCH";
        case SAHPI_ENT_SWITCH_BLADE             : return "SAHPI_ENT_SWITCH_BLADE";
        case SAHPI_ENT_SBC_BLADE                : return "SAHPI_ENT_SBC_BLADE";
        case SAHPI_ENT_IO_BLADE                 : return "SAHPI_ENT_IO_BLADE";
        case SAHPI_ENT_DISK_BLADE               : return "SAHPI_ENT_DISK_BLADE";
        case SAHPI_ENT_DISK_DRIVE               : return "SAHPI_ENT_DISK_DRIVE";
        case SAHPI_ENT_FAN                      : return "SAHPI_ENT_FAN";
        case SAHPI_ENT_POWER_DISTRIBUTION_UNIT  : return "SAHPI_ENT_POWER_DISTRIBUTION_UNIT";
        case SAHPI_ENT_SPEC_PROC_BLADE          : return "SAHPI_ENT_SPEC_PROC_BLADE";
        case SAHPI_ENT_IO_SUBBOARD              : return "SAHPI_ENT_IO_SUBBOARD";
        case SAHPI_ENT_SBC_SUBBOARD             : return "SAHPI_ENT_SBC_SUBBOARD";
        case SAHPI_ENT_ALARM_MANAGER            : return "SAHPI_ENT_ALARM_MANAGER";
        case SAHPI_ENT_SHELF_MANAGER            : return "SAHPI_ENT_SHELF_MANAGER";
        case SAHPI_ENT_DISPLAY_PANEL            : return "SAHPI_ENT_DISPLAY_PANEL";
        case SAHPI_ENT_SUBBOARD_CARRIER_BLADE   : return "SAHPI_ENT_SUBBOARD_CARRIER_BLADE";
        case SAHPI_ENT_PHYSICAL_SLOT            : return "SAHPI_ENT_PHYSICAL_SLOT";
        case SAHPI_ENT_PICMG_FRONT_BLADE        : return "SAHPI_ENT_PICMG_FRONT_BLADE";
        case SAHPI_ENT_SYSTEM_INVENTORY_DEVICE  : return "SAHPI_ENT_SYSTEM_INVENTORY_DEVICE";
        case SAHPI_ENT_FILTRATION_UNIT          : return "SAHPI_ENT_FILTRATION_UNIT";
        case SAHPI_ENT_AMC                      : return "SAHPI_ENT_AMC";
        case SAHPI_ENT_BMC                      : return "SAHPI_ENT_BMC";
        case SAHPI_ENT_IPMC                     : return "SAHPI_ENT_IPMC";
        case SAHPI_ENT_MMC                      : return "SAHPI_ENT_MMC";
        case SAHPI_ENT_SHMC                     : return "SAHPI_ENT_SHMC";
        case SAHPI_ENT_CPLD                     : return "SAHPI_ENT_CPLD";
        case SAHPI_ENT_EPLD                     : return "SAHPI_ENT_EPLD";
        case SAHPI_ENT_FPGA                     : return "SAHPI_ENT_FPGA";
        case SAHPI_ENT_DASD                     : return "SAHPI_ENT_DASD";
        case SAHPI_ENT_NIC                      : return "SAHPI_ENT_NIC";
        case SAHPI_ENT_DSP                      : return "SAHPI_ENT_DSP";
        case SAHPI_ENT_UCODE                    : return "SAHPI_ENT_UCODE";
        case SAHPI_ENT_NPU                      : return "SAHPI_ENT_NPU";
        case SAHPI_ENT_OEM                      : return "SAHPI_ENT_OEM";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN ENTITY [0x%X]", entityType);
    return buf;
}

/*****************************************************************************
 * Convert an Entity Path to a string.
 *****************************************************************************/

const char *HpiString::entityPath(SaHpiEntityPathT *entityPath) {
    char tmpbuf[50];
    char *buf = StringBuffer::next();
    
    strcpy(buf, "{");
    for (int i = 0; i < SAHPI_MAX_ENTITY_PATH; i++) {
        strcat(buf, "{");
        strcat(buf, HpiString::entityType(entityPath->Entry[i].EntityType));
        sprintf(tmpbuf, ", %d", entityPath->Entry[i].EntityLocation);
        strcat(buf, tmpbuf);
        strcat(buf, "}");

        if (entityPath->Entry[i].EntityType == SAHPI_ENT_ROOT) {
            break;
        }
    }
    strcat(buf, "}");

    return buf;
}

/*****************************************************************************
 * Convert an Event Category to a string.
 *****************************************************************************/

const char *HpiString::eventCategory(SaHpiEventCategoryT category) {
    switch (category) {
        case SAHPI_EC_UNSPECIFIED     : return "SAHPI_EC_UNSPECIFIED";
        case SAHPI_EC_THRESHOLD       : return "SAHPI_EC_THRESHOLD";
        case SAHPI_EC_USAGE           : return "SAHPI_EC_USAGE";
        case SAHPI_EC_STATE           : return "SAHPI_EC_STATE";
        case SAHPI_EC_PRED_FAIL       : return "SAHPI_EC_PRED_FAIL";
        case SAHPI_EC_LIMIT           : return "SAHPI_EC_LIMIT";
        case SAHPI_EC_PERFORMANCE     : return "SAHPI_EC_PERFORMANCE";
        case SAHPI_EC_SEVERITY        : return "SAHPI_EC_SEVERITY";
        case SAHPI_EC_PRESENCE        : return "SAHPI_EC_PRESENCE";
        case SAHPI_EC_ENABLE          : return "SAHPI_EC_ENABLE";
        case SAHPI_EC_AVAILABILITY    : return "SAHPI_EC_AVAILABILITY";
        case SAHPI_EC_REDUNDANCY      : return "SAHPI_EC_REDUNDANCY";
        case SAHPI_EC_SENSOR_SPECIFIC : return "SAHPI_EC_SENSOR_SPECIFIC";
        case SAHPI_EC_GENERIC         : return "SAHPI_EC_GENERIC";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN EVENT CATEGORY [0x%X]", category);
    return buf;
}

/*****************************************************************************
 * Convert an Event Type to a string.
 *****************************************************************************/

const char *HpiString::eventType(SaHpiEventTypeT eventType) {
    switch (eventType) {
        case SAHPI_ET_RESOURCE             : return "SAHPI_ET_RESOURCE";
        case SAHPI_ET_DOMAIN               : return "SAHPI_ET_DOMAIN";
        case SAHPI_ET_SENSOR               : return "SAHPI_ET_SENSOR";
        case SAHPI_ET_SENSOR_ENABLE_CHANGE : return "SAHPI_ET_SENSOR_ENABLE_CHANGE";
        case SAHPI_ET_HOTSWAP              : return "SAHPI_ET_HOTSWAP";
        case SAHPI_ET_WATCHDOG             : return "SAHPI_ET_WATCHDOG";
        case SAHPI_ET_HPI_SW               : return "SAHPI_ET_HPI_SW";
        case SAHPI_ET_OEM                  : return "SAHPI_ET_OEM";
        case SAHPI_ET_USER                 : return "SAHPI_ET_USER";
        case SAHPI_ET_DIMI                 : return "SAHPI_ET_DIMI";
        case SAHPI_ET_DIMI_UPDATE          : return "SAHPI_ET_DIMI_UPDATE";
        case SAHPI_ET_FUMI                 : return "SAHPI_ET_FUMI";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN EVENT [0x%X]", eventType);
    return buf;
}

/*****************************************************************************
 * Convert a Severity to a string.
 *****************************************************************************/

const char *HpiString::severity(SaHpiSeverityT severity) {

    switch (severity) {
        case SAHPI_CRITICAL       : return "SAHPI_CRITICAL";
        case SAHPI_MAJOR          : return "SAHPI_MAJOR";
        case SAHPI_MINOR          : return "SAHPI_MINOR";
        case SAHPI_INFORMATIONAL  : return "SAHPI_INFORMATIONAL";
        case SAHPI_OK             : return "SAHPI_OK";
        case SAHPI_DEBUG          : return "SAHPI_DEBUG";
        case SAHPI_ALL_SEVERITIES : return "SAHPI_ALL_SEVERITIES";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN SEVERITY [0x%X]", severity);
    return buf;
}

/*****************************************************************************
 * Convert a Sensor Type to a string.
 *****************************************************************************/

const char *HpiString::sensorType(SaHpiSensorTypeT type) {

    switch (type) {
        case SAHPI_TEMPERATURE                 : return "SAHPI_TEMPERATURE";
        case SAHPI_VOLTAGE                     : return "SAHPI_VOLTAGE";
        case SAHPI_CURRENT                     : return "SAHPI_CURRENT";
        case SAHPI_FAN                         : return "SAHPI_FAN";
        case SAHPI_PHYSICAL_SECURITY           : return "SAHPI_PHYSICAL_SECURITY";
        case SAHPI_PLATFORM_VIOLATION          : return "SAHPI_PLATFORM_VIOLATION";
        case SAHPI_PROCESSOR                   : return "SAHPI_PROCESSSOR";
        case SAHPI_POWER_SUPPLY                : return "SAHPI_POWER_SUPPLY";
        case SAHPI_POWER_UNIT                  : return "SAHPI_POWER_UNIT";
        case SAHPI_COOLING_DEVICE              : return "SAHPI_COOLING_DEVICE";
        case SAHPI_OTHER_UNITS_BASED_SENSOR    : return "SAHPI_OTHER_UNITS_BASED_SENSOR";
        case SAHPI_MEMORY                      : return "SAHPI_MEMORY";
        case SAHPI_DRIVE_SLOT                  : return "SAHPI_DRIVE_SLOT";
        case SAHPI_POST_MEMORY_RESIZE          : return "SAHPI_POST_MEMORY_RESIZE";
        case SAHPI_SYSTEM_FW_PROGRESS          : return "SAHPI_SYSTEM_FW_PROGRESS";
        case SAHPI_EVENT_LOGGING_DISABLED      : return "SAHPI_EVENT_LOGGING_DISABLED";
        case SAHPI_RESERVED1                   : return "SAHPI_RESERVED1";
        case SAHPI_SYSTEM_EVENT                : return "SAHPI_SYSTEM_EVENT";
        case SAHPI_CRITICAL_INTERRUPT          : return "SAHPI_CRITICAL_INTERRUPT";
        case SAHPI_BUTTON                      : return "SAHPI_BUTTON";
        case SAHPI_MODULE_BOARD                : return "SAHPI_MODULE_BOARD";
        case SAHPI_MICROCONTROLLER_COPROCESSOR : return "SAHPI_MICROCONTROLLER_COPROCESSOR";
        case SAHPI_ADDIN_CARD                  : return "SAHPI_ADDIN_CARD";
        case SAHPI_CHASSIS                     : return "SAHPI_CHASSIS";
        case SAHPI_CHIP_SET                    : return "SAHPI_CHIP_SET";
        case SAHPI_OTHER_FRU                   : return "SAHPI_OTHER_FRU";
        case SAHPI_CABLE_INTERCONNECT          : return "SAHPI_CABLE_INTERCONNECT";
        case SAHPI_TERMINATOR                  : return "SAHPI_TERMINATOR";
        case SAHPI_SYSTEM_BOOT_INITIATED       : return "SAHPI_SYSTEM_BOOT_INITIATED";
        case SAHPI_BOOT_ERROR                  : return "SAHPI_BOOT_ERROR";
        case SAHPI_OS_BOOT                     : return "SAHPI_OS_BOOT";
        case SAHPI_OS_CRITICAL_STOP            : return "SAHPI_OS_CRITICAL_STOP";
        case SAHPI_SLOT_CONNECTOR              : return "SAHPI_SLOT_CONNECTOR";
        case SAHPI_SYSTEM_ACPI_POWER_STATE     : return "SAHPI_SYSTEM_ACPI_POWER_STATE";
        case SAHPI_RESERVED2                   : return "SAHPI_RESERVED2";
        case SAHPI_PLATFORM_ALERT              : return "SAHPI_PLATFORM_ALERT";
        case SAHPI_ENTITY_PRESENCE             : return "SAHPI_ENTITY_PRESENCE";
        case SAHPI_MONITOR_ASIC_IC             : return "SAHPI_MONITOR_ASIC_IC";
        case SAHPI_LAN                         : return "SAHPI_LAN";
        case SAHPI_MANAGEMENT_SUBSYSTEM_HEALTH : return "SAHPI_MANAGEMENT_SUBSYSTEM_HEALTH";
        case SAHPI_BATTERY                     : return "SAHPI_BATTERY";
        case SAHPI_SESSION_AUDIT               : return "SAHPI_SESSION_AUDIT";
        case SAHPI_VERSION_CHANGE              : return "SAHPI_VERSION_CHANGE";
        case SAHPI_OPERATIONAL                 : return "SAHPI_OPERATIONAL";
        case SAHPI_OEM_SENSOR                  : return "SAHPI_OEM_SENSOR";
        case SAHPI_COMM_CHANNEL_LINK_STATE     : return "SAHPI_COMM_CHANNEL_LINK_STATE";
        case SAHPI_MANAGEMENT_BUS_STATE        : return "SAHPI_MANAGEMENT_BUS_STATE";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN SENSOR TYPE [0x%X]", type);
    return buf;
}

/*****************************************************************************
 * Convert a Sensor Reading Type to a string.
 *****************************************************************************/

const char *HpiString::sensorReadingType(SaHpiSensorReadingTypeT readingType) {

    switch (readingType) {
        case SAHPI_SENSOR_READING_TYPE_INT64   : return "SAHPI_SENSOR_READING_TYPE_INT64";
        case SAHPI_SENSOR_READING_TYPE_UINT64  : return "SAHPI_SENSOR_READING_TYPE_UINT64";
        case SAHPI_SENSOR_READING_TYPE_FLOAT64 : return "SAHPI_SENSOR_READING_TYPE_FLOAT64";
        case SAHPI_SENSOR_READING_TYPE_BUFFER  : return "SAHPI_SENSOR_READING_TYPE_BUFFER";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN SENSOR READING TYPE [0x%X]", readingType);
    return buf;
}

/*****************************************************************************
 * Convert a Sensor Units to a string.
 *****************************************************************************/

const char *HpiString::sensorUnits(SaHpiSensorUnitsT units) {
    switch (units) {
        case SAHPI_SU_UNSPECIFIED          : return "SAHPI_SU_UNSPECIFIED";
        case SAHPI_SU_DEGREES_C            : return "SAHPI_SU_DEGREES_C";
        case SAHPI_SU_DEGREES_F            : return "SAHPI_SU_DEGREES_F";
        case SAHPI_SU_DEGREES_K            : return "SAHPI_SU_DEGREES_K";
        case SAHPI_SU_VOLTS                : return "SAHPI_SU_VOLTS";
        case SAHPI_SU_AMPS                 : return "SAHPI_SU_AMPS";
        case SAHPI_SU_WATTS                : return "SAHPI_SU_WATTS";
        case SAHPI_SU_JOULES               : return "SAHPI_SU_JOULES";
        case SAHPI_SU_COULOMBS             : return "SAHPI_SU_COULOMBS";
        case SAHPI_SU_VA                   : return "SAHPI_SU_VA";
        case SAHPI_SU_NITS                 : return "SAHPI_SU_NITS";
        case SAHPI_SU_LUMEN                : return "SAHPI_SU_LUMEN";
        case SAHPI_SU_LUX                  : return "SAHPI_SU_LUX";
        case SAHPI_SU_CANDELA              : return "SAHPI_SU_CANDELA";
        case SAHPI_SU_KPA                  : return "SAHPI_SU_KPA";
        case SAHPI_SU_PSI                  : return "SAHPI_SU_PSI";
        case SAHPI_SU_NEWTON               : return "SAHPI_SU_NEWTON";
        case SAHPI_SU_CFM                  : return "SAHPI_SU_CFM";
        case SAHPI_SU_RPM                  : return "SAHPI_SU_RPM";
        case SAHPI_SU_HZ                   : return "SAHPI_SU_HZ";
        case SAHPI_SU_MICROSECOND          : return "SAHPI_SU_MICROSECOND";
        case SAHPI_SU_MILLISECOND          : return "SAHPI_SU_MILLISECOND";
        case SAHPI_SU_SECOND               : return "SAHPI_SU_SECOND";
        case SAHPI_SU_MINUTE               : return "SAHPI_SU_MINUTE";
        case SAHPI_SU_HOUR                 : return "SAHPI_SU_HOUR";
        case SAHPI_SU_DAY                  : return "SAHPI_SU_DAY";
        case SAHPI_SU_WEEK                 : return "SAHPI_SU_WEEK";
        case SAHPI_SU_MIL                  : return "SAHPI_SU_MIL";
        case SAHPI_SU_INCHES               : return "SAHPI_SU_INCHES";
        case SAHPI_SU_FEET                 : return "SAHPI_SU_FEET";
        case SAHPI_SU_CU_IN                : return "SAHPI_SU_CU_IN";
        case SAHPI_SU_CU_FEET              : return "SAHPI_SU_CU_FEET";
        case SAHPI_SU_MM                   : return "SAHPI_SU_MM";
        case SAHPI_SU_CM                   : return "SAHPI_SU_CM";
        case SAHPI_SU_M                    : return "SAHPI_SU_M";
        case SAHPI_SU_CU_CM                : return "SAHPI_SU_CU_CM";
        case SAHPI_SU_CU_M                 : return "SAHPI_SU_CU_M";
        case SAHPI_SU_LITERS               : return "SAHPI_SU_LITERS";
        case SAHPI_SU_FLUID_OUNCE          : return "SAHPI_SU_FLUID_OUNCE";
        case SAHPI_SU_RADIANS              : return "SAHPI_SU_RADIANS";
        case SAHPI_SU_STERADIANS           : return "SAHPI_SU_STERADIANS";
        case SAHPI_SU_REVOLUTIONS          : return "SAHPI_SU_REVOLUTIONS";
        case SAHPI_SU_CYCLES               : return "SAHPI_SU_CYCLES";
        case SAHPI_SU_GRAVITIES            : return "SAHPI_SU_GRAVITIES";
        case SAHPI_SU_OUNCE                : return "SAHPI_SU_OUNCE";
        case SAHPI_SU_POUND                : return "SAHPI_SU_POUND";
        case SAHPI_SU_FT_LB                : return "SAHPI_SU_FT_LB";
        case SAHPI_SU_OZ_IN                : return "SAHPI_SU_OZ_IN";
        case SAHPI_SU_GAUSS                : return "SAHPI_SU_GAUSS";
        case SAHPI_SU_GILBERTS             : return "SAHPI_SU_GILBERTS";
        case SAHPI_SU_HENRY                : return "SAHPI_SU_HENRY";
        case SAHPI_SU_MILLIHENRY           : return "SAHPI_SU_MILLIHENRY";
        case SAHPI_SU_FARAD                : return "SAHPI_SU_FARAD";
        case SAHPI_SU_MICROFARAD           : return "SAHPI_SU_MICROFARAD";
        case SAHPI_SU_OHMS                 : return "SAHPI_SU_OHMS";
        case SAHPI_SU_SIEMENS              : return "SAHPI_SU_SIEMENS";
        case SAHPI_SU_MOLE                 : return "SAHPI_SU_MOLE";
        case SAHPI_SU_BECQUEREL            : return "SAHPI_SU_BECQUEREL";
        case SAHPI_SU_PPM                  : return "SAHPI_SU_PPM";
        case SAHPI_SU_RESERVED             : return "SAHPI_SU_RESERVED";
        case SAHPI_SU_DECIBELS             : return "SAHPI_SU_DECIBELS";
        case SAHPI_SU_DBA                  : return "SAHPI_SU_DBA";
        case SAHPI_SU_DBC                  : return "SAHPI_SU_DBC";
        case SAHPI_SU_GRAY                 : return "SAHPI_SU_GRAY";
        case SAHPI_SU_SIEVERT              : return "SAHPI_SU_SIEVERT";
        case SAHPI_SU_COLOR_TEMP_DEG_K     : return "SAHPI_SU_COLOR_TEMP_DEG_K";
        case SAHPI_SU_BIT                  : return "SAHPI_SU_BIT";
        case SAHPI_SU_KILOBIT              : return "SAHPI_SU_KILOBIT";
        case SAHPI_SU_MEGABIT              : return "SAHPI_SU_MEGABIT";
        case SAHPI_SU_GIGABIT              : return "SAHPI_SU_GIGABIT";
        case SAHPI_SU_BYTE                 : return "SAHPI_SU_BYTE";
        case SAHPI_SU_KILOBYTE             : return "SAHPI_SU_KILOBYTE";
        case SAHPI_SU_MEGABYTE             : return "SAHPI_SU_MEGABYTE";
        case SAHPI_SU_GIGABYTE             : return "SAHPI_SU_GIGABYTE";
        case SAHPI_SU_WORD                 : return "SAHPI_SU_WORD";
        case SAHPI_SU_DWORD                : return "SAHPI_SU_DWORD";
        case SAHPI_SU_QWORD                : return "SAHPI_SU_QWORD";
        case SAHPI_SU_LINE                 : return "SAHPI_SU_LINE";
        case SAHPI_SU_HIT                  : return "SAHPI_SU_HIT";
        case SAHPI_SU_MISS                 : return "SAHPI_SU_MISS";
        case SAHPI_SU_RETRY                : return "SAHPI_SU_RETRY";
        case SAHPI_SU_RESET                : return "SAHPI_SU_RESET";
        case SAHPI_SU_OVERRUN              : return "SAHPI_SU_OVERRUN";
        case SAHPI_SU_UNDERRUN             : return "SAHPI_SU_UNDERRUN";
        case SAHPI_SU_COLLISION            : return "SAHPI_SU_COLLISION";
        case SAHPI_SU_PACKETS              : return "SAHPI_SU_PACKETS";
        case SAHPI_SU_MESSAGES             : return "SAHPI_SU_MESSAGES";
        case SAHPI_SU_CHARACTERS           : return "SAHPI_SU_CHARACTERS";
        case SAHPI_SU_ERRORS               : return "SAHPI_SU_ERRORS";
        case SAHPI_SU_CORRECTABLE_ERRORS   : return "SAHPI_SU_CORRECTABLE_ERRORS";
        case SAHPI_SU_UNCORRECTABLE_ERRORS : return "SAHPI_SU_UNCORRECTABLE_ERRORS";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN UNITS [0x%X]", units);
    return buf;
}

/*****************************************************************************
 * Convert a Sensor Modifier Units Use to a string.
 *****************************************************************************/

const char *HpiString::sensorModUnitUse(SaHpiSensorModUnitUseT modifier) {

    switch (modifier) {
        case SAHPI_SMUU_NONE                 : return "SAHPI_SMUU_NONE";
        case SAHPI_SMUU_BASIC_OVER_MODIFIER  : return "SAHPI_SMUU_BASIC_OVER_MODIFIER";
        case SAHPI_SMUU_BASIC_TIMES_MODIFIER : return "SAHPI_SMUU_BASIC_TIMES_MODIFIER";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN UNITS MODIFIER [0x%X]", modifier);
    return buf;
}

/*****************************************************************************
 * Convert a Sensor EventCtrl to a string.
 *****************************************************************************/

const char *HpiString::sensorEventCtrl(SaHpiSensorEventCtrlT eventCtrl) {

    switch (eventCtrl) {
        case SAHPI_SEC_PER_EVENT       : return "SAHPI_SEC_PER_EVENT";
        case SAHPI_SEC_READ_ONLY_MASKS : return "SAHPI_SEC_READ_ONLY_MASKS";
        case SAHPI_SEC_READ_ONLY       : return "SAHPI_SEC_READ_ONLY";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN EVENT CTRL [0x%X]", eventCtrl);
    return buf;
}

/*****************************************************************************
 * Convert a Ctrl Type to a string.
 *****************************************************************************/

const char *HpiString::ctrlType(SaHpiCtrlTypeT ctrlType) {

    switch (ctrlType) {
        case SAHPI_CTRL_TYPE_DIGITAL   : return "SAHPI_CTRL_TYPE_DIGITAL";
        case SAHPI_CTRL_TYPE_DISCRETE  : return "SAHPI_CTRL_TYPE_DISCRETE";
        case SAHPI_CTRL_TYPE_ANALOG    : return "SAHPI_CTRL_TYPE_ANALOG";
        case SAHPI_CTRL_TYPE_STREAM    : return "SAHPI_CTRL_TYPE_STREAM";
        case SAHPI_CTRL_TYPE_TEXT      : return "SAHPI_CTRL_TYPE_TEXT";
        case SAHPI_CTRL_TYPE_OEM       : return "SAHPI_CTRL_TYPE_OEM";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN CTRL TYPE [0x%X]", ctrlType);
    return buf;
}

/*****************************************************************************
 * Convert a Ctrl State Digital to a string.
 *****************************************************************************/

const char *HpiString::ctrlStateDigital(SaHpiCtrlStateDigitalT state) {

    switch (state) {
        case SAHPI_CTRL_STATE_OFF       : return "SAHPI_CTRL_STATE_OFF";
        case SAHPI_CTRL_STATE_ON        : return "SAHPI_CTRL_STATE_ON";
        case SAHPI_CTRL_STATE_PULSE_OFF : return "SAHPI_CTRL_STATE_PULSE_OFF";
        case SAHPI_CTRL_STATE_PULSE_ON  : return "SAHPI_CTRL_STATE_PULSE_ON";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN CTRL STATE DIGITAL [0x%X]", state);
    return buf;
}

/*****************************************************************************
 * Convert an Control Mode to a string.
 *****************************************************************************/

const char *HpiString::ctrlMode(SaHpiCtrlModeT ctrlMode) {
    switch (ctrlMode) {
        case SAHPI_CTRL_MODE_AUTO   : return "SAHPI_CTRL_MODE_AUTO";
        case SAHPI_CTRL_MODE_MANUAL : return "SAHPI_CTRL_MODE_MANUAL";
    } 

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN CTRL MODE [0x%X]", ctrlMode);
    return buf;
}

/*****************************************************************************
 * Convert an Control Output Type to a string.
 *****************************************************************************/

const char *HpiString::ctrlOutputType(SaHpiCtrlOutputTypeT ctrlOutputType) {
    switch (ctrlOutputType) {
        case SAHPI_CTRL_GENERIC              : return "SAHPI_CTRL_GENERIC";
        case SAHPI_CTRL_LED                  : return "SAHPI_CTRL_LED";
        case SAHPI_CTRL_FAN_SPEED            : return "SAHPI_CTRL_FAN_SPEED";
        case SAHPI_CTRL_DRY_CONTACT_CLOSURE  : return "SAHPI_CTRL_DRY_CONTACT_CLOSURE";
        case SAHPI_CTRL_POWER_SUPPLY_INHIBIT : return "SAHPI_CTRL_POWER_SUPPLY_INHIBIT";
        case SAHPI_CTRL_AUDIBLE              : return "SAHPI_CTRL_AUDIBLE";
        case SAHPI_CTRL_FRONT_PANEL_LOCKOUT  : return "SAHPI_CTRL_FRONT_PANEL_LOCKOUT";
        case SAHPI_CTRL_POWER_INTERLOCK      : return "SAHPI_CTRL_POWER_INTERLOCK";
        case SAHPI_CTRL_POWER_STATE          : return "SAHPI_CTRL_POWER_STATE";
        case SAHPI_CTRL_LCD_DISPLAY          : return "SAHPI_CTRL_LCD_DISPLAY";
        case SAHPI_CTRL_OEM                  : return "SAHPI_CTRL_OEM";
    } 

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN CTRL OUTPUT TYPE [0x%X]", ctrlOutputType);
    return buf;
}

/*****************************************************************************
 * Convert an IDR Area Type to a string.
 *****************************************************************************/

const char *HpiString::idrAreaType(SaHpiIdrAreaTypeT areaType) {

    switch (areaType) {
        case SAHPI_IDR_AREATYPE_INTERNAL_USE   : return "SAHPI_IDR_AREATYPE_INTERNAL_USE";
        case SAHPI_IDR_AREATYPE_CHASSIS_INFO   : return "SAHPI_IDR_AREATYPE_CHASSIS_INFO";
        case SAHPI_IDR_AREATYPE_BOARD_INFO     : return "SAHPI_IDR_AREATYPE_BOARD_INFO";
        case SAHPI_IDR_AREATYPE_PRODUCT_INFO   : return "SAHPI_IDR_AREATYPE_PRODUCT_INFO";
        case SAHPI_IDR_AREATYPE_OEM            : return "SAHPI_IDR_AREATYPE_OEM";
        case SAHPI_IDR_AREATYPE_UNSPECIFIED    : return "SAHPI_IDR_AREATYPE_UNSPECIFIED";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN AREA TYPE [0x%X]", areaType);
    return buf;
}

/*****************************************************************************
 * Convert an IDR Field Type to a string.
 *****************************************************************************/

const char *HpiString::idrFieldType(SaHpiIdrFieldTypeT fieldType) {

    switch (fieldType) {
        case SAHPI_IDR_FIELDTYPE_CHASSIS_TYPE    : return "SAHPI_IDR_FIELDTYPE_CHASSIS_TYPE";
        case SAHPI_IDR_FIELDTYPE_MFG_DATETIME    : return "SAHPI_IDR_FIELDTYPE_MFG_DATETIME";
        case SAHPI_IDR_FIELDTYPE_MANUFACTURER    : return "SAHPI_IDR_FIELDTYPE_MANUFACTURER";
        case SAHPI_IDR_FIELDTYPE_PRODUCT_NAME    : return "SAHPI_IDR_FIELDTYPE_PRODUCT_NAME";
        case SAHPI_IDR_FIELDTYPE_PRODUCT_VERSION : return "SAHPI_IDR_FIELDTYPE_PRODUCT_VERSION";
        case SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER   : return "SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER";
        case SAHPI_IDR_FIELDTYPE_PART_NUMBER     : return "SAHPI_IDR_FIELDTYPE_PART_NUMBER";
        case SAHPI_IDR_FIELDTYPE_FILE_ID         : return "SAHPI_IDR_FIELDTYPE_FILE_ID";
        case SAHPI_IDR_FIELDTYPE_ASSET_TAG       : return "SAHPI_IDR_FIELDTYPE_ASSET_TAG";
        case SAHPI_IDR_FIELDTYPE_CUSTOM          : return "SAHPI_IDR_FIELDTYPE_CUSTOM";
        case SAHPI_IDR_FIELDTYPE_UNSPECIFIED     : return "SAHPI_IDR_FIELDTYPE_UNSPECIFIED";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN FIELD TYPE [0x%X]", fieldType);
    return buf;
}

/*****************************************************************************
 * Convert a Watchdog Action to a string.
 *****************************************************************************/

const char *HpiString::watchdogAction(SaHpiWatchdogActionT action) {
    switch (action) {
        case SAHPI_WA_NO_ACTION   : return "SAHPI_WA_NO_ACTION";
        case SAHPI_WA_RESET       : return "SAHPI_WA_RESET";
        case SAHPI_WA_POWER_DOWN  : return "SAHPI_WA_POWER_DOWN";
        case SAHPI_WA_POWER_CYCLE : return "SAHPI_WA_POWER_CYCLE";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN WATCHDOG ACTION [0x%X]", action);
    return buf;
}

/*****************************************************************************
 * Convert a Watchdog Action Event to a string.
 *****************************************************************************/

const char *HpiString::watchdogActionEvent(SaHpiWatchdogActionEventT actionEvent) {
    switch (actionEvent) {
        case SAHPI_WAE_NO_ACTION    : return "SAHPI_WAI_NO_ACTION";
        case SAHPI_WAE_RESET        : return "SAHPI_WAI_RESET";
        case SAHPI_WAE_POWER_DOWN   : return "SAHPI_WAI_POWER_DOWN";
        case SAHPI_WAE_POWER_CYCLE  : return "SAHPI_WAI_POWER_CYCLE";
        case SAHPI_WAE_TIMER_INT    : return "SAHPI_WAI_TIMER_INT";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN WATCHDOG ACTION EVENT [0x%X]", actionEvent);
    return buf;
}

/*****************************************************************************
 * Convert a Watchdog Pretimer Interrupt to a string.
 *****************************************************************************/

const char *HpiString::watchdogPretimerInterrupt(SaHpiWatchdogPretimerInterruptT pretimerInterrupt) {
    switch (pretimerInterrupt) {
        case SAHPI_WPI_NONE              : return "SAHPI_WPI_NONE";
        case SAHPI_WPI_SMI               : return "SAHPI_WPI_SMI";
        case SAHPI_WPI_NMI               : return "SAHPI_WPI_NMI";
        case SAHPI_WPI_MESSAGE_INTERRUPT : return "SAHPI_WPI_MESSAGE_INTERRUPT";
        case SAHPI_WPI_OEM               : return "SAHPI_WPI_OEM";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN WATCHDOG PRETIMER INTERRUPT [0x%X]", pretimerInterrupt);
    return buf;
}

/*****************************************************************************
 * Convert a Watchdog Timer Use to a string.
 *****************************************************************************/

const char *HpiString::watchdogTimerUse(SaHpiWatchdogTimerUseT timerUse) {
    switch (timerUse) {
        case SAHPI_WTU_NONE        : return "SAHPI_WTU_NONE";
        case SAHPI_WTU_BIOS_FRB2   : return "SAHPI_WTU_BIOS_FRB2";
        case SAHPI_WTU_BIOS_POST   : return "SAHPI_WTU_BIOS_POST";
        case SAHPI_WTU_OS_LOAD     : return "SAHPI_WTU_OS_LOAD";
        case SAHPI_WTU_SMS_OS      : return "SAHPI_WTU_SMS_OS";
        case SAHPI_WTU_OEM         : return "SAHPI_WTU_OEM";
        case SAHPI_WTU_UNSPECIFIED : return "SAHPI_WTU_UNSPECIFIED";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN WATCHDOG TIMER USE [0x%X]", timerUse);
    return buf;
}

/*****************************************************************************
 * Convert a Watchdog TimerUse Exp Flag to a string.
 *****************************************************************************/

const char *HpiString::watchdogTimerUseExpFlag(SaHpiWatchdogExpFlagsT flag) {

  switch ( flag ) {
      case SAHPI_WATCHDOG_EXP_BIOS_FRB2 : return "SAHPI_WATCHDOG_EXP_BIOS_FRB2";
      case SAHPI_WATCHDOG_EXP_BIOS_POST : return "SAHPI_WATCHDOG_EXP_BIOS_POST";
      case SAHPI_WATCHDOG_EXP_OS_LOAD   : return "SAHPI_WATCHDOG_EXP_OS_LOAD";
      case SAHPI_WATCHDOG_EXP_SMS_OS    : return "SAHPI_WATCHDOG_EXP_SMS_OS";
      case SAHPI_WATCHDOG_EXP_OEM       : return "SAHPI_WATCHDOG_EXP_OEM";
  }
  char *buf = StringBuffer::next();
  sprintf(buf, "UNKNOWN WATCHDOG TimerUseExpFlag [0x%X]", flag);
  return buf;
}


/*****************************************************************************
 * Convert a HotSwap Indicator State to a string.
 *****************************************************************************/

const char *HpiString::hotSwapIndicatorState(SaHpiHsIndicatorStateT state) {
    switch (state) {
        case SAHPI_HS_INDICATOR_OFF : return "SAHPI_HS_INDICATOR_OFF";
        case SAHPI_HS_INDICATOR_ON  : return "SAHPI_HS_INDICATOR_ON";
    } 

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN HOTSWAP INDICATOR [0x%X]", state);
    return buf;
}

/*****************************************************************************
 * Convert a HotSwap Action to a string.
 *****************************************************************************/

const char *HpiString::hotSwapAction(SaHpiHsActionT action) {
    switch (action) {
        case SAHPI_HS_ACTION_INSERTION  : return "SAHPI_HS_ACTION_INSERTION";
        case SAHPI_HS_ACTION_EXTRACTION : return "SAHPI_HS_ACTION_EXTRACTION";
    } 

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN HOTSWAP ACTION [0x%X]", action);
    return buf;
}

/*****************************************************************************
 * Convert a HotSwap State to a string.
 *****************************************************************************/

const char *HpiString::hotSwapState(SaHpiHsStateT state) {
    switch (state) {
        case SAHPI_HS_STATE_INACTIVE           : return "SAHPI_HS_STATE_INACTIVE";
        case SAHPI_HS_STATE_INSERTION_PENDING  : return "SAHPI_HS_STATE_INSERTION_PENDING";
        case SAHPI_HS_STATE_ACTIVE             : return "SAHPI_HS_STATE_ACTIVE";
        case SAHPI_HS_STATE_EXTRACTION_PENDING : return "SAHPI_HS_STATE_EXTRACTION_PENDING";
        case SAHPI_HS_STATE_NOT_PRESENT        : return "SAHPI_HS_STATE_NOT_PRESENT";
    } 

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN HOTSWAP STATE [0x%X]", state);
    return buf;
}

/*****************************************************************************
 * Convert a Name to a string.
 *****************************************************************************/

const char *HpiString::name(SaHpiNameT *name) {
    char *buf = StringBuffer::next();

    strcpy(buf, "(");
    for (int i = 0; i < name->Length; i++) {
        char tmp[2];
        tmp[0] = name->Value[i];
        tmp[1] = '\0';
        strcat(buf, tmp);
    }
    strcat(buf, ")");

    return buf;
}

/*****************************************************************************
 * Convert a Status Condition Type to a string.
 *****************************************************************************/

const char *HpiString::statusCondType(SaHpiStatusCondTypeT condType) {
    switch (condType) {
        case SAHPI_STATUS_COND_TYPE_SENSOR   : return "SAHPI_STATUS_COND_TYPE_SENSOR";
        case SAHPI_STATUS_COND_TYPE_RESOURCE : return "SAHPI_STATUS_COND_TYPE_RESOURCE";
        case SAHPI_STATUS_COND_TYPE_OEM      : return "SAHPI_STATUS_COND_TYPE_OEM";
        case SAHPI_STATUS_COND_TYPE_USER     : return "SAHPI_STATUS_COND_TYPE_USER";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN STATUS COND [0x%X]", condType);
    return buf;
}

/*****************************************************************************
 * Convert an Annunciator Mode to a string.
 *****************************************************************************/

const char *HpiString::annunciatorMode(SaHpiAnnunciatorModeT annunciatorMode) {
    switch (annunciatorMode) {
        case SAHPI_ANNUNCIATOR_MODE_AUTO   : return "SAHPI_ANNUNCIATOR_MODE_AUTO";
        case SAHPI_ANNUNCIATOR_MODE_USER   : return "SAHPI_ANNUNCIATOR_MODE_USER";
        case SAHPI_ANNUNCIATOR_MODE_SHARED : return "SAHPI_ANNUNCIATOR_MODE_SHARED";
    } 

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN ANNUNCIATOR MODE [0x%X]", annunciatorMode);
    return buf;
}

/*****************************************************************************
 * Convert an AnnunciatorType to a string.
 *****************************************************************************/

const char *HpiString::annunciatorType(SaHpiAnnunciatorTypeT annunciatorType) {
    switch (annunciatorType) {
        case SAHPI_ANNUNCIATOR_TYPE_LED         : return "SAHPI_ANNUNCIATOR_TYPE_LED";
        case SAHPI_ANNUNCIATOR_TYPE_DRY_CONTACT_CLOSURE : 
                                                  return "SAHPI_ANNUNCIATOR_TYPE_DRY_CONTACT_CLOSURE";
        case SAHPI_ANNUNCIATOR_TYPE_AUDIBLE     : return "SAHPI_ANNUNCIATOR_TYPE_AUDIBLE";
        case SAHPI_ANNUNCIATOR_TYPE_LCD_DISPLAY : return "SAHPI_ANNUNCIATOR_TYPE_LCD_DISPLAY";
        case SAHPI_ANNUNCIATOR_TYPE_MESSAGE     : return "SAHPI_ANNUNCIATOR_TYPE_MESSAGE";
        case SAHPI_ANNUNCIATOR_TYPE_COMPOSITE   : return "SAHPI_ANNUNCIATOR_TYPE_COMPOSITE";
        case SAHPI_ANNUNCIATOR_TYPE_OEM         : return "SAHPI_ANNUNCIATOR_TYPE_OEM";
    } 

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN ANNUNCIATOR TYPE [0x%X]", annunciatorType);
    return buf;
}

/*****************************************************************************
 * Convert an RDR Type to a string.
 *****************************************************************************/

const char *HpiString::rdrType(SaHpiRdrTypeT rdrType) {

    switch (rdrType) {
        case SAHPI_NO_RECORD       : return "SAHPI_NO_RECORD";
        case SAHPI_CTRL_RDR        : return "SAHPI_CTRL_RDR";
        case SAHPI_SENSOR_RDR      : return "SAHPI_SENSOR_RDR";
        case SAHPI_INVENTORY_RDR   : return "SAHPI_INVENTORY_RDR";
        case SAHPI_WATCHDOG_RDR    : return "SAHPI_WATCHDOG_RDR";
        case SAHPI_ANNUNCIATOR_RDR : return "SAHPI_ANNUNCIATOR_RDR";
        case SAHPI_DIMI_RDR        : return "SAHPI_DIMI_RDR";
        case SAHPI_FUMI_RDR        : return "SAHPI_FUMI_RDR";
    }

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN RDR [0x%X]", rdrType);
    return buf;
}

/*****************************************************************************
 * Convert a Reset Action to a string.
 *****************************************************************************/

const char *HpiString::resetAction(SaHpiResetActionT action) {
    switch (action) {
        case SAHPI_COLD_RESET     : return "SAHPI_COLD_RESET";
        case SAHPI_WARM_RESET     : return "SAHPI_WARM_RESET";
        case SAHPI_RESET_ASSERT   : return "SAHPI_RESET_ASSERT";
        case SAHPI_RESET_DEASSERT : return "SAHPI_RESET_DEASSERT";
    } 

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN RESET ACTION [0x%X]", action);
    return buf;
}

/*****************************************************************************
 * Convert a Power State to a string.
 *****************************************************************************/

const char *HpiString::powerState(SaHpiPowerStateT powerState) {
    switch (powerState) {
        case SAHPI_POWER_OFF   : return "SAHPI_POWER_OFF";
        case SAHPI_POWER_ON    : return "SAHPI_POWER_ON";
        case SAHPI_POWER_CYCLE : return "SAHPI_POWER_CYCLE";
    } 

    char *buf = StringBuffer::next();
    sprintf(buf, "UNKNOWN POWER STATE [0x%X]", powerState);
    return buf;
}

