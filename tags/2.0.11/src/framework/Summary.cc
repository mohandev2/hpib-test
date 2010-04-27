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

#include "Summary.h"
#include "TestStatus.h"

using namespace std;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Summary::Summary() {
    total = 0;
    passed = 0;
    blocked = 0;
    failed = 0;
    errored = 0;
    notsupported = 0;
    unknown = 0;
    warnings = 0;
}

/*****************************************************************************
 * Update the summary with the given test case.  Keep a count of the
 * status' of all the executed test cases.
 *****************************************************************************/

void Summary::update(BaseTestCase *tc) {
    total++;
    TestStatus &status = tc->getStatus();
    if (status.isUnknown()) {
        unknown++;
    } else if (status.isPassed()) {
        passed++;
    } else if (status.hasWarning()) {
        warnings++;
    } else if (status.hasFailure()) {
        failed++;
    } else if (status.hasError()) {
        errored++;
    } else if (status.isBlocked()) {
        blocked++;
    } else if (status.isNotSupported()) {
        notsupported++;
    }
}

/*****************************************************************************
 * Write the summary to the given output stream.
 *****************************************************************************/

void Summary::write(ostream &out) {
    out << "Passed Test Cases: " << passed << endl;
    if (warnings > 0) {
        out << "    Warnings: " << warnings << endl;
    }
    out << "Blocked Test Cases: " << blocked << endl;
    out << "Failed Test Cases: " << failed << endl;
    out << "Errored Test Cases: " << errored << endl;
    out << "Not Supported Test Cases: " << notsupported << endl;
    out << "Unknown Test Cases: " << unknown << endl;
    out << "Total Executed Test Cases: " << total << endl;
}

