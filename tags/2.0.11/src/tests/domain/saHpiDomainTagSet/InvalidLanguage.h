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

#ifndef __INVALIDLANGUAGE_H__
#define __INVALIDLANGUAGE_H__

#include "SafTest.h"
#include "TagSetTestCase.h"

/*****************************************************************************
 * Invalid Language Test Case
 *****************************************************************************/

namespace ns_saHpiDomainTagSet
{
    class InvalidLanguage : public TagSetTestCase
    {
    public:
        InvalidLanguage(char *line);

        HpiTestStatus runTagTest(SaHpiSessionIdT sessionId);

        const char *getName();

        const char *getDescription();

        SaErrorT getExpectedReturn();
    };
}

#endif
