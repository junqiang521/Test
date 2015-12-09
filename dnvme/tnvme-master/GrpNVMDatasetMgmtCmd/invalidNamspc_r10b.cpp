/*
 * Copyright (c) 2011, Intel Corporation.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <boost/format.hpp>
#include "invalidNamspc_r10b.h"
#include "globals.h"
#include "grpDefs.h"
#include "../Queues/iocq.h"
#include "../Queues/iosq.h"
#include "../Utils/io.h"
#include "../Cmds/datasetMgmt.h"

namespace GrpNVMDatasetMgmtCmd {


InvalidNamspc_r10b::InvalidNamspc_r10b(
    string grpName, string testName) :
    Test(grpName, testName, SPECREV_10b)
{
    // 63 chars allowed:     xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    mTestDesc.SetCompliance("revision 1.0b, section 4,6");
    mTestDesc.SetShort(     "Issue dataset mgmt and cause SC=Invalid Namspc or Format");
    // No string size limit for the long description
    mTestDesc.SetLong(
        "Determine Identify.NN and issue dataset mgmt cmd to all namspcs not "
        "supported by DUT, expect failure.");
}


InvalidNamspc_r10b::~InvalidNamspc_r10b()
{
    ///////////////////////////////////////////////////////////////////////////
    // Allocations taken from the heap and not under the control of the
    // RsrcMngr need to be freed/deleted here.
    ///////////////////////////////////////////////////////////////////////////
}


InvalidNamspc_r10b::
InvalidNamspc_r10b(const InvalidNamspc_r10b &other) : Test(other)
{
    ///////////////////////////////////////////////////////////////////////////
    // All pointers in this object must be NULL, never allow shallow or deep
    // copies, see Test::Clone() header comment.
    ///////////////////////////////////////////////////////////////////////////
}


InvalidNamspc_r10b &
InvalidNamspc_r10b::operator=(const InvalidNamspc_r10b &other)
{
    ///////////////////////////////////////////////////////////////////////////
    // All pointers in this object must be NULL, never allow shallow or deep
    // copies, see Test::Clone() header comment.
    ///////////////////////////////////////////////////////////////////////////
    Test::operator=(other);
    return *this;
}


Test::RunType
InvalidNamspc_r10b::RunnableCoreTest(bool preserve)
{
    ///////////////////////////////////////////////////////////////////////////
    // All code contained herein must never permanently modify the state or
    // configuration of the DUT. Permanence is defined as state or configuration
    // changes that will not be restored after a cold hard reset.
    ///////////////////////////////////////////////////////////////////////////

    ConstSharedIdentifyPtr idCtrlrCap = gInformative->GetIdentifyCmdCtrlr();
    uint64_t oncs = idCtrlrCap->GetValue(IDCTRLRCAP_ONCS);
    if ((oncs & ONCS_SUP_DSM_CMD) == 0)
        return RUN_FALSE;

    preserve = preserve;    // Suppress compiler error/warning
    return RUN_TRUE;        // This test is never destructive
}


void
InvalidNamspc_r10b::RunCoreTest()
{
    /** \verbatim
     * Assumptions:
     * 1) Test CreateResources_r10b has run prior.
     * \endverbatim
     */
    uint64_t inc, i;
    string work;
    ConstSharedIdentifyPtr namSpcPtr;

    // Lookup objs which were created in a prior test within group
    SharedIOSQPtr iosq = CAST_TO_IOSQ(gRsrcMngr->GetObj(IOSQ_GROUP_ID));
    SharedIOCQPtr iocq = CAST_TO_IOCQ(gRsrcMngr->GetObj(IOCQ_GROUP_ID));

    SharedDatasetMgmtPtr datasetMgmtCmd =
        SharedDatasetMgmtPtr(new DatasetMgmt());

    LOG_NRM("Create memory to contain 1 dataset range def");
    SharedMemBufferPtr rangeMem = SharedMemBufferPtr(new MemBuffer());
    rangeMem->Init(sizeof(RangeDef), true);
    send_64b_bitmask prpReq =
        (send_64b_bitmask)(MASK_PRP1_PAGE | MASK_PRP2_PAGE);
    datasetMgmtCmd->SetPrpBuffer(prpReq, rangeMem);

    LOG_NRM("Setup generic range def guidelines");
    RangeDef *rangeDef = (RangeDef *)rangeMem->GetBuffer();
    rangeDef->length = 1;

    // For all namspc's issue cmd to an illegal namspc
    ConstSharedIdentifyPtr idCtrlrStruct = gInformative->GetIdentifyCmdCtrlr();
    uint32_t nn = (uint32_t)idCtrlrStruct->GetValue(IDCTRLRCAP_NN);
    for (i = (nn + 1), inc = 1; i <= 0xffffffff; i += (2 * inc), inc += 1327) {

        LOG_NRM("Issue dataset mgmt cmd with illegal namspc ID=%llu",
            (unsigned long long)i);
        datasetMgmtCmd->SetNSID(i);
        work = str(boost::format("namspc%d") % i);
        IO::SendAndReapCmd(mGrpName, mTestName, CALC_TIMEOUT_ms(1), iosq,
            iocq, datasetMgmtCmd, work, true, CESTAT_INVAL_NAMSPC);
    }
}


}   // namespace

