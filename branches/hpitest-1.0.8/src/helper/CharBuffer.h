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

#ifndef __CHARBUFFER_H__
#define __CHARBUFFER_H__

#include "SafTest.h"

/*****************************************************************************
 * CharBuffer
 *
 * Used to perform operations on chars that can be converted
 * to byte format based upon the data type.
 *****************************************************************************/

class CharBuffer
{
private:
    bool valid;
    SaHpiUint8T dataLength;  // number of chars 
    SaHpiUint32T data[SAHPI_MAX_TEXT_BUFFER_LENGTH]; 
    SaHpiTextBufferT textBuffer;

public:
    CharBuffer();
    CharBuffer(SaHpiTextTypeT dataType);
    CharBuffer(SaHpiTextTypeT dataType, SaHpiLanguageT language);
    CharBuffer(SaHpiTextBufferT &textBuffer);
    
    void fill(SaHpiUint16T value);
    void fill(SaHpiUint16T value, SaHpiUint8T numChars);

    bool isValid();
    SaHpiTextTypeT getDataType();
    SaHpiLanguageT getLanguage();
    SaHpiUint8T getLength();
    SaHpiUint32T getChar(SaHpiUint8T index);

    bool isBlank(SaHpiUint8T maxChars);
    bool isBlank(SaHpiUint8T startIndex, SaHpiUint8T numChars);

    bool matches(CharBuffer &buf);
    bool matches(CharBuffer &buf, SaHpiUint8T startIndex);
    bool matches(CharBuffer &buf, SaHpiUint8T startIndex, SaHpiUint8T numChars);
    bool matches(SaHpiUint32T value, SaHpiUint8T numChars);

    const SaHpiTextBufferT &getTextBuffer();

private:
    void init(SaHpiTextTypeT dataType, SaHpiLanguageT language);
    SaHpiUint8T getMaxChars();
};

#endif
