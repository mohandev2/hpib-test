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

#ifndef __INVALIDPARENTENTITYPATH_H__
#define __INVALIDPARENTENTITYPATH_H__

#include "SafTest.h"
#include "HpiSessionTestCase.h"

/*****************************************************************************
 * INVALID Parent Entity Path Test Case
 *****************************************************************************/

namespace ns_saHpiGetChildEntityPath
{
    class InvalidParentEntityPath : public HpiSessionTestCase
    {
    public:
        InvalidParentEntityPath(char *line);

        HpiTestStatus runSessionTest(SaHpiSessionIdT sessionId);

        const char *getName();

        const char *getDescription();

        SaErrorT getExpectedReturn();
    };
}

#endif
