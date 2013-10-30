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


/*****************************************************************************
 * String Buffer
 *
 * Provides temporary character buffers.  Since the buffers are recycled,
 * users should not hold onto the buffer for a long period of time.  In
 * most cases, the buffer can simply be copied into another string owned
 * by the user.
 *****************************************************************************/

#ifndef __STRINGBUFFER_H__
#define __STRINGBUFFER_H__

#include <stdio.h>
#include <string>

using namespace std;

#define STRING_BUFCNT 10
#define STRING_BUFLEN 2000

class StringBuffer
{
private:
    static char buffer[STRING_BUFCNT][STRING_BUFLEN];
    static int index;

public:
    static char *next();
};

#endif
