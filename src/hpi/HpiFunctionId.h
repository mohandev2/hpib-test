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

#ifndef __HPIFUNCTIONID_H__
#define __HPIFUNCTIONID_H__

/*****************************************************************************
 * Enumerated list of all HPI functions.
 *****************************************************************************/

typedef enum 
{
    VERSION_GET,
    INITIALIZE,
    FINALIZE,

    SESSION_OPEN,
    SESSION_CLOSE,
    SESSION_DISCOVER,

    DOMAIN_INFO_GET,
    DRT_ENTRY_GET,
    DOMAIN_TAG_SET,

    RPT_ENTRY_GET,
    RPT_ENTRY_GET_BY_RESOURCE_ID,

    RESOURCE_SEVERITY_SET,
    RESOURCE_TAG_SET,
    RESOURCE_ID_GET,
    GET_CHILD_ENTITY_PATH,
    GET_ID_BY_ENTITY_PATH,    
    RESOURCE_FAILED_REMOVE,
    MY_ENTITY_PATH_GET,

    EVENT_LOG_INFO_GET,
    EVENT_LOG_ENTRY_GET,
    EVENT_LOG_ENTRY_ADD,
    EVENT_LOG_CLEAR,
    EVENT_LOG_TIME_GET,
    EVENT_LOG_TIME_SET,
    EVENT_LOG_STATE_GET,
    EVENT_LOG_STATE_SET,
    EVENT_LOG_OVERFLOW_RESET,
    EVENT_LOG_CAPABILITIES_GET,
    
    SUBSCRIBE,
    UNSUBSCRIBE,
    EVENT_GET,
    EVENT_ADD,

    ALARM_GET_NEXT,
    ALARM_GET,
    ALARM_ACKNOWLEDGE,
    ALARM_ADD,
    ALARM_DELETE,

    RDR_GET,
    RDR_GET_BY_INSTRUMENT_ID,
    RDR_UPDATE_COUNT_GET,

    SENSOR_READING_GET,
    SENSOR_THRESHOLDS_GET,
    SENSOR_THRESHOLDS_SET,
    SENSOR_TYPE_GET,
    SENSOR_ENABLE_GET,
    SENSOR_ENABLE_SET,
    SENSOR_EVENT_ENABLE_GET,
    SENSOR_EVENT_ENABLE_SET,
    SENSOR_EVENT_MASKS_GET,
    SENSOR_EVENT_MASKS_SET,

    CONTROL_TYPE_GET,
    CONTROL_GET,
    CONTROL_SET,

    IDR_INFO_GET,
    IDR_AREA_HEADER_GET,
    IDR_AREA_ADD,
    IDR_AREA_ADD_BY_ID,
    IDR_AREA_DELETE,
    IDR_FIELD_GET,
    IDR_FIELD_ADD,
    IDR_FIELD_ADD_BY_ID,
    IDR_FIELD_SET,
    IDR_FIELD_DELETE,
    
    WATCHDOG_TIMER_GET,
    WATCHDOG_TIMER_SET,
    WATCHDOG_TIMER_RESET,

    ANNUNCIATOR_GET_NEXT,
    ANNUNCIATOR_GET,
    ANNUNCIATOR_ACKNOWLEDGE,
    ANNUNCIATOR_ADD,
    ANNUNCIATOR_DELETE,
    ANNUNCIATOR_MODE_GET,
    ANNUNCIATOR_MODE_SET,

    HOT_SWAP_POLICY_CANCEL,
    RESOURCE_ACTIVE_SET,
    RESOURCE_INACTIVE_SET,
    AUTO_INSERT_TIMEOUT_GET,
    AUTO_INSERT_TIMEOUT_SET,
    AUTO_EXTRACT_TIMEOUT_GET,
    AUTO_EXTRACT_TIMEOUT_SET,
    HOT_SWAP_STATE_GET,
    HOT_SWAP_ACTION_REQUEST,
    HOT_SWAP_INDICATOR_STATE_GET,
    HOT_SWAP_INDICATOR_STATE_SET,

    PARM_CONTROL,

    RESOURCE_RESET_STATE_GET,
    RESOURCE_RESET_STATE_SET,

    RESOURCE_POWER_STATE_GET,
    RESOURCE_POWER_STATE_SET,
    
    FUMI_AUTO_ROLLBACK_DISABLE_GET,
    FUMI_AUTO_ROLLBACK_DISABLE_SET,
    FUMI_LOGICAL_TARGET_INFO_GET,
    FUMI_SERVICE_IMPACT_GET,
    FUMI_SPEC_INFO_GET
} HpiFunctionId;

#endif
