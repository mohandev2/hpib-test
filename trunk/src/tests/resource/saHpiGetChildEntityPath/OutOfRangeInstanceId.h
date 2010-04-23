/*
 * (C) Copyright Emerson Corp. 2009
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 *  Authors:
 *  Anurag Saxena   <Anurag.Saxena@emerson.com>
 */

#ifndef __OUTOFRANGEINSTANCEID_H__
#define __OUTOFRANGEINSTANCEID_H__

#include "SafTest.h"
#include "HpiSessionTestCase.h"

#define MAX_DOMAIN_ENTITY_TREE_DEPTH 1000

/*****************************************************************************
 * A valid instanceId other than SAHPI_FIRST_ENTRY for an entity having no
 * corresponding child entity Test Case
 *****************************************************************************/

namespace ns_saHpiGetChildEntityPath
{
    class OutOfRangeInstanceId : public HpiSessionTestCase
    {
    public:
        OutOfRangeInstanceId(char *line);

        HpiTestStatus runSessionTest(SaHpiSessionIdT sessionId);

        const char *getName();

        const char *getDescription();

        SaErrorT getExpectedReturn();
    };
}

#endif
