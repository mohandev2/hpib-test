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

#ifndef __LIST_H__
#define __LIST_H__

#include <cstdio>
#include "ListItem.h"
#include "ListIterator.h"

/*****************************************************************************
 * List of objects.
 *****************************************************************************/

class List
{
private:
    ListItem* head;
    ListItem* tail;

public:
    List();
    ~List();

    void add(void *object);
    void remove(void *object);

    bool isEmpty();
    void *get(int index);

    ListIterator iterator() const;

    void clear();
};

#endif
