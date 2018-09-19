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

#include "LanguageTypes.h"
#include "Opcode.h"

#include <cstdint>

class Instruction
{
private:
	constexpr static const std::uint32_t opcodeMask = 0x000000FFu;
	constexpr static const std::uint32_t opcodeShift = 24;

	constexpr static const std::uint32_t operandMask = 0x00FFFFFFu;
	constexpr static const std::uint32_t operandShift = 0;

private:
	std::uint32_t value = 0;

	constexpr static std::int32_t signExtend(std::uint32_t value)
	{
		return static_cast<std::int32_t>((((value & 0x00800000u) != 0) ? (value | 0xFF000000u) : (value & 0x007FFFFFu)));
	}

	constexpr static std::uint32_t signReduce(std::int32_t value)
	{
		return (static_cast<std::uint32_t>(value) & 0x00FFFFFFu);
	}

	constexpr static std::uint32_t createInstructionValue(Opcode opcode)
	{
		return ((static_cast<std::uint32_t>(opcode) & opcodeMask) << opcodeShift);
	}

	constexpr static std::uint32_t createInstructionValue(Opcode opcode, Word value)
	{
		return (((static_cast<std::uint32_t>(opcode) & opcodeMask) << opcodeShift) | ((value & operandMask) << operandShift));
	}

	constexpr static std::uint32_t createInstructionValue(Opcode opcode, SWord value)
	{
		return (((static_cast<std::uint32_t>(opcode) & opcodeMask) << opcodeShift) | ((signExtend(static_cast<std::uint32_t>(value)) & operandMask) << operandShift));
	}

public:
	constexpr Instruction(void) = default;

	constexpr explicit Instruction(std::uint32_t value)
		: value(value)
	{
	}

	constexpr explicit Instruction(Opcode opcode)
		: value(createInstructionValue(opcode))
	{
	}

	constexpr explicit Instruction(Opcode opcode, Word value)
		: value(createInstructionValue(opcode, value))
	{
	}

	constexpr explicit Instruction(Opcode opcode, SWord value)
		: value(createInstructionValue(opcode, value))
	{
	}

	constexpr Opcode getOpcode(void) const
	{
		return static_cast<Opcode>((this->value >> opcodeShift) & opcodeMask);
	}

	constexpr Word getOperand(void) const
	{
		return ((this->value >> operandShift) & operandMask);
	}

	constexpr SWord getSignedOperand(void) const
	{
		return signExtend((this->value >> operandShift) & operandMask);
	}
};