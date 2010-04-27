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

#include "CharBuffer.h"
#include "TextBufferHelper.h"
#include "Util.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

CharBuffer::CharBuffer() {
    init(SAHPI_TL_TYPE_TEXT, SAHPI_LANG_ENGLISH);
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

CharBuffer::CharBuffer(SaHpiTextTypeT dataType) {
    init(dataType, SAHPI_LANG_ENGLISH);
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

CharBuffer::CharBuffer(SaHpiTextTypeT dataType, SaHpiLanguageT language) {
    init(dataType, language);
}

/*****************************************************************************
 * Constructor
 *
 * Build a CharBuffer based upon an HPI text buffer.  In order to properly
 * process the text buffer, it must be valid.
 *
 * For non-UNICODE data types, the conversion from a text buffer to a char
 * buffer is a straight one-to-one mapping, i.e. each byte corresponds to
 * one character.  For UNICODE, every 2 or 4 bytes corresponds to one 
 * character, i.e. codepoint.  Normally, 2 bytes are used, but if a 
 * surrogate is found, then 4 bytes are used to create the codepoint.
 *****************************************************************************/

CharBuffer::CharBuffer(SaHpiTextBufferT &txtBuffer) {

    if (!TextBufferHelper::isValid(&txtBuffer)) {
        valid = false;
        dataLength = 0;
    } else {
        valid = true;
        this->textBuffer.DataType = txtBuffer.DataType;
        this->textBuffer.Language = txtBuffer.Language;

        if (txtBuffer.DataType != SAHPI_TL_TYPE_UNICODE) {
            dataLength = txtBuffer.DataLength;
            for (SaHpiUint32T i = 0; i < dataLength; i++) {
                data[i] = txtBuffer.Data[i];
            }
        } else { // UNICODE

            const SaHpiUint32T SURROGATE_OFFSET = 0x10000 - (0xD800 << 10) - 0xDC00;

            dataLength = 0;
            int index = 0;
            while (index < txtBuffer.DataLength) {
                SaHpiUint32T low = txtBuffer.Data[index];
                SaHpiUint32T high = txtBuffer.Data[index + 1];
                SaHpiUint32T codepoint = (high << 8) + low;

                // Do we have a surrogate?

                if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                    index += 2;
                    low = txtBuffer.Data[index];
                    high = txtBuffer.Data[index + 1];
                    SaHpiUint32T lead = codepoint;
                    SaHpiUint32T trail = (high << 8) + low;

                    codepoint = (lead << 10) + trail + SURROGATE_OFFSET;
                }

                data[dataLength] = codepoint;
                dataLength++;
                index += 2;
            }
        }
    }
}

/*****************************************************************************
 * Initialize the character buffer.
 *****************************************************************************/

void CharBuffer::init(SaHpiTextTypeT dataType, SaHpiLanguageT language) {
    valid = true;
    dataLength = 0;
    textBuffer.DataType = dataType;
    textBuffer.Language = language;
    textBuffer.DataLength = 0;
}

/*****************************************************************************
 * Fill the entire character buffer with the given value.  The buffer is
 * filled with the maximum number of characters possible based upon the 
 * data type.
 *
 * NOTE: The value is only 16 bits to prevent UNICODE surrogate values.
 *****************************************************************************/

void CharBuffer::fill(SaHpiUint16T value) {
    dataLength = getMaxChars();
    for (int i = 0; i < dataLength; i++) {
        data[i] = value;
    }
}

/*****************************************************************************
 * Fill the character buffer with the given value.  The buffer is filled
 * with the specified number of characters (or less).
 *
 * NOTE: The value is only 16 bits to prevent UNICODE surrogate values.
 *****************************************************************************/

void CharBuffer::fill(SaHpiUint16T value, SaHpiUint8T numChars) {
    dataLength = MIN(numChars, getMaxChars());
    for (int i = 0; i < dataLength; i++) {
        data[i] = value;
    }
}

/*****************************************************************************
 * Return the maximum number of characters that can fill an HPI text buffer.
 * For UNICODE, this method assumes that no surrogates will be used.
 *****************************************************************************/

SaHpiUint8T CharBuffer::getMaxChars() {
    if (textBuffer.DataType == SAHPI_TL_TYPE_UNICODE) {
        return SAHPI_MAX_TEXT_BUFFER_LENGTH / 2;
    } else {
        return SAHPI_MAX_TEXT_BUFFER_LENGTH;
    }
}

/*****************************************************************************
 * Is the character buffer valid or not?
 *****************************************************************************/

bool CharBuffer::isValid() {
    return valid;
}

/*****************************************************************************
 * Return the data type.
 *****************************************************************************/

SaHpiTextTypeT CharBuffer::getDataType() {
    return textBuffer.DataType;
}

/*****************************************************************************
 * Return the language.
 *****************************************************************************/

SaHpiLanguageT CharBuffer::getLanguage() {
    return textBuffer.Language;
}

/*****************************************************************************
 * Return the number of characters in the buffer.
 *****************************************************************************/

SaHpiUint8T CharBuffer::getLength() {
    return dataLength;
}

/*****************************************************************************
 * Return the character at the given index.
 *****************************************************************************/

SaHpiUint32T CharBuffer::getChar(SaHpiUint8T index) {
    return data[index];
}

/*****************************************************************************
 * Is the character buffer blank?
 *
 * For Text Controls, The HPI specification is unclear as to what a blank 
 * line is.  Does a blank line contain no characters?  Or does it contain
 * a full line with only blanks?  And what character code is a blank, 
 * especially when the data type is BINARY?
 *
 * To handle all possible cases, a blank line is defined as the following:
 *
 * 1. An empty buffer is blank.
 * 2. A buffer has the specificed number of maximum characters and all
 *    of those characters are the same character code.
 *****************************************************************************/

bool CharBuffer::isBlank(SaHpiUint8T maxChars) {
    if (dataLength == 0) {
        return true;
    } else if (dataLength != maxChars) {
        return false;
    } else {
        SaHpiUint32T blank = data[0];
        for (SaHpiUint32T i = 1; i < dataLength; i++) {
            if (data[i] != blank) {
                return false;
            }
        }
        return true;
    }
}

/*****************************************************************************
 * Is the sequence of characters starting at "startIndex" and going for 
 * "numChars" a sequence of blanks?  (See above comments for a description
 * of blanks in an HPI Text Control.)
 *****************************************************************************/

bool CharBuffer::isBlank(SaHpiUint8T startIndex, SaHpiUint8T numChars) {
    if (numChars == 0) {
        return true;
    } else {
        SaHpiUint32T blank = data[startIndex];
        for (SaHpiUint32T i = 1; i < numChars; i++) {
            if (data[startIndex + i] != blank) {
                return false;
            }
        }
        return true;
    }
}

/*****************************************************************************
 * Does the given character buffer match this character buffer?
 *****************************************************************************/

bool CharBuffer::matches(CharBuffer &buf) {
    return matches(buf, 0);
}

/*****************************************************************************
 * Does the given character buffer match a part of this character buffer
 * starting at the given index?
 *****************************************************************************/

bool CharBuffer::matches(CharBuffer &buf, SaHpiUint8T startIndex) {
    if (startIndex + buf.dataLength > dataLength) {
        return false;
    } else {
        for (SaHpiUint32T i = 0; i < buf.dataLength; i++) {
            if (data[i + startIndex] != buf.data[i]) {
                return false;
            }
        }
        return true;
    }
}

/*****************************************************************************
 * Does the given character buffer partially match a part of this character 
 * buffer starting at the given index?
 *****************************************************************************/

bool CharBuffer::matches(CharBuffer &buf, SaHpiUint8T startIndex, SaHpiUint8T numChars) {
    if (buf.dataLength - startIndex + 1 < numChars) {
        return false;
    } else {
        for (SaHpiUint32T i = 0; i < numChars; i++) {
            if (data[i + startIndex] != buf.data[i]) {
                return false;
            }
        }
        return true;
    }
}

/*****************************************************************************
 * Does the given repetition of the given value match this buffer?
 *****************************************************************************/

bool CharBuffer::matches(SaHpiUint32T value, SaHpiUint8T numChars) {
    if (numChars > dataLength) {
        return false;
    } else {
        for (int i = 0; i < numChars; i++) {
            if (data[i] != value) {
                return false;
            }
        }

        // The remaining chars must be blanks.

        if (numChars < dataLength) {
            SaHpiUint32T blank = data[numChars];
            for (SaHpiUint32T i = numChars + 1; i < dataLength; i++) {
                if (data[i] != blank) {
                    return false;
                }
            }
        }

        return true;
    }
}

/*****************************************************************************
 * Return the HPI text buffer that corresponds to this character buffer.
 * The characters must be converted to bytes.  For non-UNICODE, this is a
 * one-to-one mapping.  For UNICODE, the codepoints are converted to either
 * 2 or 4 bytes.
 *****************************************************************************/

const SaHpiTextBufferT& CharBuffer::getTextBuffer() {
    if (textBuffer.DataType == SAHPI_TL_TYPE_UNICODE) {
        const SaHpiUint32T LEAD_OFFSET = 0xD800 - (0x10000 >> 10);

        int i = 0;
        for (int j = 0; j < dataLength; j++) {
            SaHpiUint32T codepoint = data[j];
            if (codepoint <= 0xFFFF) {
                textBuffer.Data[i++] = codepoint & 0xFF;
                textBuffer.Data[i++] = (codepoint >> 8) & 0xFF;
            } else {
                SaHpiUint32T lead = LEAD_OFFSET + (codepoint >> 10);
                SaHpiUint32T trail = 0xDC00 + (codepoint & 0x3FF);

                textBuffer.Data[i++] = lead & 0xFF;
                textBuffer.Data[i++] = (lead >> 8) & 0xFF;

                textBuffer.Data[i++] = trail & 0xFF;
                textBuffer.Data[i++] = (trail >> 8) & 0xFF;
            }
        }
        textBuffer.DataLength = i;
    } else {
        textBuffer.DataLength = dataLength;
        for (int i = 0; i < dataLength; i++) {
            textBuffer.Data[i] = data[i] & 0xFF;
        }
    }

    return textBuffer;
}

