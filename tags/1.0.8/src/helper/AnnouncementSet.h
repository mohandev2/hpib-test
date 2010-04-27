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

#ifndef __ANNOUNCEMENTSET_H__
#define __ANNOUNCEMENTSET_H__

#include "SafTest.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MAX_ANNOUNCEMENTS 10

/*****************************************************************************
 * A set of announcements.
 *****************************************************************************/

class AnnouncementSet
{
private:
    SaHpiAnnouncementT announcement[MAX_ANNOUNCEMENTS];
    int count;

public:
    AnnouncementSet();

    int getCount();
    void add(SaHpiAnnouncementT &newAnnouncement);
    SaHpiAnnouncementT *get(int index);
    void clear();
};

#endif
