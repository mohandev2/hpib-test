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
 *
 * 09/07/28 lars.wetzel@emerson.com
 *          Indroduction of IdrAttribute and defines
 */

#ifndef __WRITEIDRTESTCASE_H___
#define __WRITEIDRTESTCASE_H___

#include "SafTest.h"
#include "IdrTestCase.h"

/*****************************************************************************
 * Write IDR Test Case
 *
 * The base class for IDR tests that need to write to the IDR.
 *****************************************************************************/

#define WRITE_TC_READONLY_IDR  0
#define WRITE_TC_READWRITE_IDR 1
#define WRITE_TC_ALL_IDR       2

class WriteIdrTestCase : public IdrTestCase
{
private:
  int ReadIdrAttribute;

public:
    WriteIdrTestCase(int  IdrAttribute);
    WriteIdrTestCase(char* line, int IdrAttribute);

protected:
    HpiTestStatus runIdrTest(SaHpiSessionIdT sessionId,
                             SaHpiRptEntryT *rptEntry,
                             SaHpiInventoryRecT *idrRec);

    virtual HpiTestStatus runWriteIdrTest(SaHpiSessionIdT sessionId,
                                          SaHpiRptEntryT *rptEntry,
                                          SaHpiInventoryRecT *idrRec) = 0;

    HpiTestStatus addField(SaHpiSessionIdT sessionId,
                           SaHpiResourceIdT resourceId,
                           SaHpiIdrIdT idrId,
                           SaHpiEntryIdT areaId,
                           SaHpiIdrFieldT *field);
};

#endif

