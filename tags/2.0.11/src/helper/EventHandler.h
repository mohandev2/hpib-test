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

#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include "SafTest.h"
#include "HpiTestStatus.h"

/*****************************************************************************
 * Event Handler
 *
 * Event Handlers are used to process events in collaberation with the 
 * Event Helper.  The Event Helper's pollEvents() method poll events.  When
 * a event that is being looked for is found, the Event Handler is notified
 * via handleEvent().  
 *
 * pollEvents() will also "ask" the Event Handler whether or not it should
 * stop polling for events via isEventProcessingDone().
 *****************************************************************************/

class EventHandler
{
public:
    virtual ~EventHandler() {}
    virtual HpiTestStatus handleEvent(SaHpiSessionIdT sessionId, SaHpiEventT *event) = 0;
    virtual bool isEventProcessingDone() = 0;
};

#endif
