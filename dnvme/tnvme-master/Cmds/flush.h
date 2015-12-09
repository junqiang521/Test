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

#ifndef _FLUSH_H_
#define _FLUSH_H_

#include "cmd.h"


class Flush;    // forward definition
typedef boost::shared_ptr<Flush>             SharedFlushPtr;
typedef boost::shared_ptr<const Flush>       ConstSharedFlushPtr;
#define CAST_TO_FLUSH(shared_trackable_ptr)  \
        boost::dynamic_pointer_cast<Flush>(shared_trackable_ptr);


/**
* This class implements the flush admin cmd
*
* @note This class may throw exceptions.
*/
class Flush : public Cmd
{
public:
    Flush();
    virtual ~Flush();

    /// Used to compare for NULL pointers being returned by allocations
    static SharedFlushPtr NullFlushPtr;
    static const uint8_t Opcode;
};


#endif
