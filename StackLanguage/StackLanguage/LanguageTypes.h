#pragma once

//
//   Copyright (C) 2018 Pharap (@Pharap)
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//

#include "StdInt.h"

/*using Byte = unsigned char;
using Word = unsigned int;
using SWord = signed int;*/

using Byte = std::uint8_t;
using Word = std::uint32_t;
using SWord = std::int32_t;

using Address = Word;
using AddressOffset = SWord;

static_assert(sizeof(Address) >= sizeof(void *), "Address isn't large enough to store a pointer");
static_assert(sizeof(Address) == sizeof(Word), "Address isn't the same size as Word");