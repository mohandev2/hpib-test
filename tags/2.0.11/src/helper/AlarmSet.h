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

#ifndef __ALARMSET_H__
#define __ALARMSET_H__

#include "SafTest.h"
#include "HpiTestStatus.h"

/*****************************************************************************
 * A set of alarms.  
 *****************************************************************************/

class AlarmSet
{
private:
    int addIndex;
    int size;
    SaHpiAlarmT **alarmList;

public:
    AlarmSet();
    AlarmSet(int size);
    ~AlarmSet();

    void setSize(int size);
    void add(SaHpiAlarmT *alarm);
    bool contains(SaHpiAlarmIdT alarmId);

    int getSize();
    SaHpiAlarmT *getAlarm(int index);

    HpiTestStatus deleteAll(SaHpiSessionIdT sessionId);

private:
    void init(int size);
    void free();
};

#endif
