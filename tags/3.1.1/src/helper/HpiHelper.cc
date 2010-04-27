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

#include "HpiHelper.h"

/*****************************************************************************
 * Determine if two HPI boolean values are equal or not.  For HPI booleans,
 * a TRUE value is any non-zero value.  Thus, integers 2 and 3 are both true
 * and are thus equivalent.
 *****************************************************************************/

bool HpiHelper::isBoolEqual(SaHpiBoolT b1, SaHpiBoolT b2) {
    if (b1 == SAHPI_FALSE && b2 == SAHPI_FALSE) {
        return true;
    } else if (b1 != SAHPI_FALSE && b2 != SAHPI_FALSE) {
        return true;
    } else {
        return false;
    }
}

/*****************************************************************************
 * Is the severity one of the valid enumerated values?
 *****************************************************************************/

bool HpiHelper::isValidSeverity(SaHpiSeverityT severity) {
    return ((severity == SAHPI_CRITICAL) ||
            (severity == SAHPI_MAJOR) ||
            (severity == SAHPI_MINOR) ||
            (severity == SAHPI_INFORMATIONAL) ||
            (severity == SAHPI_OK) ||
            (severity == SAHPI_DEBUG));
}

