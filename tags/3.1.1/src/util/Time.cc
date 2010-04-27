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

#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include "Time.h"
#include "StringBuffer.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define ONE_SECOND 1000  /* msec */
#define ONE_MINUTE 60000 /* msec */

/*****************************************************************************
 * Return the current time in milliseconds.
 *****************************************************************************/

TimeMsec Time::getCurrent() {
    struct timeval tv;

    gettimeofday(&tv, NULL);
    TimeMsec msec = (tv.tv_sec * 1000LL + (tv.tv_usec + 500LL) / 1000LL);
        
    return msec;
}

/*****************************************************************************
 * Convert the given elapsed time in milliseconds to a string.  
 * Depending upon the amount of elapsed time, the string will be
 * formatted for milliseconds, seconds, or minutes.
 *****************************************************************************/

char *Time::toString(TimeMsec elapsedTime) {
    char *buf = StringBuffer::next();
    if (elapsedTime < ONE_SECOND) {
        sprintf(buf, "%lld msec", elapsedTime);
    } else if (elapsedTime < ONE_MINUTE) {
        double seconds = elapsedTime / (double) ONE_SECOND;
        sprintf(buf, "%.2lf seconds", seconds);
    } else {
        double minutes = elapsedTime / (double) ONE_MINUTE;
        sprintf(buf, "%.2lf minutes", minutes);
    }
    return buf;
}
