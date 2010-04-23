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

#include <cstdio>
#include <cstring>

#include "TextBufferHelper.h"
#include "HpiString.h"
#include "Report.h"

/*****************************************************************************
 * Fill in the Text Buffer with default values.
 *****************************************************************************/

void TextBufferHelper::fill(SaHpiTextBufferT *buf) {
    buf->DataType = SAHPI_TL_TYPE_TEXT;
    buf->Language = SAHPI_LANG_ENGLISH;
    buf->DataLength = 1;
    buf->Data[0] = 'a';
}

/*****************************************************************************
 * Fill in the Text Buffer with the given text string.
 *****************************************************************************/

void TextBufferHelper::fill(SaHpiTextBufferT *buf, const char *text) {
    buf->DataType = SAHPI_TL_TYPE_TEXT;
    buf->Language = SAHPI_LANG_ENGLISH;
    int len = strlen(text);
    buf->DataLength = len;
    for (int i = 0; i < len; i++) {
        buf->Data[i] = text[i];
    }
}

/*****************************************************************************
 * Fill in the Text Buffer with the given number of bytes.
 *****************************************************************************/

void TextBufferHelper::fill(SaHpiTextBufferT *buf, SaHpiUint32T dataLength) {
    buf->DataType = SAHPI_TL_TYPE_TEXT;
    buf->Language = SAHPI_LANG_ENGLISH;
    buf->DataLength = dataLength;
    for (SaHpiUint32T i = 0; i < dataLength; i++) {
        buf->Data[i] = 'a';
    }
}

/*****************************************************************************
 * Fill in the Text Buffer using the given data type.
 *****************************************************************************/

void TextBufferHelper::fillByDataType(SaHpiTextBufferT *buf, SaHpiTextTypeT dataType) {
    buf->DataType = dataType;
    buf->Language = SAHPI_LANG_ENGLISH;
    if (dataType != SAHPI_TL_TYPE_UNICODE) {
        buf->DataLength = 1;
        buf->Data[0] = '0';
    } else { // Unicode requires an even number of bytes
        buf->DataLength = 2;  
        buf->Data[0] = 0x0;
        buf->Data[1] = 'a';
    }
}

/*****************************************************************************
 * Fill in the Text Buffer with invalid character data.  This function is
 * only valid for unicode, bcdplus, and ascii6. 
 *****************************************************************************/

void TextBufferHelper::fillInvalidData(SaHpiTextBufferT *buf,
                                       SaHpiTextTypeT dataType) {
    buf->DataType = dataType;
    buf->Language = SAHPI_LANG_ENGLISH;
    buf->DataLength = 1;
    buf->Data[0] = 'a';
}

/*****************************************************************************
 * Is the text buffer valid or not?
 *****************************************************************************/

bool TextBufferHelper::isValid(SaHpiTextBufferT *buf) {
    Report report;
    return isValid(buf, report);
}

/*****************************************************************************
 * Is the Text Buffer valid or not?  Include a report of anything wrong.
 *****************************************************************************/

bool TextBufferHelper::isValid(SaHpiTextBufferT *buf, Report &report) {
    bool valid = true;

    if (!(buf->DataType >= SAHPI_TL_TYPE_UNICODE && buf->DataType <= SAHPI_TL_TYPE_BINARY)) {
        valid = false;
        report.add("TextBuffer->DataType %s is invalid.", HpiString::dataType(buf->DataType));
    } 
    
    if (buf->DataType == SAHPI_TL_TYPE_TEXT || buf->DataType == SAHPI_TL_TYPE_UNICODE) {
        if (!(buf->Language >= SAHPI_LANG_UNDEF && buf->Language <= SAHPI_LANG_ZULU)) {
            valid = false;
            report.add("TextBuffer->Language 0x%X is invalid.", buf->Language);
        }
    }
    
    if (!isValidCharacterData(buf->DataType, buf->DataLength, buf->Data)) {
        valid = false;
        report.add("TextBuffer->Data is invalid.");
    }

    return valid;
}

/*****************************************************************************
 * Are the two text buffers equal or not?
 *****************************************************************************/

bool TextBufferHelper::isEqual(SaHpiTextBufferT *buf1, SaHpiTextBufferT *buf2) {
    Report report;
    return isEqual(buf1, buf2, report);
}

/*****************************************************************************
 * Are the two text buffers equal or not?
 *****************************************************************************/

bool TextBufferHelper::isEqual(SaHpiTextBufferT *buf1,
                               SaHpiTextBufferT *buf2, Report &report) {
    bool equal = true;

    if (buf1->DataType != buf2->DataType) {
        equal = false;
        report.add("TextBuffer->DataType is not equal [%s, %s].",
                   HpiString::dataType(buf1->DataType),
                   HpiString::dataType(buf2->DataType));
    }

    if (((buf1->DataType == SAHPI_TL_TYPE_TEXT) || (buf2->DataType == SAHPI_TL_TYPE_TEXT)) ||
        ((buf1->DataType == SAHPI_TL_TYPE_UNICODE) || (buf2->DataType == SAHPI_TL_TYPE_UNICODE))) {
        if (buf1->Language != buf2->Language) {
            equal = false;
            report.add("TextBuffer->Language is not equal [%s, %s].",
                       HpiString::language(buf1->Language),
                       HpiString::language(buf2->Language));
       } 
    }
    
    if (buf1->DataLength != buf2->DataLength) {
        equal = false;
        report.add("TextBuffer->DataLength is not equal [%d, %d].",
                   buf1->DataLength, buf2->DataLength);
    } else if (memcmp(buf1->Data, buf2->Data, buf1->DataLength)) {
        equal = false;
        report.add("TextBuffer->Data is not equal.");
    }

    return equal;
}

