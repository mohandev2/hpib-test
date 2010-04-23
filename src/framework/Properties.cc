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

#include <cstring>
#include <iostream>
#include <fstream>

#include "Properties.h"

using namespace std;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define DEFAULT_TEST_TIMEOUT 120
#define MIN_TEST_TIMEOUT 60

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Properties::Properties() {
}

/*****************************************************************************
 * Destructor
 *****************************************************************************/

Properties::~Properties() {
}

/*****************************************************************************
 * Initialize the properties.  First, set the default values.  Then read in
 * the configuration file to override the default values.
 *****************************************************************************/

void Properties::initialize(const char *filename) {
    char pathname[2000];
    char line[500];
    char *homedir = getenv("HOME");

    setDefaults();

    sprintf(pathname, "%s/%s", homedir, filename);
    ifstream in;
    in.open(pathname);
    if (in) {
        while (!in.eof()) {
            line[0] = '\0';
            in >> line;
            if (strlen(line) != 0) {
                char *loc = index(line, '=');
                if (loc == NULL) {
                    cout << "Invalid property line: " << line << endl;
                    exit(1);
                } else {
                    loc[0] = '\0';
                    char *name = line;
                    char *value = loc + 1;
                    setPropertyValue(name, value);
                }
            }
        }
        in.close();
    }
}

/*****************************************************************************
 * Set the default values.
 *****************************************************************************/

void Properties::setDefaults() {
    testTimeout = DEFAULT_TEST_TIMEOUT; // in seconds
}

/*****************************************************************************
 * Return the test timeout period.
 *****************************************************************************/

int Properties::getTestTimeout() {
    return testTimeout;
}

/*****************************************************************************
 * Set a property based upon a name/value pair.
 *****************************************************************************/

void Properties::setPropertyValue(const char *name, const char *value) {
    if (strcmp(name, "TestTimeout") == 0) {
        testTimeout = atoi(value);
        if (testTimeout < MIN_TEST_TIMEOUT) {
            testTimeout = MIN_TEST_TIMEOUT;
        }
    } else {
        cout << "Unknown property: " << name << endl;
        exit(1);
    }
}

/*****************************************************************************
 * Convert a hex string to a long value.
 *****************************************************************************/

unsigned long Properties::hexToLong(const char *hex, bool *error) {
    *error = false;

    const char *hexStg = hex;
    if (*hex == '0' && (*(hex+1) == 'x' || *(hex+1) == 'X')) {
        hexStg = hex+2;
    }

    int len = strlen(hexStg);
    if (len > 8) {
        *error = true;
        return 0;
    }

    int n = 0;         // position in string
    int m = 0;         // position in digit[] to shift
    int count;         // loop index
    unsigned long value = 0;   // value of hex string
    unsigned long digit[10];   // hold values to convert

    while (n < len) {
        if (hexStg[n]=='\0') {
            break;
        }

        if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ) {       // if 0 to 9
            digit[n] = hexStg[n] & 0x0f;                   // convert to int
        } else if (hexStg[n] >='a' && hexStg[n] <= 'f') {  // if a to f
            digit[n] = (hexStg[n] & 0x0f) + 9;             // convert to int
        } else if (hexStg[n] >='A' && hexStg[n] <= 'F') {  // if A to F
            digit[n] = (hexStg[n] & 0x0f) + 9;             // convert to int
        } else {
            *error = true;
            return 0;
        }
        n++;
    }

    count = n;
    m = n - 1;
    n     = 0;
    while(n < count) {
        // digit[n] is value of hex digit at position n
        // (m << 2) is the number of positions to shift
        // OR the bits into return value
        value = value | (digit[n] << (m << 2));
        m--;   // adjust the position to set
        n++;   // next digit to process
    }

    return value;
}
