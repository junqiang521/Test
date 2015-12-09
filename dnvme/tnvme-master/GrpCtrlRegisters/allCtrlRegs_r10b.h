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

#ifndef _ALLCTRLREGS_r10b_H_
#define _ALLCTRLREGS_r10b_H_

#include "test.h"

namespace GrpCtrlRegisters {


/** \verbatim
 * -----------------------------------------------------------------------------
 * ----------------Mandatory rules for children to follow-----------------------
 * -----------------------------------------------------------------------------
 * 1) See notes in the header file of the Test base class
 * \endverbatim
 */
class AllCtrlRegs_r10b : virtual public Test
{
public:
    AllCtrlRegs_r10b(string grpName, string testName);
    virtual ~AllCtrlRegs_r10b();

    /**
     * IMPORTANT: Read Test::Clone() header comment.
     */
    virtual AllCtrlRegs_r10b *Clone() const
        { return new AllCtrlRegs_r10b(*this); }
    AllCtrlRegs_r10b &operator=(const AllCtrlRegs_r10b &other);
    AllCtrlRegs_r10b(const AllCtrlRegs_r10b &other);


protected:
    virtual void RunCoreTest();
    virtual RunType RunnableCoreTest(bool preserve);

    /**
     * Validate the specified ctrl'r register RO bits report correct values if
     * and only if they are not vendor specific.
     * @param reg Pass the register to validate
     * @return returns upon success, otherwise throws exception
     */
    virtual void ValidateCtlRegisterROAttribute(CtlSpc reg);

    /**
     * Validate all the registers have default values being reported for
     * the RO bits which are not vendor specific.
     * @return returns upon success, otherwise throws exception
     */
    virtual void ValidateDefaultValues();

    /**
     * Validate all the registers hare RO after attempting to write to them.
     * @return returns upon success, otherwise throws exception
     */
    virtual void ValidateROBitsAfterWriting();

private:
    ///////////////////////////////////////////////////////////////////////////
    // Adding a member variable? Then edit the copy constructor and operator=().
    ///////////////////////////////////////////////////////////////////////////
};

}   // namespace

#endif
