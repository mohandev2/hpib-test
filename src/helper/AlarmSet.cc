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

#include "AlarmSet.h"

/*****************************************************************************
 * Constructor.  The set is empty.
 *****************************************************************************/

AlarmSet::AlarmSet() {
    this->size = 0;
    this->alarmList = NULL;
}

/*****************************************************************************
 * Constructor.  Set the number of alarms to store in the set.
 *****************************************************************************/

AlarmSet::AlarmSet(int sz) {
    init(sz);
}

/*****************************************************************************
 * Destructor.
 *****************************************************************************/

AlarmSet::~AlarmSet() {
    free();
}

/*****************************************************************************
 * Set the number of alarms to store in the set.
 * This will cause any currently stored alarms to be discarded.
 *****************************************************************************/

void AlarmSet::setSize(int sz) {
    free();
    init(sz);
}

/*****************************************************************************
 * Initialize the alarm set.  Allocate the memory.
 *****************************************************************************/

void AlarmSet::init(int sz) {
    this->addIndex = 0;
    this->size = sz;
    this->alarmList = new SaHpiAlarmT*[size];
    for (int i = 0; i < size; i++) {
        alarmList[i] = NULL;
    }
}

/*****************************************************************************
 * Free the allocated alarms.
 *****************************************************************************/

void AlarmSet::free() {
    if (alarmList != NULL) {
        for (int i = 0; i < this->size; i++) {
            if (alarmList[i] != NULL) {
                delete alarmList[i];
            }
        }
        delete [] alarmList;
        alarmList = NULL;
    }
}

/*****************************************************************************
 * Add an alarm to the set.  Insert into the next available space.
 * Make a copy of the alarm.
 *****************************************************************************/

void AlarmSet::add(SaHpiAlarmT *alarm) {
    if (addIndex < size) {
        alarmList[addIndex] = new SaHpiAlarmT;
        *(alarmList[addIndex]) = *alarm;
        addIndex++;
    }
}

/*****************************************************************************
 * Does the Alarm set contain an alarm with the given alarm id?
 *****************************************************************************/

bool AlarmSet::contains(SaHpiAlarmIdT alarmId) {
    for (int i = 0; i < size; i++) {
        if (alarmList[i] != NULL) {
            if (alarmList[i]->AlarmId == alarmId) {
                return true;
            }
        }
    }
    return false;
}

/*****************************************************************************
 * Return the size of the Alarm Set.
 *****************************************************************************/

int AlarmSet::getSize() {
    return size;
}

/*****************************************************************************
 * Get an alarm from the Alarm Set.
 *****************************************************************************/

SaHpiAlarmT *AlarmSet::getAlarm(int index) {
    return alarmList[index];
}

/*****************************************************************************
 * Delete all of the alarms in the Alarm Set from the actual
 * HPI Alarm Table.
 *****************************************************************************/

HpiTestStatus AlarmSet::deleteAll(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    for (int i = 0; i < size; i++) {
        if (alarmList[i] != NULL) {
            SaErrorT error = saHpiAlarmDelete(sessionId, alarmList[i]->AlarmId, SAHPI_MINOR);
            status.checkError(TRACE, ALARM_DELETE, error);
        }
    }

    return status;
}
