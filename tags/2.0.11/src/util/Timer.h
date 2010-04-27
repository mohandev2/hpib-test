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

#ifndef __TIMER_H__
#define __TIMER_H__

#include <config.h>

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MAX_TIMERS 10

/*****************************************************************************
 * Timer callback function.
 *****************************************************************************/

typedef void (*TimeoutFunc)(void *data);

/*****************************************************************************
 * A TimerEntry stores a timer callback.  
 *
 * Fields:
 *    id: unique ID identify the TimerEntry (timer callback)
 *    func: callback function
 *    seconds: the number of elapsed seconds before invoking the 
 *             callback function
 *    timeCounter: countdown in seconds
 *    data: the data to send to the callback function
 *    repeat: call function once (false) or call function continuously (true)
 *    pause: has the timer callback been paused?
 *****************************************************************************/

class TimerEntry
{
public:
    bool isActive;
    bool isAlive;
    TimeoutFunc func;     // callback function
    int seconds;   // the number of elapsed msec before invoking
    int counter;
    void *data;
    bool isRepeated;
    bool isPaused;
    bool isRunning;

    TimerEntry() {
        isActive = false;
    }
};

/*****************************************************************************
 * The Timer Utility.
 *
 * This is a simple utility for executing a code segment after an elapsed 
 * period of time.  The elapsed time is in seconds.  The Operating System's
 * Alarm Signal mechanism is used to implement this functionality.  Thus, the
 * time in seconds is not completely accurate, but still reliable.
 *****************************************************************************/

class Timer
{
private:
    static TimerEntry timer[];
#ifdef USE_THREADS
    static pthread_mutex_t mutex;
#endif

public:
    Timer() { }

    static void start();
    static void sleep(unsigned int seconds);
    static void usecSleep(long usec);
    static int add(TimeoutFunc func, int seconds, void *data);
    static int add(TimeoutFunc func, int seconds, void *data, bool repeat);
    static void remove(int id);
    static void pause(int id);
    static void resume(int id);
#ifndef USE_THREADS
    static void alarmHandler(int dummy);
#else
    static void *timerHandler(void *);
    static void *functionHandler(void *data);
#endif
};

#endif
