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

#include "ControlHelper.h"
#include "TextBufferHelper.h"
#include "HpiHelper.h"
#include "HpiString.h"
#include "Report.h"

/*****************************************************************************
 * Is the CtrlRec valid or not?  Include a report of everything that is
 * found to be invalid.
 *****************************************************************************/

bool ControlHelper::isValid(SaHpiCtrlRecT *rec, Report &report) {
    bool valid = true;

    if ((rec->Type < SAHPI_CTRL_TYPE_DIGITAL || rec->Type > SAHPI_CTRL_TYPE_OEM) ||
        (rec->Type > SAHPI_CTRL_TYPE_TEXT && rec->Type < SAHPI_CTRL_TYPE_OEM)) {

        valid = false;
        report.add("CtrlRec->Type is invalid [0x%X].", rec->Type);
    }

    if (rec->OutputType < SAHPI_CTRL_GENERIC || rec->OutputType > SAHPI_CTRL_OEM) {
        valid = false;
        report.add("CtrlRec->OutputType is invalid [0x%X].", rec->OutputType);
    }

    if (rec->DefaultMode.Mode < SAHPI_CTRL_MODE_AUTO || 
        rec->DefaultMode.Mode > SAHPI_CTRL_MODE_MANUAL) {

        valid = false;
        report.add("CtrlRec->DefaultMode.Mode is invalid [0x%X].", rec->DefaultMode.Mode);
    }

    SaHpiCtrlRecAnalogT *analog;
    SaHpiCtrlRecStreamT *stream;
    SaHpiCtrlRecTextT *text;

    switch (rec->Type) {
        case  SAHPI_CTRL_TYPE_DIGITAL :
            // nothing to verify
            break;

        case SAHPI_CTRL_TYPE_DISCRETE :
            // nothing to verify
            break;

        case SAHPI_CTRL_TYPE_ANALOG :
            analog = &(rec->TypeUnion.Analog);
            if (analog->Default < analog->Min) {
                valid = false;
                report.add("CtrlRec->TypeUnion.Analog.Default (%d) is less than the Min (%d).",
                           analog->Default, analog->Min);
            } else if (analog->Default > analog->Max) {
                valid = false;
                report.add("CtrlRec->TypeUnion.Analog.Default (%d) is greater than the Max (%d).",
                           analog->Default, analog->Max);
            }
            break;

        case SAHPI_CTRL_TYPE_STREAM :
            stream = &(rec->TypeUnion.Stream);
            if (stream->Default.StreamLength > SAHPI_CTRL_MAX_STREAM_LENGTH) {
                valid = false;
                report.add("CtrlRec->TypeUnion.Stream.Default.StreamLength (%d) is "
                           "greater than SAHPI_CTRL_MAX_STREAM_LENGTH.",
                           stream->Default.StreamLength);
            }
            break;

        case SAHPI_CTRL_TYPE_TEXT :
            text = &(rec->TypeUnion.Text);
            if (text->DataType == SAHPI_TL_TYPE_UNICODE) {
                if (text->MaxChars * 2 > SAHPI_MAX_TEXT_BUFFER_LENGTH) {
                    valid = false;
                    report.add("CtrlRec->TypeUnion.Text.MaxChars (%d) exceeds "
                               "SAHPI_MAX_TEXT_BUFFER_LENGTH limit for UNICODE.",
                               text->MaxChars);
                } 

                if (!TextBufferHelper::isValidLanguage(text->Language)) {
                    valid = false;
                    report.add("CtrlRec->TypeUnion.Text.Language (0x%X) is invalid.",
                               text->Language);
                }
            } else if (text->DataType == SAHPI_TL_TYPE_TEXT) {
                if (!TextBufferHelper::isValidLanguage(text->Language)) {
                    valid = false;
                    report.add("CtrlRec->TypeUnion.Text.Language (0x%X) is invalid.",
                               text->Language);
                }
            } else if (text->DataType == SAHPI_TL_TYPE_BCDPLUS) {
                // do nothing
            } else if (text->DataType == SAHPI_TL_TYPE_ASCII6) {
                // do nothing
            } else if (text->DataType == SAHPI_TL_TYPE_BINARY) {
                // do nothing
            } else {
                valid = false;
                report.add("CtrlRec->TypeUnion.Text.DataType (0x%X) is invalid.", text->DataType);
            }

            if (!TextBufferHelper::isValid(&text->Default.Text, report)) {
                valid = false;
                report.add("CtrlRec->TypeUnion.Text.Default.Text is invalid.");
            }
            break;

        case SAHPI_CTRL_TYPE_OEM :
            // nothing to verify
            break;
    }

    return valid;
}

