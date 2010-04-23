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

#include "HpiInvalidCommandTestCase.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

HpiInvalidCommandTestCase::HpiInvalidCommandTestCase(char *line, const char *command)
: HpiResourceTestCase(line) {
    sprintf(description, "Test to see that the resource does not accept "
                         "an invalid command %s.", command);

    sprintf(precondition, "Requires a resource that does not support the "
                          "%s command.", command);
}

/*****************************************************************************
 * Return the name of the test.
 *****************************************************************************/

const char *HpiInvalidCommandTestCase::getName() {
    return "InvalidCommand";
}

/*****************************************************************************
 * Return the description.
 *****************************************************************************/

const char *HpiInvalidCommandTestCase::getDescription() {
    return description;
}

/*****************************************************************************
 * Return the precondition.
 *****************************************************************************/

const char *HpiInvalidCommandTestCase::getPrecondition() {
    return precondition;
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT HpiInvalidCommandTestCase::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_CMD;
}

