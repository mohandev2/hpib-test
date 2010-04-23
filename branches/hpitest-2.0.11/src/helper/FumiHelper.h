/*
 * (C) Copyright Pigeon Point Systems, 2009
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
 *     Anton Pak <anton.pak@pigeonpoint.com>
 */

#ifndef __FUMIHELPER_H__
#define __FUMIHELPER_H__

#include "SafTest.h"

class Report;

/*****************************************************************************
 * DIMI Helper
 *****************************************************************************/

class FumiHelper
{
public:
    static bool isValid(SaHpiFumiRecT *rec, Report &report);
    static bool isEqual(SaHpiFumiRecT *rec1, SaHpiFumiRecT *rec2);
    static bool isEqual(SaHpiFumiRecT *rec1, SaHpiFumiRecT *rec2, Report &report);
};

#endif

