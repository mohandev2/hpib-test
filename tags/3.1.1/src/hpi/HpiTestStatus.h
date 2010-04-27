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

#ifndef __HPITESTSTATUS_H__
#define __HPITESTSTATUS_H__

#include <string>
#include "TestStatus.h"
#include "SafTest.h"
#include "HpiFunctionId.h"

using namespace std;

/*****************************************************************************
 * HpiTestStatus
 *****************************************************************************/

class HpiTestStatus : public TestStatus
{
public:
    HpiTestStatus();
    HpiTestStatus(const HpiTestStatus &status);

    HpiTestStatus& operator=(const TestStatus& status);
    HpiTestStatus& operator=(const HpiTestStatus& status);

    using TestStatus::assertError;

    void assertError(const char *func, const char *fname, int line, 
                     HpiFunctionId functionId, SaErrorT expected, SaErrorT error);

    void assertError(const char *func, const char *fname, int line, 
                     HpiFunctionId functionId, SaErrorT expected,
                     SaErrorT error, const char *fmt, ...);

    void assertError(const char *func, const char *fname, int line, 
                     HpiFunctionId functionId, SaErrorT expected1, 
                     SaErrorT expected2, SaErrorT error);

    void assertError(const char *func, const char *fname, int line, 
                     HpiFunctionId functionId, SaErrorT expected1, 
                     SaErrorT expected2, SaErrorT error, const char *fmt, ...);

    void checkError(const char *func, const char *fname, int line, 
                    HpiFunctionId functionId, SaErrorT error);

    void checkError(const char *func, const char *fname, int line, 
                    HpiFunctionId functionId, SaErrorT error, const char *fmt, ...);

    using TestStatus::assertFailure;

    void assertFailure(const char *func, const char *fname, int line, 
                       HpiFunctionId functionId, SaErrorT expected, SaErrorT error);

    void assertFailure(const char *func, const char *fname, int line, 
                       HpiFunctionId functionId, SaErrorT expected,
                       SaErrorT error, const char *fmt, ...);

    void assertFailure(const char *func, const char *fname, int line, HpiFunctionId functionId, 
                       SaErrorT expected1, SaErrorT expected2, SaErrorT error);

    void assertFailure(const char *func, const char *fname, int line, HpiFunctionId functionId, 
                       SaErrorT expected1, SaErrorT expected2, SaErrorT error, const char *fmt, ...);

};

#endif
