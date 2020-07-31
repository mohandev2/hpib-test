/*
 * (C) Copyright Univeristy of New Hampshire, 2006
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

#include <config.h>
#include <cstdio>

#include "Controller.h"
#include "HpiExecutive.h"
#include "RootTestSuite.h"
#include "Arguments.h"
#include "Timer.h"

/*****************************************************************************
 * HPI Test Main Program
 *****************************************************************************/

int main (int argc, char *argv[]) {
    Timer::start();

    RootTestSuite *testSuite = new RootTestSuite();
    HpiExecutive *executive = new HpiExecutive(testSuite);
    Controller controller(VERSION, argc, argv, executive, testSuite);
    controller.run();

    Timer::stop();
}
