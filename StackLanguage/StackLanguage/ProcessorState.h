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

#include "Stack.h"
#include "LanguageTypes.h"

template< typename Settings >
class ProcessorState
{
public:
	using SettingsType = typename Settings;

public:
	static constexpr std::size_t DataStackSize = SettingsType::DataStackSize;
	static constexpr std::size_t ReturnStackSize = SettingsType::ReturnStackSize;

public:
	using DataStack = Stack<Word, DataStackSize>;
	using ReturnStack = Stack<Address, ReturnStackSize>;

private:
	DataStack dataStack = DataStack();
	ReturnStack returnStack = ReturnStack();
	Address instructionPointer = 0;

public:

	DataStack & getDataStack(void)
	{
		return this->dataStack;
	}

	const DataStack & getDataStack(void) const
	{
		return this->dataStack;
	}

	ReturnStack & getReturnStack(void)
	{
		return this->returnStack;
	}

	const ReturnStack & getReturnStack(void) const
	{
		return this->returnStack;
	}

	const Address & getInstructionPointer(void) const
	{
		return this->instructionPointer;
	}

	void incrementInstructionPointer(void)
	{
		++this->instructionPointer;
	}

	void functionCall(Address address)
	{
		this->returnStack.push(this->instructionPointer);
		this->instructionPointer = address;
	}

	void functionReturn(void)
	{
		this->instructionPointer = this->returnStack.peek();
		this->returnStack.drop();
	}

	void jumpAbsolute(Address address)
	{
		this->instructionPointer = address;
	}

	void jumpRelative(AddressOffset addressOffset)
	{
		this->instructionPointer += addressOffset;
	}
};