/*****************************************************************************
 * Are the two Ctrl Records the same or not?
 *****************************************************************************/

bool ControlHelper::isEqual(SaHpiCtrlRecT *rec1, SaHpiCtrlRecT *rec2) {
    Report report;
    return isEqual(rec1, rec2, report);
}

/*****************************************************************************
 * Are the two Ctrl Records the same or not?  Include report of any
 * differences.
 *****************************************************************************/

bool ControlHelper::isEqual(SaHpiCtrlRecT *rec1, SaHpiCtrlRecT *rec2, Report &report) {
    bool equal = true;

    if (rec1->Num != rec2->Num) {
        equal = false;
        report.add("CtrlRec->Num fields are not equal [0x%X, 0x%X].", rec1->Num, rec2->Num);
    }

    if (rec1->OutputType != rec2->OutputType) {
        equal = false;
        report.add("CtrlRec->OutputType fields are not equal [%s, %s].", 
                   HpiString::ctrlOutputType(rec1->OutputType), 
                   HpiString::ctrlOutputType(rec2->OutputType));
    }

    if (rec1->Type != rec2->Type) {
        equal = false;
        report.add("CtrlRec->Type fields are not equal [%s, %s].", 
                   HpiString::ctrlType(rec1->Type), 
                   HpiString::ctrlType(rec2->Type));
    }

    if (rec1->DefaultMode.Mode != rec2->DefaultMode.Mode) {
        equal = false;
        report.add("CtrlRec->DefaultMode.Mode fields are not equal [%s, %s].", 
                   HpiString::ctrlMode(rec1->DefaultMode.Mode), 
                   HpiString::ctrlMode(rec2->DefaultMode.Mode));
    }

    if (!HpiHelper::isBoolEqual(rec1->DefaultMode.ReadOnly, rec2->DefaultMode.ReadOnly)) {
        equal = false;
        report.add("CtrlRec->DefaultMode.ReadOnly fields are not equal [%s, %s].", 
                   HpiString::boolean(rec1->DefaultMode.ReadOnly), 
                   HpiString::boolean(rec2->DefaultMode.ReadOnly));
    }

    if (!HpiHelper::isBoolEqual(rec1->WriteOnly, rec2->WriteOnly)) {
        report.add("CtrlRec->WriteOnly fields are not equal [%s, %s].", 
                   HpiString::boolean(rec1->WriteOnly), 
                   HpiString::boolean(rec2->WriteOnly));
    }

    if (rec1->Oem != rec2->Oem) {
        equal = false;
        report.add("CtrlRec->Oem fields are not equal [0x%X, 0x%X].", 
                   rec1->Oem, rec2->Oem);
    }

    // FUTURE NOTE: Add comparisons for specific types if both types are the same.

    return equal;
}

/*****************************************************************************
 * Is the CtrlType one of the valid enumerated types or not?
 *****************************************************************************/

bool ControlHelper::isValidCtrlType(SaHpiCtrlTypeT ctrlType) {
    return ((ctrlType == SAHPI_CTRL_TYPE_DIGITAL) ||
            (ctrlType == SAHPI_CTRL_TYPE_DISCRETE) ||
            (ctrlType == SAHPI_CTRL_TYPE_ANALOG) ||
            (ctrlType == SAHPI_CTRL_TYPE_STREAM) ||
            (ctrlType == SAHPI_CTRL_TYPE_TEXT) ||
            (ctrlType == SAHPI_CTRL_TYPE_OEM));
}

/*****************************************************************************
 * Is the CtrlMode one of the valid enumerated types or not?
 *****************************************************************************/

bool ControlHelper::isValidCtrlMode(SaHpiCtrlModeT ctrlMode) {
    return (ctrlMode == SAHPI_CTRL_MODE_AUTO) ||
           (ctrlMode == SAHPI_CTRL_MODE_MANUAL);
}

