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

#ifndef __CONTROLTESTCASE_H___
#define __CONTROLTESTCASE_H___

#include "SafTest.h"
#include "HpiRdrTestCase.h"

/********************************************************************************
 *
 * Character data that can be used for testing.  Since BCDPLUS is the most
 * restrictive, we must use characters that are supported by BCDPLUS.
 *
 ********************************************************************************/

#define BYTE_VALUE_1 '1'
#define BYTE_VALUE_2 '2'

/*************************************************************************
 *
 * The Control Data is used to store the mode and the state(s) for a
 * control.  Most control types have only one state, but a Text control
 * can have many states, one for each line.
 *
 *************************************************************************/

typedef struct {
    SaHpiCtrlModeT   mode;
    SaHpiCtrlStateT  *state;
    int              size;    /* Number of states */
} ControlData;

/*****************************************************************************
 * Base Control Test Case
 *****************************************************************************/

class ControlTestCase : public HpiRdrTestCase
{
private:
    ControlData testControlData;

public:
    ControlTestCase();
    ControlTestCase(char* line);

protected:
    HpiTestStatus runRdrTest(SaHpiSessionIdT sessionId,
                             SaHpiRptEntryT *rptEntry,
                             SaHpiRdrT *rdr);

    virtual HpiTestStatus runCtrlTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiCtrlRecT *ctrlRec) = 0;

    bool isTextControl(SaHpiCtrlRecT *ctrlRec);
    bool isAnalogControl(SaHpiCtrlRecT *ctrlRec);
    bool isDigitalControl(SaHpiCtrlRecT *ctrlRec);
    bool isDiscreteControl(SaHpiCtrlRecT *ctrlRec);
    bool isStreamControl(SaHpiCtrlRecT *ctrlRec);
    bool isOemControl(SaHpiCtrlRecT *ctrlRec);

    bool isUnicodeDataType(SaHpiCtrlRecT *ctrlRec);
    bool isTextDataType(SaHpiCtrlRecT *ctrlRec);
    bool isAscii6DataType(SaHpiCtrlRecT *ctrlRec);
    bool isBcdPlusDataType(SaHpiCtrlRecT *ctrlRec);

    SaHpiUint8T getMaxChars(SaHpiCtrlRecT *ctrlRec);
    SaHpiUint8T getMaxLines(SaHpiCtrlRecT *ctrlRec);
    SaHpiTextTypeT getDataType(SaHpiCtrlRecT *ctrlRec);
    SaHpiLanguageT getLanguage(SaHpiCtrlRecT *ctrlRec);

    bool canOverflowLine(SaHpiCtrlRecT *ctrlRec);
    SaHpiUint8T getMaxCharsPerBuffer(SaHpiCtrlRecT *ctrlRec);

    bool canSetControlState(SaHpiCtrlRecT *ctrlRec);
    SaHpiCtrlModeT getDefaultMode(SaHpiCtrlRecT *ctrlRec);

    bool isReadOnlyMode(SaHpiCtrlRecT *ctrlRec);

    void setDataType(SaHpiCtrlStateT *ctrlState, SaHpiTextTypeT dataType);

    void setLanguage(SaHpiCtrlStateT *ctrlState, SaHpiLanguageT language);

    void setDefaultCtrlState(SaHpiCtrlRecT    *ctrlRec,
                             SaHpiCtrlStateT  *ctrlState);

    HpiTestStatus getControlData(SaHpiSessionIdT     sessionId,
                                 SaHpiResourceIdT    resourceId,
                                 SaHpiCtrlRecT       *ctrlRec,
                                 ControlData         *controlData);

    HpiTestStatus saveControlData(SaHpiSessionIdT     sessionId,
                                  SaHpiResourceIdT    resourceId,
                                  SaHpiCtrlRecT       *ctrlRec);

    HpiTestStatus restoreControlData(SaHpiSessionIdT     sessionId,
                                     SaHpiResourceIdT    resourceId,
                                     SaHpiCtrlNumT       ctrlNum);

    HpiTestStatus setControlTextBuffer(SaHpiSessionIdT     sessionId,
                                       SaHpiResourceIdT    resourceId,
                                       SaHpiCtrlRecT       *ctrlRec,
                                       SaHpiTxtLineNumT    lineNum,
                                       SaHpiUint8T         numChars,
                                       SaHpiUint8T         byteValue);

    HpiTestStatus setControlAllTextBuffers(SaHpiSessionIdT     sessionId,
                                           SaHpiResourceIdT    resourceId,
                                           SaHpiCtrlRecT       *ctrlRec,
                                           SaHpiUint8T         byteValue);
};

#endif