/*****************************************************************************
 * Convert the text buffer to string format.
 *****************************************************************************/

string TextBufferHelper::toString(SaHpiTextBufferT *buf) {
   string s;
   s.append("{");
   s.append(toString(buf->DataType));
   s.append(",");
   s.append(toString(buf->Language));
   s.append(",");
   s.append(toString(buf->DataLength));
   s.append(",(");
   if (buf->DataLength > 0) {
       s.append(toString(buf->Data[0]));
       for (int i = 1; i < buf->DataLength; i++) {
           s.append(",");
           s.append(toString(buf->Data[i]));
       }
   }
   s.append(")}");
   return s;
}

/*****************************************************************************
 * Convert an integer to a string.
 *****************************************************************************/

string TextBufferHelper::toString(unsigned int value) {
    char buf[100];
    sprintf(buf, "%u", value);
    string s(buf);
    return s;
}

/*****************************************************************************
 * Is the text buffer printable?
 *****************************************************************************/

bool TextBufferHelper::isPrintable(SaHpiTextBufferT *buf) {
    return (buf->DataType == SAHPI_TL_TYPE_BCDPLUS) ||
           (buf->DataType == SAHPI_TL_TYPE_ASCII6) ||
           (buf->DataType == SAHPI_TL_TYPE_TEXT);
}

/*****************************************************************************
 * Convert the text buffer to a printable string.
 *****************************************************************************/

string TextBufferHelper::toPrintable(SaHpiTextBufferT *buf) {
    string s;
    if (isPrintable(buf)) {
        char data[SAHPI_MAX_TEXT_BUFFER_LENGTH + 1];
        for (int i = 0; i < buf->DataLength; i++) {
            data[i] = buf->Data[i];
        }
        data[buf->DataLength] = '\0';
        s.append(data);
    }
    return s;
}

/*****************************************************************************
 * Is this a valid language?
 *****************************************************************************/

bool TextBufferHelper::isValidLanguage(SaHpiLanguageT language) {
    return (language >= SAHPI_LANG_UNDEF && language <= SAHPI_LANG_ZULU);
}

/*****************************************************************************
 * Is this the character data valid?
 *
 * The two values FFFE and FFFF as well as the 32 values from FDD0 to FDEF 
 * represent noncharacters. They are invalid in interchange, but may be freely 
 * used internal to an implementation. Unpaired surrogates are invalid as well, 
 * i.e. any value in the range D800 to DBFF not followed by a value in the range 
 * DC00 to DFFF, or any value in the range DC00 to DFFF not preceded by a value 
 * in the range D800 to DBFF.
 *****************************************************************************/

bool TextBufferHelper::isValidCharacterData(SaHpiTextTypeT dataType,
                                            SaHpiUint8T dataLength,
                                            SaHpiUint8T *data) {

    if (dataType == SAHPI_TL_TYPE_UNICODE) {
        if (dataLength % 2 != 0) {
            return false;
        } else {
            int i = 0;
            while (i < dataLength) {
                int codepoint = data[i] + (((int) data[i+1]) << 8);
                if (codepoint == 0xFFFE) {
                    return false;
                } else if (codepoint == 0xFFFF) {
                    return false;
                } else if (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                    return false;
                } else if (codepoint >= 0xFDD0 && codepoint <= 0xFDEF) {
                    return false;
                } else if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                    i += 2;
                    if (i >= dataLength) {
                        return false;
                    } else {
                        unsigned short surrogate = data[i] + (((int) data[i+1]) << 8);
                        if (surrogate < 0xDC00 || surrogate > 0xDFFF) {
                            return false;
                        } else if (codepoint & 0x003F == 0x003F) {
                            return false;
                        } else if (surrogate & 0x03FE == 0x03FE) {
                            return false;
                        }
                    }
                }
                i += 2;
            }
        }
    } else if (dataType == SAHPI_TL_TYPE_BCDPLUS) {
        for (int i = 0; i < dataLength; i++) {
            if (!isBcdCharacter(data[i])) {
                return false;
            }
        }
    } else if (dataType == SAHPI_TL_TYPE_ASCII6) {
        for (int i = 0; i < dataLength; i++) {
            if (data[i] < 0x20 || data[i] > 0x5F) {
                return false;
            }
        }
    }

    return true;
}

/*****************************************************************************
 * Is this a valid BCD PLUS Character?
 *****************************************************************************/

bool TextBufferHelper::isBcdCharacter(SaHpiUint8T ch) {
    if (ch >= '0' && ch <= '9') {
        return true;
    } else if (ch == ' ') {
        return true;
    } else if (ch == '.') {
        return true;
    } else if (ch == ':') {
        return true;
    } else if (ch == ',') {
        return true;
    } else if (ch == '_') {
        return true;
    }

    return false;
}
