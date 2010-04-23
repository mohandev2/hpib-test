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
 * Changes:
 * 09/06/24 saxena.anurag@emerson.com
 *          Add getInvalidEntityPath
 * 09/10/28 <larswetzel@users.sourceforge.net>
 *          Add isChildOf
 */

#ifndef __RESOURCEHELPER_H__
#define __RESOURCEHELPER_H__

#include "SafTest.h"

class Report;

/*****************************************************************************
 * Resource Helper
 *****************************************************************************/

class ResourceHelper
{
public:
    static void fillEntityPath(SaHpiEntityPathT *entityPath);
    static void getInvalidEntityPath(SaHpiEntityPathT *entityPath);
    static bool isChildOf(SaHpiEntityPathT *childPath, SaHpiEntityPathT *parentPath);
    static bool isEqual(SaHpiRptEntryT *rptEntry1, SaHpiRptEntryT *rptEntry2);
    static bool isEqual(SaHpiRptEntryT *rptEntry1, SaHpiRptEntryT *rptEntry2, Report &report);
    static bool isEqual(SaHpiResourceInfoT *info1, SaHpiResourceInfoT *info2, Report &report);
    static bool isEqual(SaHpiEntityPathT *path1, SaHpiEntityPathT *path2);
};

#endif
