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
 *
 * Changes
 * 2009/05/19 - Lars.Wetzel@emerson.com
 *              enhance isEqual for FUMI and DIMI RDR
 * 2009/11/04 - larswetzel@users.sourceforge.net
 *              Use the RDR_TYPE_MAX definition
 */

#include "RdrHelper.h"
#include "Report.h"
#include "HpiString.h"
#include "ResourceHelper.h"
#include "SensorHelper.h"
#include "ControlHelper.h"
#include "InventoryHelper.h"
#include "WatchdogHelper.h"
#include "AnnunciatorHelper.h"
#include "DimiHelper.h"
#include "FumiHelper.h"
#include "HpiHelper.h"
#include "TextBufferHelper.h"

/*****************************************************************************
 * Return the RDR's Instrument Id.  It depends on the type of RDR.
 *****************************************************************************/

SaHpiInstrumentIdT RdrHelper::getId(SaHpiRdrT *rdr) {
    SaHpiInstrumentIdT id = 0;

    switch (rdr->RdrType) {
        case SAHPI_NO_RECORD:
            break;
        case SAHPI_SENSOR_RDR:
            id = (SaHpiInstrumentIdT) rdr->RdrTypeUnion.SensorRec.Num;
            break;
        case SAHPI_CTRL_RDR:
            id = (SaHpiInstrumentIdT) rdr->RdrTypeUnion.CtrlRec.Num;
            break;
        case SAHPI_INVENTORY_RDR:
            id = (SaHpiInstrumentIdT) rdr->RdrTypeUnion.InventoryRec.IdrId;
            break;
        case SAHPI_WATCHDOG_RDR:
            id = (SaHpiInstrumentIdT) rdr->RdrTypeUnion.WatchdogRec.WatchdogNum;
            break;
        case SAHPI_ANNUNCIATOR_RDR:
            id = (SaHpiInstrumentIdT) rdr->RdrTypeUnion.AnnunciatorRec.AnnunciatorNum;
            break;
        case SAHPI_DIMI_RDR:
            id = (SaHpiInstrumentIdT) rdr->RdrTypeUnion.DimiRec.DimiNum;
            break;
        case SAHPI_FUMI_RDR:
            id = (SaHpiInstrumentIdT) rdr->RdrTypeUnion.FumiRec.Num;
            break;

    }

    return id;
}

/*****************************************************************************
 * Is the RDR valid or not?
 *****************************************************************************/

bool RdrHelper::isValid(SaHpiRdrT *rdr, Report &report) {
    bool valid = true;

    if (rdr->RdrType <= SAHPI_NO_RECORD || rdr->RdrType > SAHPI_RDR_TYPE_MAX_VALID) {
        valid = false;
        report.add("Rdr->RdrType is invalid [%s].", HpiString::rdrType(rdr->RdrType));
    }

    switch (rdr->RdrType) {
        case SAHPI_NO_RECORD:
            break;
        case SAHPI_SENSOR_RDR:
            if (!SensorHelper::isValid(&rdr->RdrTypeUnion.SensorRec, report)) {
                valid = false;
            }
            break;
        case SAHPI_CTRL_RDR:
            if (!ControlHelper::isValid(&rdr->RdrTypeUnion.CtrlRec, report)) {
                valid = false;
            }
            break;
        case SAHPI_INVENTORY_RDR:
            if (!InventoryHelper::isValid(&rdr->RdrTypeUnion.InventoryRec, report)) {
                valid = false;
            }
            break;
        case SAHPI_WATCHDOG_RDR:
            if (!WatchdogHelper::isValid(&rdr->RdrTypeUnion.WatchdogRec, report)) {
                valid = false;
            }
            break;
        case SAHPI_ANNUNCIATOR_RDR:
            if (!AnnunciatorHelper::isValid(&rdr->RdrTypeUnion.AnnunciatorRec, report)) {
                valid = false;
            }
            break;
        case SAHPI_DIMI_RDR:
            if (!DimiHelper::isValid(&rdr->RdrTypeUnion.DimiRec, report)) {
                valid = false;
            }
            break;
        case SAHPI_FUMI_RDR:
            if (!FumiHelper::isValid(&rdr->RdrTypeUnion.FumiRec, report)) {
                valid = false;
            }
            break;
    }

    return valid;
}

