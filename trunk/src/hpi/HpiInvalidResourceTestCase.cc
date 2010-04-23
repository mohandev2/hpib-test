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

#include "HpiInvalidResourceTestCase.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

HpiInvalidResourceTestCase::HpiInvalidResourceTestCase(char *line) 
: HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test.
 *****************************************************************************/

const char *HpiInvalidResourceTestCase::getName() {
    return "InvalidResource";
}

/*****************************************************************************
 * Return the description.
 *****************************************************************************/

const char *HpiInvalidResourceTestCase::getDescription() {
    return "Test a resource identifier for which there is no resource.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT HpiInvalidResourceTestCase::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_RESOURCE;
}

