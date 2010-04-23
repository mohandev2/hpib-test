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

#include "AnnouncementSet.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AnnouncementSet::AnnouncementSet() {
    count = 0;
}

/*****************************************************************************
 * Return the number of announcements in the set.
 *****************************************************************************/

int AnnouncementSet::getCount() {
    return count;
}

/*****************************************************************************
 * Add an announcement to the set.
 *****************************************************************************/

void AnnouncementSet::add(SaHpiAnnouncementT &newAnnouncement) {
    if (count < MAX_ANNOUNCEMENTS) {
        announcement[count] = newAnnouncement;
        count++;
    }
}

/*****************************************************************************
 * Return an announcement.
 *****************************************************************************/

SaHpiAnnouncementT *AnnouncementSet::get(int index) {
    return &announcement[index];
}

/*****************************************************************************
 * Clear the list of announcements.
 *****************************************************************************/

void AnnouncementSet::clear() {
    count = 0;
}

