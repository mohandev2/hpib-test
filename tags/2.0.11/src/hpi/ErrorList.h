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

#ifndef __ERRORLIST_H__
#define __ERRORLIST_H__

#include "SafTest.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MAX_ERRORS 5

/*****************************************************************************
 * List of Errors
 *
 * Some HPI functions can return more than one valid error code, such as
 * SA_OK and SA_ERR_HPI_NOT_PRESENT.  For those functions, their expected
 * return error codes must be defined using an ErrorList.
 *****************************************************************************/

class ErrorList
{
private:
    SaErrorT errorList[MAX_ERRORS];
    int count;

public:
    ErrorList();

    void add(SaErrorT error);
    int getCount();
    SaErrorT get(int index);
};

#endif
