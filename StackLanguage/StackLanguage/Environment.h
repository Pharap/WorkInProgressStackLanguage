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

#include "Instruction.h"
#include "List.h"

template< typename Settings >
class Environment
{
public:
	using SettingsType = typename Settings;

public:
	static constexpr std::size_t InstructionListSize = SettingsType::InstructionListSize;

public:
	using PrinterType = typename SettingsType::PrinterType;
	using InstructionListType = List<Instruction, InstructionListSize>;

private:
	PrinterType & printer;
	InstructionListType instructions;

public:

public:
	//Environment(void) = default;
	Environment(PrinterType & printer)
		: printer(printer)
	{
	}

public:
	PrinterType & getPrinter(void)
	{
		return this->printer;
	}

	const PrinterType & getPrinter(void) const
	{
		return this->printer;
	}

	InstructionListType & getInstructions(void)
	{
		return this->instructions;
	}

	const InstructionListType & getInstructions(void) const
	{
		return this->instructions;
	}
};