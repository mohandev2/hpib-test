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

#ifndef __LOCATIONMGR_H__
#define __LOCATIONMGR_H__

/*****************************************************************************
 * Each location consists of a name and it's ID.  The name is used to 
 * identify the ID.
 *****************************************************************************/

typedef struct {
    const char *name;
    unsigned int id;
} LocationItem;

/*****************************************************************************
 * LocationMgr
 *
 * The Location Manager is used to identify which "object" in an HPI system
 * is being tested.  For example, if a particular domain is being tested, it's
 * identifier might be:
 *
 *     [Domain:0x1]
 *
 * Likewise, if a resource is being tested, it's identifer might be:
 *
 *     [Domain:0x1; Resource:0x4]
 *
 *****************************************************************************/

class LocationMgr
{
private:
    static LocationItem location[];
    static int locationCount;

public:
    static void push(const char *name, unsigned int id);
    static void pop();
    static const char *getLocation();
};

#endif

