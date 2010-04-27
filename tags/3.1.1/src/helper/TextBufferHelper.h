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

#ifndef __TEXTBUFFERHELPER_H__
#define __TEXTBUFFERHELPER_H__

#include <string>
#include "SafTest.h"
#include "Report.h"

using namespace std;

/*****************************************************************************
 * Text Buffer Helper
 *****************************************************************************/

class TextBufferHelper
{
public:
    static void fill(SaHpiTextBufferT *buf);
    static void fill(SaHpiTextBufferT *buf, SaHpiUint32T dataLength);
    static void fill(SaHpiTextBufferT *buf, const char *text);
    static void fillByDataType(SaHpiTextBufferT *buf, SaHpiTextTypeT dataType);
    static void fillInvalidData(SaHpiTextBufferT *buf, SaHpiTextTypeT dataType);
    static bool isValid(SaHpiTextBufferT *buf);
    static bool isValid(SaHpiTextBufferT *buf, Report &report);
    static bool isEqual(SaHpiTextBufferT *buf1, SaHpiTextBufferT *buf2);
    static bool isEqual(SaHpiTextBufferT *buf1, SaHpiTextBufferT *buf2, Report &report);
    static string toString(SaHpiTextBufferT *buf);
    static bool isPrintable(SaHpiTextBufferT *buf);
    static string toPrintable(SaHpiTextBufferT *buf);
    static bool isValidLanguage(SaHpiLanguageT language);
    static bool isValidCharacterData(SaHpiTextTypeT dataType,
                                     SaHpiUint8T dataLength,
                                     SaHpiUint8T *data);

private:
    static string toString(unsigned int value);
    static bool isBcdCharacter(SaHpiUint8T ch);
};

#endif
