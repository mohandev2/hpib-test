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
#include <cstdlib>
#include "AppExit.h"
#include "Timer.h"

/*****************************************************************************
 * Abort the application due to a test case timeout.  
 *****************************************************************************/

void AppExit::timeoutError(const char *name)
{
    printf("\n");
    printf("******************** UNRECOVERABLE TIMEOUT ERROR  ********************\n");
    printf("Test Case: %s\n", name);
    printf(" \n");
    printf("For unknown reasons, this test case did not execute within\n");
    printf("the timeout period.  Some of the possible reasons are:\n");
    printf("\n");
    printf("    1. The HPI implementation did not return after invocation of\n");
    printf("       an HPI function.\n");
    printf("    2. The timeout period was too short.  Try increasing the timeout\n");
    printf("       with the TestTimeout property in the ~/.hpitest properties\n");
    printf("       file.\n");
    printf("    3. The test case entered an infinite loop.\n");
    printf("\n");
    printf("Until the issue is resolved, it is recommended that this test case\n");
    printf("be excluded by using the \"-e\" option on the command line.\n");
    printf("**********************************************************************\n");
    printf("\n");

    exit(1);
}
