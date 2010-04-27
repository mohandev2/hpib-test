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

#include "ControlTestCase.h"
#include "CharBuffer.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ControlTestCase::ControlTestCase()
: HpiRdrTestCase(SAHPI_CAPABILITY_CONTROL) {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ControlTestCase::ControlTestCase(char *line)
: HpiRdrTestCase(line, SAHPI_CAPABILITY_CONTROL) {
}

/*****************************************************************************
 * In order to run the real test, this must be a Control.
 *****************************************************************************/

HpiTestStatus ControlTestCase::runRdrTest(SaHpiSessionIdT sessionId,
                                          SaHpiRptEntryT *rptEntry,
                                          SaHpiRdrT *rdr) {
    HpiTestStatus status;

    if (rdr->RdrType != SAHPI_CTRL_RDR) {
        status.assertNotSupport();
    } else {
        pushLocation("Control", rdr->RdrTypeUnion.CtrlRec.Num);
        status = runCtrlTest(sessionId, rptEntry, &(rdr->RdrTypeUnion.CtrlRec));
        popLocation();
    }

    return status;
}

/*****************************************************************************
 * Is this a Text Control?
 *****************************************************************************/

bool ControlTestCase::isTextControl(SaHpiCtrlRecT *ctrlRec) {
    return (ctrlRec->Type == SAHPI_CTRL_TYPE_TEXT);
}

/*****************************************************************************
 * Is this a Analog Control?
 *****************************************************************************/

bool ControlTestCase::isAnalogControl(SaHpiCtrlRecT *ctrlRec) {
    return (ctrlRec->Type == SAHPI_CTRL_TYPE_ANALOG);
}

/*****************************************************************************
 * Is this a Digital Control?
 *****************************************************************************/

bool ControlTestCase::isDigitalControl(SaHpiCtrlRecT *ctrlRec) {
    return (ctrlRec->Type == SAHPI_CTRL_TYPE_DIGITAL);
}

/*****************************************************************************
 * Is this a Discrete Control?
 *****************************************************************************/

bool ControlTestCase::isDiscreteControl(SaHpiCtrlRecT *ctrlRec) {
    return (ctrlRec->Type == SAHPI_CTRL_TYPE_DISCRETE);
}

/*****************************************************************************
 * Is this a Stream Control?
 *****************************************************************************/

bool ControlTestCase::isStreamControl(SaHpiCtrlRecT *ctrlRec) {
    return (ctrlRec->Type == SAHPI_CTRL_TYPE_STREAM);
}

/*****************************************************************************
 * Is this an Oem Control?
 *****************************************************************************/

bool ControlTestCase::isOemControl(SaHpiCtrlRecT *ctrlRec) {
    return (ctrlRec->Type == SAHPI_CTRL_TYPE_OEM);
}

/*****************************************************************************
 * Is this a UNICODE data type?
 *****************************************************************************/

bool ControlTestCase::isUnicodeDataType(SaHpiCtrlRecT *ctrlRec) {
    return (ctrlRec->TypeUnion.Text.DataType == SAHPI_TL_TYPE_UNICODE);
}

/*****************************************************************************
 * Is this a TEXT data type?
 *****************************************************************************/

bool ControlTestCase::isTextDataType(SaHpiCtrlRecT *ctrlRec) {
    return (ctrlRec->TypeUnion.Text.DataType == SAHPI_TL_TYPE_TEXT);
}

/*****************************************************************************
 * Is this a ASCII6 data type?
 *****************************************************************************/

bool ControlTestCase::isAscii6DataType(SaHpiCtrlRecT *ctrlRec) {
    return (ctrlRec->TypeUnion.Text.DataType == SAHPI_TL_TYPE_ASCII6);
}

/*****************************************************************************
 * Is this a BCD PLUS data type?
 *****************************************************************************/

bool ControlTestCase::isBcdPlusDataType(SaHpiCtrlRecT *ctrlRec) {
    return (ctrlRec->TypeUnion.Text.DataType == SAHPI_TL_TYPE_BCDPLUS);
}

/*****************************************************************************
 * Return the maximum number of characters on a single line in a Text Control.
 *****************************************************************************/

SaHpiUint8T ControlTestCase::getMaxChars(SaHpiCtrlRecT *ctrlRec) {
    return ctrlRec->TypeUnion.Text.MaxChars;
}

/*****************************************************************************
 * Return the maximum number of lines in a Text Control.
 *****************************************************************************/

SaHpiUint8T ControlTestCase::getMaxLines(SaHpiCtrlRecT *ctrlRec) {
    return ctrlRec->TypeUnion.Text.MaxLines;
}

/*****************************************************************************
 * Return the Text Control's Data Type.
 *****************************************************************************/

SaHpiTextTypeT ControlTestCase::getDataType(SaHpiCtrlRecT *ctrlRec) {
    return ctrlRec->TypeUnion.Text.DataType;
}

/*****************************************************************************
 * Is it possible to overflow a single line in a Text Control so that
 * the text gets written to the next line?
 *
 * NOTE: For UNICODE, we are assuming that surrogates will not be written
 *       to the Text Control.
 *****************************************************************************/

bool ControlTestCase::canOverflowLine(SaHpiCtrlRecT *ctrlRec) {
    int maxChars = getMaxChars(ctrlRec);

    if (isUnicodeDataType(ctrlRec)) {
        return maxChars < SAHPI_MAX_TEXT_BUFFER_LENGTH / 2;
    } else {
        return maxChars < SAHPI_MAX_TEXT_BUFFER_LENGTH;
    }
}

/*************************************************************************
 * Get the max number of characters that a Text Buffer can hold.
 * Remember that UNICODE requires 2 bytes per character.
 *************************************************************************/

SaHpiUint8T ControlTestCase::getMaxCharsPerBuffer(SaHpiCtrlRecT *ctrlRec) {
    if (isUnicodeDataType(ctrlRec)) {
        return SAHPI_MAX_TEXT_BUFFER_LENGTH / 2;
    } else {
        return SAHPI_MAX_TEXT_BUFFER_LENGTH;
    }
}

/*************************************************************************
 * Can the control state be set using saHpiControlSet()?
 *
 * The only situation where we can't set the control is when the
 * the default mode is AUTO and the mode is ReadOnly.
 *************************************************************************/

bool ControlTestCase::canSetControlState(SaHpiCtrlRecT *ctrlRec) {
    return (!((ctrlRec->DefaultMode.Mode == SAHPI_CTRL_MODE_AUTO) && 
              (ctrlRec->DefaultMode.ReadOnly)));
}

/*************************************************************************
 * Return the default mode.
 *************************************************************************/

SaHpiCtrlModeT ControlTestCase::getDefaultMode(SaHpiCtrlRecT *ctrlRec) {
    return ctrlRec->DefaultMode.Mode;
}

/*************************************************************************
 * Return true if the mode is read-only; otherwise false.
 *************************************************************************/

bool ControlTestCase::isReadOnlyMode(SaHpiCtrlRecT *ctrlRec) {
    return ctrlRec->DefaultMode.ReadOnly;
}

/*************************************************************************
 * Set the data type.
 *************************************************************************/

void ControlTestCase::setDataType(SaHpiCtrlStateT   *ctrlState, 
                                  SaHpiTextTypeT    dataType) {
    ctrlState->StateUnion.Text.Text.DataType = dataType;
}

/*************************************************************************
 * Return the language used by the Text Control.
 *************************************************************************/

SaHpiLanguageT ControlTestCase::getLanguage(SaHpiCtrlRecT *ctrlRec) {
    return ctrlRec->TypeUnion.Text.Language;
}

/*************************************************************************
 * Set the language used by the Text Control.
 *************************************************************************/

void ControlTestCase::setLanguage(SaHpiCtrlStateT *ctrlState,
                                  SaHpiLanguageT language) {
    ctrlState->StateUnion.Text.Text.Language = language;
}

/*************************************************************************
 * When setting a Text Control state, the Type, DataType, and Language
 * must be set to correspond to what is in the RDR.
 *************************************************************************/

void ControlTestCase::setDefaultCtrlState(SaHpiCtrlRecT *ctrlRec,
                                          SaHpiCtrlStateT *ctrlState) {
    ctrlState->Type = ctrlRec->Type;
    if (ctrlRec->Type == SAHPI_CTRL_TYPE_DIGITAL) {
        ctrlState->StateUnion.Digital = ctrlRec->TypeUnion.Digital.Default;
    } else if (ctrlRec->Type == SAHPI_CTRL_TYPE_DISCRETE) {
        ctrlState->StateUnion.Discrete = ctrlRec->TypeUnion.Discrete.Default;
    } else if (ctrlRec->Type == SAHPI_CTRL_TYPE_ANALOG) {
        ctrlState->StateUnion.Analog = ctrlRec->TypeUnion.Analog.Default;
    } else if (ctrlRec->Type == SAHPI_CTRL_TYPE_STREAM) {
        ctrlState->StateUnion.Stream = ctrlRec->TypeUnion.Stream.Default;
    } else if (ctrlRec->Type == SAHPI_CTRL_TYPE_TEXT) {
        ctrlState->StateUnion.Text = ctrlRec->TypeUnion.Text.Default;
        ctrlState->StateUnion.Text.Line = 1;
    } else if (ctrlRec->Type == SAHPI_CTRL_TYPE_OEM) {
        ctrlState->StateUnion.Oem = ctrlRec->TypeUnion.Oem.Default;
    }
}

/*************************************************************************
 * Get all of the Control Data.
 *************************************************************************/
 
HpiTestStatus ControlTestCase::getControlData(SaHpiSessionIdT sessionId,
                                              SaHpiResourceIdT resourceId,
                                              SaHpiCtrlRecT *ctrlRec,
                                              ControlData *controlData) {
    HpiTestStatus   status;
    SaHpiCtrlNumT   ctrlNum = ctrlRec->Num;
    int             maxLines = ctrlRec->TypeUnion.Text.MaxLines;

    if (!isTextControl(ctrlRec)) {

        controlData->size = 1;
        controlData->state = (SaHpiCtrlStateT *) new SaHpiCtrlStateT[1];
        if (controlData->state == 0) {
            status.assertError(TRACE, "Unable to allocate memory!");
        } else {
            SaErrorT error = saHpiControlGet(sessionId, resourceId, ctrlNum,
                                             &(controlData->mode),
                                             &(controlData->state[0]));
            if (error != SA_OK) {
                status.assertError(TRACE, CONTROL_GET, SA_OK, error);
                controlData->size = 0;
                delete [] controlData->state;
                controlData->state = NULL;
            }
        }
    } else {
        controlData->size = maxLines;
        controlData->state = 
            (SaHpiCtrlStateT *) new SaHpiCtrlStateT[maxLines];
        if (controlData->state == 0) {
            status.assertError(TRACE, "Unable to allocate memory!");
        } else {
            for (int i = 0; i < maxLines; i++) {
                controlData->state[i].Type = SAHPI_CTRL_TYPE_TEXT;
                controlData->state[i].StateUnion.Text.Line = i + 1;
                SaErrorT error = saHpiControlGet(sessionId, resourceId, ctrlNum,
                                                 &(controlData->mode),
                                                 &(controlData->state[i]));
                if (error != SA_OK) {
                    status.assertError(TRACE, CONTROL_GET, SA_OK, error);
                    controlData->size = 0;
                    delete [] controlData->state;
                    controlData->state = NULL;
                    break;
                }
            }
        }
    }

    return status;
}

/*************************************************************************
 * Save all of the Control Data for later restoration.
 *************************************************************************/
 
HpiTestStatus ControlTestCase::saveControlData(SaHpiSessionIdT sessionId,
                                               SaHpiResourceIdT resourceId,
                                               SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;

    status.add(TRACE, getControlData(sessionId, resourceId,
                                     ctrlRec, &testControlData));

    return status;
}

/*************************************************************************
 * Restore all of the original Control Data.
 *************************************************************************/

HpiTestStatus ControlTestCase::restoreControlData(SaHpiSessionIdT sessionId,
                                                  SaHpiResourceIdT resourceId,
                                                  SaHpiCtrlNumT ctrlNum) {
    HpiTestStatus status;

    if (testControlData.mode == SAHPI_CTRL_MODE_AUTO) {
        SaErrorT error = saHpiControlSet(sessionId, resourceId, ctrlNum,
                                         testControlData.mode, NULL);
        if ((error != SA_OK) &&
            (error != SA_ERR_HPI_INVALID_DATA) &&
            (error != SA_ERR_HPI_INVALID_REQUEST)) {
            status.assertError(TRACE, CONTROL_SET, SA_OK, error);
        }
    } else {
        for (int i = 0; i < testControlData.size; i++) {
            SaErrorT error = saHpiControlSet(sessionId, resourceId, ctrlNum,
                                             testControlData.mode, &(testControlData.state[i]));
            if ((error == SA_ERR_HPI_INVALID_DATA) ||
                (error == SA_ERR_HPI_INVALID_REQUEST)) {
                status.assertNotSupport();
                break; // don't bother with any more lines
            } else if (error != SA_OK) {
                status.assertError(TRACE, CONTROL_SET, SA_OK, error);
                break; // prevent more errors from printing
            }
        }
    }

    delete [] testControlData.state;

    return status;
}

/*************************************************************************
 * Set the text for a specific line in a Text Control.  The text
 * will be the given byte value repeated "numChars" times.
 *************************************************************************/

HpiTestStatus ControlTestCase::setControlTextBuffer(SaHpiSessionIdT sessionId,
                                                    SaHpiResourceIdT resourceId,
                                                    SaHpiCtrlRecT *ctrlRec,
                                                    SaHpiTxtLineNumT lineNum,
                                                    SaHpiUint8T numChars,
                                                    SaHpiUint8T byteValue) {
    HpiTestStatus status;
    SaHpiCtrlStateT ctrlState;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;

    CharBuffer buffer(getDataType(ctrlRec), getLanguage(ctrlRec));
    buffer.fill(byteValue, numChars);

    setDefaultCtrlState(ctrlRec, &ctrlState);
    ctrlState.StateUnion.Text.Line = lineNum;
    ctrlState.StateUnion.Text.Text = buffer.getTextBuffer();

    SaErrorT error = saHpiControlSet(sessionId, resourceId, ctrlNum,
                                     SAHPI_CTRL_MODE_MANUAL, &ctrlState);

    if ((error == SA_ERR_HPI_INVALID_DATA) ||
        (error == SA_ERR_HPI_INVALID_REQUEST)) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, CONTROL_SET, SA_OK, error);
    }

    return status;
}

/*************************************************************************
 * Fill every line in the Text Control with the given byte value.
 *************************************************************************/

HpiTestStatus ControlTestCase::setControlAllTextBuffers(SaHpiSessionIdT sessionId,
                                                        SaHpiResourceIdT resourceId,
                                                        SaHpiCtrlRecT *ctrlRec,
                                                        SaHpiUint8T byteValue) {
    HpiTestStatus  status;
    int maxLines = getMaxLines(ctrlRec);
    int maxChars = getMaxChars(ctrlRec);

    for (int lineNum = 1; lineNum <= maxLines; lineNum++) {
        status.add(TRACE, setControlTextBuffer(sessionId, resourceId, ctrlRec,
                                               lineNum, maxChars, byteValue));
        if (!status.isOkay()) {
            break;
        }
    }

    return status;
}

