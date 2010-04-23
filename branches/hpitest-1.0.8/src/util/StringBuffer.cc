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

#include "StringBuffer.h"

/*****************************************************************************
 * Declare the buffers and the initialize index to the first available
 * buffer.
 *****************************************************************************/

char StringBuffer::buffer[STRING_BUFCNT][STRING_BUFLEN];
int StringBuffer::index = 0;

/*****************************************************************************
 * Return the next available buffer.
 *****************************************************************************/

char *StringBuffer::next() {
    char *buf = buffer[index];
    index++;
    if (index >= STRING_BUFCNT) {
        index = 0;
    }
    return buf;
}
