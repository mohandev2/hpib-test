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

#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__

/*****************************************************************************
 * Test Properties
 *****************************************************************************/

class Properties
{
private:
    int testTimeout;

public:
    Properties();
    virtual ~Properties();
    void initialize(const char *filename);

    int getTestTimeout();

protected:
    virtual void setDefaults();
    virtual void setPropertyValue(const char *name, const char *value);

    unsigned long hexToLong(const char *hex, bool *error);
};

#endif

