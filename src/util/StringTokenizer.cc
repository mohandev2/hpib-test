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

#include "StringTokenizer.h"

#include <string.h>

using namespace std;

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

StringTokenizer::StringTokenizer(const char *s) {
    data = s;
    size = strlen(s);
    index = 0;
    skipDelimiters();
}

/*****************************************************************************
 * Does the tokenizer have more tokens to process.
 *****************************************************************************/

bool StringTokenizer::hasMoreTokens() {
    return (index < size);
}

/*****************************************************************************
 * Return the next token.
 *****************************************************************************/

string StringTokenizer::nextToken() {
    char buf[2];
    buf[1] = 0;
    string token;
    while (!isDelimiter(data[index])) {
        if (!token.empty() && data[index] == '<') break;
        buf[0] = data[index];
        token.append(buf);
        index++;
        if (data[index-1] == '>') break;
    }
    skipDelimiters();

    return token;
}

/*****************************************************************************
 * Is this a delimiter?
 *****************************************************************************/

bool StringTokenizer::isDelimiter(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0');
}

/*****************************************************************************
 * Skip over delimiters to get to the start of the next token.
 *****************************************************************************/

void StringTokenizer::skipDelimiters() {
    while (isDelimiter(data[index]) && index < size) {
        index++;
    }
}

