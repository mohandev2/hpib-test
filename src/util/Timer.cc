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

#include <config.h>

#ifdef USE_THREADS
#include <atomic>
#include <pthread.h>
#endif

#include <cstdio>
#include <csignal>
#include <iostream>
#include <sys/select.h>
#include <unistd.h>

#include "Timer.h"
#include "Time.h"

/*****************************************************************************
 * Timer Variables
 *****************************************************************************/

TimerEntry Timer::timer[MAX_TIMERS];

#ifdef USE_THREADS
pthread_mutex_t Timer::mutex = PTHREAD_MUTEX_INITIALIZER;
std::atomic_bool Timer::testsAreRunning(false);
#endif

/*****************************************************************************
 * Startup the Timer Utility.
 *
 * Begin getting callbacks via the ALARM signal every second.
 *****************************************************************************/

void Timer::start() {

    for (int i = 0; i < MAX_TIMERS; ++i) {
        timer[i].isActive = false;
    }

#ifndef USE_THREADS
    signal(SIGALRM, alarmHandler);
    alarm(1);
#else
    testsAreRunning.store(true);
    pthread_t thread1;
    int rc = pthread_create(&thread1, NULL, timerHandler, NULL);
    if (rc) {
        perror("pthread_create() failed");
    }
#endif
}

/*****************************************************************************
 * Stop the Timer Utility.
 *
 * Stop thread which is servicing Timer::timerHandler function.
 *****************************************************************************/

void Timer::stop() {
#ifdef USE_THREADS
    testsAreRunning.store(false);
#endif
}

/*****************************************************************************
 * Sleep for a given number of seconds
 *
 * Since sleep can have side effects due to system signals, use this 
 * method to sleep for a given period of seconds.
 *****************************************************************************/

void Timer::sleep(unsigned int seconds) {
    struct timeval timeout;

    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;

    select(0, NULL, NULL, NULL, &timeout);
}

/*****************************************************************************
 * Sleep for a given number of microseconds.
 *
 * Since sleep can have side effects due to system signals, use this 
 * method to sleep for a given period of seconds.
 *****************************************************************************/

void Timer::usecSleep(long usec) {
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = usec;

    select(0, NULL, NULL, NULL, &timeout);
}

/*****************************************************************************
 * Add a timer callback.  After the elapsed number of seconds, the callback
 * function will be invoked with the given data.  After the call, the timer 
 * callback is removed, i.e. the function is only called once.
 *
 * This function returns an ID that indentifies the timer callback.
 *****************************************************************************/

int Timer::add(TimeoutFunc func, int seconds, void *data) {
    return add(func, seconds, data, false);
}

/*****************************************************************************
 * Add a timer callback.  After the elapsed number of seconds, the callback
 * function will be invoked with the given data.  The "repeat" determines if
 * the function will be continuously invoked every "seconds" or only invoked
 * once.
 * 
 * This function returns an ID that indentifies the timer callback.
 *****************************************************************************/

int Timer::add(TimeoutFunc func, int seconds, void *data, bool repeat) {

#ifdef USE_THREADS
    pthread_mutex_lock(&mutex);
#endif

    for (int i = 0; i < MAX_TIMERS; ++i) {
        if (!timer[i].isActive) {
            timer[i].func = func;
            timer[i].seconds = seconds;
            timer[i].data = data;
            timer[i].isRepeated = repeat;
            timer[i].isPaused = false;
            timer[i].isRunning = false;
            timer[i].counter = seconds;
            timer[i].isAlive = true;
            timer[i].isActive = true;
#ifdef USE_THREADS
            pthread_mutex_unlock(&mutex);
#endif
            return i;
        }
    }

#ifdef USE_THREADS
    pthread_mutex_unlock(&mutex);
#endif

    return -1;
}

/*****************************************************************************
 * Remove a timer callback.  The callback function will no longer be invoked.
 *****************************************************************************/

void Timer::remove(int id) {
    timer[id].isActive = false;
}

/*****************************************************************************
 * Pause a timer callback.  Unlike remove(), the timer callback remains and
 * can be restarted with a call to resume().  During a pause, the callback
 * function will not be invoked.
 *****************************************************************************/

void Timer::pause(int id) {
    timer[id].isPaused = true;
}

/*****************************************************************************
 * Resume a timer callback that was paused.
 *****************************************************************************/

void Timer::resume(int id) {
    timer[id].isPaused = false;
}

#ifndef USE_THREADS

/*****************************************************************************
 * The alarmHandler() gets invoked by the Operating System approximately every
 * second.  For every timer callback, which isn't paused, and whose time has
 * expired, invoke the callback function.
 *****************************************************************************/

void Timer::alarmHandler(int ) {

    for (int i = 0; i < MAX_TIMERS; ++i) {
        TimerEntry *timerEntry = &timer[i];
        if (timerEntry->isActive && timerEntry->isAlive && 
            !timerEntry->isPaused && !timerEntry->isRunning) {

            timerEntry->counter--;
            if (timerEntry->counter < 0) {
                timerEntry->isRunning = true;
                timerEntry->func(timerEntry->data);
                if (timerEntry->isRepeated) {
                    timerEntry->counter = timerEntry->seconds;
                } else {
                    timerEntry->isAlive = false;
                }
            }
        }
    }

    alarm(1);
}

#else 

/*****************************************************************************
 * The alarmHandler() gets invoked by the Operating System approximately every
 * second.  For every timer callback, which isn't paused, and whose time has 
 * expired, invoke the callback function.
 *****************************************************************************/

void *Timer::timerHandler(void *) {
    pthread_t thread1;
    pthread_attr_t tattr;

    // initialize an attribute to the default value 
    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);

    while (testsAreRunning.load()) {

        // wait for one second

        Timer::sleep(1);

        pthread_mutex_lock(&mutex);

        for (int i = 0; i < MAX_TIMERS; ++i) {
            TimerEntry *timerEntry = &timer[i];
            if (timerEntry->isActive && timerEntry->isAlive &&
                !timerEntry->isPaused && !timerEntry->isRunning) {

                timerEntry->counter--;
                if (timerEntry->counter < 0) {
                    timerEntry->isRunning = true;

                    // create a detached thread and pass timer entry
                    int rc = pthread_create(&thread1, &tattr, functionHandler, timerEntry);
                    if (rc) {
                        perror("pthread_create() failed");
                    }
                }
            }
        }

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

/*****************************************************************************
 * The functionHandler() is responsible for calling the TimerEntry's
 * callback function.
 *****************************************************************************/

void *Timer::functionHandler(void *data) {
    TimerEntry *timerEntry = (TimerEntry *) data;

    timerEntry->func(timerEntry->data);

    if (timerEntry->isRepeated) {
        timerEntry->counter = timerEntry->seconds;
    } else {
        timerEntry->isAlive = false;
    }

    pthread_exit(0);

    return NULL;
}

#endif