/*****************************************************************************
 * Are the two RDR records the same or not?
 *****************************************************************************/

bool RdrHelper::isEqual(SaHpiRdrT *rdr1, SaHpiRdrT *rdr2) {
    Report report;
    return isEqual(rdr1, rdr2, report);
}

/*****************************************************************************
 * Are the two RDR records the same or not?  Include a report of the
 * differences.
 *****************************************************************************/

bool RdrHelper::isEqual(SaHpiRdrT *rdr1, SaHpiRdrT *rdr2, Report &report) {
    bool equal = true;

    if (rdr1->RecordId != rdr2->RecordId) {
        equal = false;
        report.add("Rdr->RecordId fields are not equal [0x%X, 0x%X].", rdr1->RecordId, rdr2->RecordId);
    }

    if (rdr1->RdrType != rdr2->RdrType) {
        equal = false;
        report.add("Rdr->RdType fields are not equal [%s, %s].", 
                   HpiString::rdrType(rdr1->RdrType), HpiString::rdrType(rdr2->RdrType));
    } else {
        switch (rdr1->RdrType) {
        case SAHPI_NO_RECORD:
            break;
        case SAHPI_SENSOR_RDR:
            if (!SensorHelper::isEqual(&rdr1->RdrTypeUnion.SensorRec, 
                                       &rdr2->RdrTypeUnion.SensorRec, report)) {
                equal = false;
            }
            break;
        case SAHPI_CTRL_RDR:
            if (!ControlHelper::isEqual(&rdr1->RdrTypeUnion.CtrlRec, 
                                        &rdr2->RdrTypeUnion.CtrlRec, report)) {
                equal = false;
            }
            break;
        case SAHPI_INVENTORY_RDR:
            if (!InventoryHelper::isEqual(&rdr1->RdrTypeUnion.InventoryRec, 
                                          &rdr2->RdrTypeUnion.InventoryRec, report)) {
                equal = false;
            }
            break;
        case SAHPI_WATCHDOG_RDR:
            if (!WatchdogHelper::isEqual(&rdr1->RdrTypeUnion.WatchdogRec, 
                                         &rdr2->RdrTypeUnion.WatchdogRec, report)) {
                equal = false;
            }
            break;
        case SAHPI_ANNUNCIATOR_RDR:
            if (!AnnunciatorHelper::isEqual(&rdr1->RdrTypeUnion.AnnunciatorRec, 
                                            &rdr2->RdrTypeUnion.AnnunciatorRec, report)) {
                equal = false;
            }
            break;
        case SAHPI_DIMI_RDR:
            if (!DimiHelper::isEqual(&rdr1->RdrTypeUnion.DimiRec, 
                                     &rdr2->RdrTypeUnion.DimiRec, report)) {
                equal = false;
            }
            break;
        case SAHPI_FUMI_RDR:
            if (!FumiHelper::isEqual(&rdr1->RdrTypeUnion.FumiRec, 
                                     &rdr2->RdrTypeUnion.FumiRec, report)) {
                equal = false;
            }
            break;
        }
    }

    // Cannot compare IsFru;  This field is only valid if the Entity path given
    // in the "Entity" field is different from the Entity path in the RPT
    // entry for the resource.  In other words, the IsFru might be uninitialized.
    //
    // if (!HpiHelper::isBoolEqual(rdr1->IsFru, rdr2->IsFru)) {
    //    equal = false;
    //    report.add("Rdr->IsFru fields are not equal [%s, %s].", 
    //              HpiString::boolean(rdr1->IsFru), HpiString::boolean(rdr2->IsFru));
    //}

    if (!ResourceHelper::isEqual(&rdr1->Entity, &rdr2->Entity)) {
        report.add("Rdr->Entity is not equal.");
        equal = false;
    }

    if (!TextBufferHelper::isEqual(&rdr1->IdString, &rdr2->IdString, report)) {
        equal = false;
    }

    return equal;
}
