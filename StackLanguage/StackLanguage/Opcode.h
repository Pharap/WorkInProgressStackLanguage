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

#include <cstdint>

enum class Opcode : std::uint8_t
{
	// Category 0 - Basic control
	Nop = 0x00,
	End = 0x01,
	Break = 0x02,
	PrintInt = 0x03,
	PrintChar = 0x04,
	PrintLine = 0x05,
	PrintStack = 0x06,

	// Category 1 - Stack Manipulation
	Push = 0x10,
	Drop = 0x11,
	Pick = 0x12,
	Roll = 0x13,
	Duplicate = 0x14, 
	Swap = 0x15,
	Rotate = 0x16, 
	Over = 0x17, 

	// DUP = PICK(0)
	// OVER = PICK(1)
	// SWAP = ROLL(1)
	// ROT = ROLL(2)

	// Category 2 - Flow Control
	Call = 0x20,
	CallIndirect = 0x21,
	Return = 0x22,
	JumpRelative = 0x23,
	JumpAbsolute = 0x24,

	// Category 3 - Arithmetic
	Add = 0x30,
	AddImmediate = 0x31,
	Subtract = 0x32,
	SubtractImmediate = 0x33,
	Negate = 0x34,

	// Category 4 - Bitwise operations
	And = 0x40,
	AndImmediate = 0x41,
	Or = 0x42,
	OrImmediate = 0x43,
	ExclusiveOr = 0x44,
	ExclusiveOrImmediate = 0x45,
	ShiftLeft = 0x46,
	ShiftLeftImmediate = 0x47,
	ShiftRight = 0x48,
	ShiftRightImmediate = 0x49,
	Not = 0x4A,

	// Category 5 - Bit operations
	BitSet = 0x50,
	BitClear = 0x51,
	BitToggle = 0x52,

	// Category 6 - Load/Store
	LoadByte = 0x60,
	LoadWord = 0x61,
	StoreByte = 0x62,
	StoreWord = 0x63,
	StoreByteImmediate = 0x64,
	StoreWordImmediate = 0x65,

	//LoadByteIndexed,
	//LoadWordIndexed,
	
	//StoreWordIndexed,
	//StoreByteIndexed,

	// Category 7 - Dynamic allocation
	Malloc = 0x70,
	MallocImmediate = 0x71,
	Calloc = 0x72,
	CallocImmediate = 0x73,
	Realloc = 0x74,
	ReallocImmediate = 0x75,
	Free = 0x76,
};
