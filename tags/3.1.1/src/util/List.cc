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

#include "List.h"

/*****************************************************************************
 * Default Constructor.
 * 
 * The List is a stored as a singly-linked list.
 *****************************************************************************/

List::List() {
    head = NULL; 
    tail = NULL; 
}

/*****************************************************************************
 * Destructor.  
 *****************************************************************************/

List::~List() {
    clear();
}

/*****************************************************************************
 * Add an object to the list.
 *****************************************************************************/

void List::add(void *object) {
    ListItem* item = new ListItem(object);
    if (head == NULL) {
        head = item;
        tail = item;
    } else {
        tail->next = item;
        tail = item;
    }
}

/*****************************************************************************
 * Remove an object from the list.
 *****************************************************************************/

void List::remove(void *object) {
    if (head->get() == object) {
        if (head == tail) {
            delete head;
            head = NULL;
            tail = NULL;
        } else {
            ListItem *temp = head;
            head = head->next;
            delete temp;
        }
    } else {
        ListItem *prev = NULL;
        ListItem *item = head;
        while (item != NULL && item->get() != object) {
            prev = item;
            item = item->next;
        }
        if (item != NULL) {
            if (item == tail) {
                tail = prev;
            }
            prev->next = item->next;
            delete item;
        }
    }
}

/*****************************************************************************
 * Is the List empty?
 *****************************************************************************/

bool List::isEmpty() {
    return (head == NULL);
}

/*****************************************************************************
 * Return an object in the list.
 *****************************************************************************/

void *List::get(int index) {
    int i = 0;
    ListItem *item = head;
    while (item != NULL) {
        if (i == index) {
            return item->get();
        }
        i++;
        item = item->next;
    }

    return NULL;
}

/*****************************************************************************
 * Return an iterator for traversing a list.  
 *
 * NOTE: We are not returning allocating a ListIterator because we don't
 *       want to force the caller to delete the ListIterator.
 *****************************************************************************/

ListIterator List::iterator() const {
    return ListIterator(head);
}

/*****************************************************************************
 * Delete the ListItems but don't touch the data objects that are stored in
 * the ListItems.
 *****************************************************************************/

void List::clear() {
    ListItem *item = head;
    while (item != NULL) {
        ListItem *next = item->next;
        delete item;
        item = next;
    }
    head = NULL;
    tail = NULL;
}

