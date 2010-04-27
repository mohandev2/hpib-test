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

#include "ConditionHelper.h"
#include "ResourceHelper.h"
#include "TextBufferHelper.h"
#include "NameHelper.h"
#include "HpiString.h"

/*****************************************************************************
 * Fill with default values.
 *****************************************************************************/

void ConditionHelper::fill(SaHpiConditionT *cond) {
    cond->Type = SAHPI_STATUS_COND_TYPE_USER;
    ResourceHelper::fillEntityPath(&cond->Entity);
    cond->DomainId = SAHPI_UNSPECIFIED_DOMAIN_ID;
    cond->ResourceId = SAHPI_UNSPECIFIED_RESOURCE_ID;
    cond->SensorNum = 0x0;
    cond->EventState = 0x0;
    NameHelper::fill(&cond->Name);
    cond->Mid = 0;
    TextBufferHelper::fill(&cond->Data);
}

/*****************************************************************************
 * Are the two conditions equal or not?
 *****************************************************************************/

bool ConditionHelper::isEqual(SaHpiConditionT *c1, SaHpiConditionT *c2) {
    Report report;
    return isEqual(c1, c2, report);
}

/*****************************************************************************
 * Are the two conditions equal or not?  Include a report of the differences.
 *****************************************************************************/

bool ConditionHelper::isEqual(SaHpiConditionT *c1,
                              SaHpiConditionT *c2, Report &report) {
    bool equal = true;

    if (c1->Type != c2->Type) {
        equal = false;
        report.add("Condition->Type fields are not equal [%s, %s].",
                    HpiString::statusCondType(c1->Type), 
                    HpiString::statusCondType(c2->Type));
    }

    if (!ResourceHelper::isEqual(&c1->Entity, &c2->Entity)) {
        equal = false;
        report.add("Condition->Entity fields are not equal [%s, %s].",
                    HpiString::entityPath(&c1->Entity), 
                    HpiString::entityPath(&c2->Entity));
    }

    if ((c1->DomainId != c2->DomainId) &&
        (c1->DomainId != SAHPI_UNSPECIFIED_DOMAIN_ID) &&
        (c2->DomainId != SAHPI_UNSPECIFIED_DOMAIN_ID)) {
        equal = false;
        report.add("Condition->DomainId fields are not equal [0x%X, 0x%X].", 
                   c1->DomainId, c2->DomainId);
    }

    if (c1->ResourceId != c2->ResourceId) {
        equal = false;
        report.add("Condition->ResourceId fields are not equal [0x%X, 0x%X].", 
                   c1->ResourceId, c2->ResourceId);
    }

    if ((c1->Type == SAHPI_STATUS_COND_TYPE_SENSOR) &&
        (c2->Type == SAHPI_STATUS_COND_TYPE_SENSOR)) {
        if (c1->SensorNum != c2->SensorNum) {
               equal = false;
            report.add("Condition->SensorNum fields are not equal [0x%X, 0x%X].", 
                       c1->SensorNum, c2->SensorNum);
        }

        if (c1->EventState != c2->EventState) {
            equal = false;
            report.add("Condition->EventState fields are not equal [0x%X, 0x%X].", 
                       c1->EventState, c2->EventState);
        }
    }

    if (!NameHelper::isEqual(&c1->Name, &c2->Name)) {
        equal = false;
        report.add("Condition->Name fields are not equal [%s, %s].", 
                   HpiString::name(&c1->Name), HpiString::name(&c2->Name));
    }

    if ((c1->Type == SAHPI_STATUS_COND_TYPE_OEM) &&
        (c2->Type == SAHPI_STATUS_COND_TYPE_OEM)) {
        if (c1->Mid != c2->Mid) {
            equal = false;
            report.add("Condition->Mid fields are not equal [0x%X, 0x%X].", 
                       c1->Mid, c2->Mid);
        }
    }

    if (!TextBufferHelper::isEqual(&c1->Data, &c2->Data)) {
        equal = false;
        report.add("Condition->Data fields are not equal [%s, %s].", 
                   HpiString::textBuffer(c1->Data),
                   HpiString::textBuffer(c2->Data));
    }

    return equal;
}
