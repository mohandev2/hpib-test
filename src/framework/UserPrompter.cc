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

#include <stdlib.h>

#include <cstring>
#include <iostream>
#include <limits>
#include <unistd.h>
#include <termios.h>

#include "UserPrompter.h"

using namespace std;

/*****************************************************************************
 * Prompt the user.  Wait for Enter to be pressed to continue.
 *****************************************************************************/

void UserPrompter::prompt(const char *message) {
    struct termios oldTerm, newTerm;

    int fd_stdin = fileno(stdin);

    // save the original terminal state
    if (tcgetattr(fd_stdin, &oldTerm) != 0) {
        cout << "tcgetattr() failed!" << endl;
        exit(1);
    }

    // flush the input stream
    tcflush(fd_stdin, TCIFLUSH);

    cout << "\n***************************************************************\n";
    cout << message << "\n";
    cout << "When ready, press Enter to continue.\n";
    cout.flush();

    // Turn off the terminal's echo
    newTerm = oldTerm;
    newTerm.c_lflag &= ~ECHO;
    if (tcsetattr(fd_stdin, TCSAFLUSH, &newTerm) != 0) {
        cout << "tcsetattr() failed!" << endl;
        exit(1);
    }

    // wait for the user to press the Enter key
    while (cin.get() != '\n');

    // restore the original terminal state
    if (tcsetattr(fd_stdin, TCSAFLUSH, &oldTerm) != 0) {
        cout << "tcsetattr() failed!" << endl;
        exit(1);
    }

    cout << "Test case is continuing..." << endl << endl;
}
