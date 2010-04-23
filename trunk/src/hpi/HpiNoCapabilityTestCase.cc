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

#include "HpiNoCapabilityTestCase.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

HpiNoCapabilityTestCase::HpiNoCapabilityTestCase(char *line, const char *capability)
: HpiResourceTestCase(line) {
    sprintf(description, "Test a resource that does not have the "
                         "needed %s capability.", capability);

    sprintf(precondition, "Requires a resource that does not support the "
                          "%s capability.", capability);
}

/*****************************************************************************
 * Return the name of the test.
 *****************************************************************************/

const char *HpiNoCapabilityTestCase::getName() {
    return "NoCapability";
}

/*****************************************************************************
 * Return the description.
 *****************************************************************************/

const char *HpiNoCapabilityTestCase::getDescription() {
    return description;
}

/*****************************************************************************
 * Return the precondition.
 *****************************************************************************/

const char *HpiNoCapabilityTestCase::getPrecondition() {
    return precondition;
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT HpiNoCapabilityTestCase::getExpectedReturn() {
    return SA_ERR_HPI_CAPABILITY;
}

