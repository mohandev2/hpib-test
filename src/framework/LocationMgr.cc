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
#include <string.h>

#include "LocationMgr.h"
#include "StringBuffer.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MAX_LOCATIONS 10

/*****************************************************************************
 * Declare Variables
 *****************************************************************************/

int LocationMgr::locationCount = 0;
LocationItem LocationMgr::location[MAX_LOCATIONS];

/*****************************************************************************
 * Push a location onto the stack of locations.
 *****************************************************************************/

void LocationMgr::push(const char *name, unsigned int id) {
    if (locationCount < MAX_LOCATIONS) {
        location[locationCount].name = name;
        location[locationCount].id = id;
        locationCount++;
    }
}

/*****************************************************************************
 * Pop the location from the top of the stack.
 *****************************************************************************/

void LocationMgr::pop() {
    locationCount--;
}

/*****************************************************************************
 * Return the current location.  
 *****************************************************************************/

const char *LocationMgr::getLocation() {
    char *buf = StringBuffer::next();
    char idBuf[50];

    if ( buf == NULL )
            return NULL;
    strcpy(buf, "");
    if (locationCount > 0) {
        strcat(buf, "[");
        strcat(buf, location[0].name);
        strcat(buf, ":");
        snprintf(idBuf, 50, "%d", location[0].id);
        strcat(buf, idBuf);
        for (int i = 1; i < locationCount; i++) {
            strcat(buf, "; ");
            strcat(buf, location[i].name);
            strcat(buf, ":");
            snprintf(idBuf, 50, "%d", location[i].id);
            strcat(buf, idBuf);
        }
        strcat(buf, "] ");
    }

    return buf;
}

