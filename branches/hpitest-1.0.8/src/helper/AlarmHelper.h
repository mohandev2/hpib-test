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

#ifndef __ALARMHELPER_H__
#define __ALARMHELPER_H__

#include "SafTest.h"
#include "HpiTestStatus.h"

class AlarmSet;
class Report;

using namespace std;

/*****************************************************************************
 * Alarm Helper
 *****************************************************************************/

class AlarmHelper
{
public:
    static void fill(SaHpiAlarmT *alarm);
    static void fill(SaHpiAlarmT *alarm, SaHpiSeverityT severity);

    static bool isEqual(SaHpiAlarmT *alarm1, SaHpiAlarmT *alarm2);
    static bool isEqual(SaHpiAlarmT *alarm1, SaHpiAlarmT *alarm2, Report &report);

    static bool isValid(SaHpiAlarmT *alarm, Report &report);

    static HpiTestStatus addFullSuite(SaHpiSessionIdT sessionId, AlarmSet *alarmSet);

    static HpiTestStatus findUnusedSeverity(SaHpiSessionIdT sessionId, SaHpiSeverityT *severity, bool *found);

    static HpiTestStatus findSystemAlarm(SaHpiSessionIdT sessionId, SaHpiAlarmT *alarm, bool *found);
};

#endif
