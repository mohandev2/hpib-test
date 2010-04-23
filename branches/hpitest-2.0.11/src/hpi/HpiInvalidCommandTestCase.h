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
 * AKMAL MUQEETH <akmal.muqeeth@unh.edu>
 * PUSHPA DATLA <pushpa.datla@unh.edu>
 */

#ifndef __HPIINVALIDCOMMANDTESTCASE_H__
#define __HPIINVALIDCOMMANDTESTCASE_H__

#include "SafTest.h"
#include "HpiResourceTestCase.h"

/*****************************************************************************
 * HpiInvalidCommandTestCase
 *
 * Many HPI function require testing for an invalid command error.  This is
 * the base class for all those tests.
 *****************************************************************************/

class HpiInvalidCommandTestCase : public HpiResourceTestCase
{
private:
    char description[500];
    char precondition[500];

public:
    HpiInvalidCommandTestCase(char *line, const char *capability);

    virtual const char *getName();

    virtual const char *getDescription();

    virtual const char *getPrecondition();

    SaErrorT getExpectedReturn();
};

#endif

