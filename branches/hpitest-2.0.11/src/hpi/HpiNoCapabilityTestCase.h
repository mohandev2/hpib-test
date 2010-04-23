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

#ifndef __HPINOCAPABILITYTESTCASE_H__
#define __HPINOCAPABILITYTESTCASE_H__

#include "SafTest.h"
#include "HpiResourceTestCase.h"

/*****************************************************************************
 * HpiNoCapabilityTestCase
 *
 * Many HPI function require testing for a CAPABILITY error.  This is
 * the base class for those tests.
 *****************************************************************************/

class HpiNoCapabilityTestCase : public HpiResourceTestCase
{
private:
    char description[500];
    char precondition[500];

public:
    HpiNoCapabilityTestCase(char *line, const char *capability);

    virtual const char *getName();

    virtual const char *getDescription();

    virtual const char *getPrecondition();

    SaErrorT getExpectedReturn();
};

#endif

