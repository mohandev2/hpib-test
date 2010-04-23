/*
 * (C) Copyright Pigeon Point Systems, 2009
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
 *     Anton Pak <anton.pak@pigeonpoint.com>
 */

#include "FumiHelper.h"
#include "Report.h"

static const SaHpiFumiProtocolT all_protocols =
    SAHPI_FUMI_PROT_TFTP |
    SAHPI_FUMI_PROT_FTP |
    SAHPI_FUMI_PROT_HTTP |
    SAHPI_FUMI_PROT_LDAP |
    SAHPI_FUMI_PROT_LOCAL |
    SAHPI_FUMI_PROT_NFS |
    SAHPI_FUMI_PROT_DBACCESS;

static const SaHpiFumiCapabilityT all_caps =
    SAHPI_FUMI_CAP_ROLLBACK |
    SAHPI_FUMI_CAP_BANKCOPY |
    SAHPI_FUMI_CAP_BANKREORDER |
    SAHPI_FUMI_CAP_BACKUP |
    SAHPI_FUMI_CAP_TARGET_VERIFY;

/*****************************************************************************
 * Is the Fumi Record valid?
 *****************************************************************************/

bool FumiHelper::isValid(SaHpiFumiRecT *rec, Report &report) {
    bool valid = true;

    if ( ( rec->AccessProt & all_protocols ) != rec->AccessProt ) {
        valid = false;
        report.add("Some of Fumi Access Protocols are unknown [0x%X]. Known mask is [0x%X]",
                   rec->AccessProt, all_protocols );
    }
    if ( ( rec->Capability & all_caps ) != rec->Capability ) {
        valid = false;
        report.add("Some of Fumi Capabilities are unknown [0x%X]. Known mask is [0x%X]",
                   rec->Capability, all_caps );
    }

    return valid;
}

/*****************************************************************************
 * Are the two Fumi records the same or not?
 *****************************************************************************/

bool FumiHelper::isEqual(SaHpiFumiRecT *rec1, SaHpiFumiRecT *rec2) {
    Report report;
    return isEqual(rec1, rec2, report);
}

/*****************************************************************************
 * Are the two Fumi records the same or not?  Also return a report
 * of what was not the same.
 *****************************************************************************/

bool FumiHelper::isEqual(SaHpiFumiRecT *rec1, SaHpiFumiRecT *rec2, Report &report) {
    bool equal = true;

    if (rec1->Num != rec2->Num) {
        equal = false;
        report.add("Num fields are not equal [0x%X, 0x%X].", 
                   rec1->Num, rec2->Num);
    }
    if (rec1->AccessProt != rec2->AccessProt) {
        equal = false;
        report.add("AccessProt fields are not equal [0x%X, 0x%X].", 
                   rec1->AccessProt, rec2->AccessProt);
    }
    if (rec1->Capability != rec2->Capability) {
        equal = false;
        report.add("Capability fields are not equal [0x%X, 0x%X].", 
                   rec1->Capability, rec2->Capability);
    }
    if (rec1->NumBanks != rec2->NumBanks) {
        equal = false;
        report.add("NumBanks fields are not equal [0x%X, 0x%X].", 
                   rec1->NumBanks, rec2->NumBanks);
    }
    if (rec1->Oem != rec2->Oem) {
        equal = false;
        report.add("Fumi->Oem fields are not equal [%d, %d].", 
                   rec1->Oem, rec2->Oem);
    }

    return equal;
}

