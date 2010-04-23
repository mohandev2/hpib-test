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

#ifndef __HPIEXECUTIVE_H__
#define __HPIEXECUTIVE_H__

#include <cstdio>
#include <iostream>
#include <fstream>

#include "SafTest.h"
#include "Executive.h"
#include "HpiProperties.h"
#include "HpiTestStatus.h"
#include "IdList.h"

using namespace std;

/*****************************************************************************
 * HPI Executive
 *
 * Executes the suite of HPI tests.  The HPI Executive also maintains
 * the HPI info (sessions, etc.) and a set of HPI properties.
 *****************************************************************************/

class HpiExecutive : public Executive
{
private:
    HpiProperties *properties;
    IdList domainIdList;

public:
    HpiExecutive(BaseTestSuite *testSuite);
    virtual ~HpiExecutive();

    void initialize();

    HpiProperties *getProperties();
    IdList &getDomainIdList();

private:
    HpiTestStatus buildDomainIdList(SaHpiDomainIdT domainId);

};

#endif
