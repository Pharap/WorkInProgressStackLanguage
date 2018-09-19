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

#include "Opcode.h"
#include "Instruction.h"
#include "LanguageTypes.h"
#include "Environment.h"
#include "ProcessorState.h"
#include "ResultInfo.h"

template< typename Settings >
class Processor
{
public:
	using SettingsType = typename Settings;

	using EnvironmentSettingsType = typename SettingsType::EnvironmentSettingsType;
	using ProcessorStateSettingsType = typename SettingsType::ProcessorStateSettingsType;

	using EnvironmentType = Environment<EnvironmentSettingsType>;
	using ProcessorStateType = ProcessorState<ProcessorStateSettingsType>;

	using BreakHandlerType = void(*)(const EnvironmentType &, const ProcessorStateType &);

private:
	EnvironmentType environment;
	ProcessorStateType state;
	BreakHandlerType breakHandler;

	bool running = false;
	bool completed = false;

public:
	Processor(EnvironmentType environment)
		: environment(environment), state(), breakHandler()
	{
	}

	Processor(EnvironmentType environment, BreakHandlerType breakHandler)
		: environment(environment), state(), breakHandler(breakHandler)
	{
	}

	bool isRunning(void) const
	{
		return this->running;
	}

	bool hasCompleted(void) const
	{
		return this->completed;
	}

	void start(void)
	{
		this->running = true;
		this->completed = false;
	}

	void stop(void)
	{
		this->running = false;
	}

	ResultInfo run(void)
	{
		this->start();

		while (this->isRunning())
		{
			const auto result = this->executeCycle();

			if (result.isError())
				return result;
		}

		return (this->hasCompleted()) ? resultSuccess() : resultError("Error unknown");
	}

	ResultInfo executeCycle(void)
	{
		if (this->hasCompleted())
			return resultSuccess();

		if (!this->isRunning())
			return resultError("Processor not running");

		const auto instructionPointer = this->state.getInstructionPointer();
		auto & instructions = this->environment.getInstructions();

		if (instructionPointer >= instructions.getCount())
			return resultError("Jumped to invalid address");

		const auto instruction = instructions[instructionPointer];

		this->state.incrementInstructionPointer();

		return this->execute(instruction);
	}

private:
	void complete(void)
	{
		this->running = false;
		this->completed = true;
	}

private:

	ResultInfo execute(Instruction instruction);

	ResultInfo assertDataStackSize(std::size_t amount);

	// Category 0 - Basic control
	ResultInfo executeNop(Instruction instruction);
	ResultInfo executeEnd(Instruction instruction);
	ResultInfo executeBreak(Instruction instruction);
	ResultInfo executePrintInt(Instruction instruction);
	ResultInfo executePrintChar(Instruction instruction);
	ResultInfo executePrintLine(Instruction instruction);
	ResultInfo executePrintStack(Instruction instruction);

	// Category 1 - Stack Manipulation
	ResultInfo executePush(Instruction instruction);
	ResultInfo executeDrop(Instruction instruction);
	ResultInfo executePick(Instruction instruction);
	ResultInfo executeRoll(Instruction instruction);
	ResultInfo executeDuplicate(Instruction instruction);
	ResultInfo executeSwap(Instruction instruction);
	ResultInfo executeRotate(Instruction instruction);
	ResultInfo executeOver(Instruction instruction);

	// Category 2 - Flow Control
	ResultInfo executeCall(Instruction instruction);
	ResultInfo executeCallIndirect(Instruction instruction);
	ResultInfo executeReturn(Instruction instruction);
	ResultInfo executeJumpRelative(Instruction instruction);
	ResultInfo executeJumpAbsolute(Instruction instruction);

	// Category 3 - Arithmetic
	ResultInfo executeAdd(Instruction instruction);
	ResultInfo executeAddImmediate(Instruction instruction);
	ResultInfo executeSubtract(Instruction instruction);
	ResultInfo executeSubtractImmediate(Instruction instruction);
	ResultInfo executeNegate(Instruction instruction);

	// Category 4 - Bitwise operations
	ResultInfo executeAnd(Instruction instruction);
	ResultInfo executeAndImmediate(Instruction instruction);
	ResultInfo executeOr(Instruction instruction);
	ResultInfo executeOrImmediate(Instruction instruction);
	ResultInfo executeExclusiveOr(Instruction instruction);
	ResultInfo executeExclusiveOrImmediate(Instruction instruction);
	ResultInfo executeShiftLeft(Instruction instruction);
	ResultInfo executeShiftLeftImmediate(Instruction instruction);
	ResultInfo executeShiftRight(Instruction instruction);
	ResultInfo executeShiftRightImmediate(Instruction instruction);
	ResultInfo executeNot(Instruction instruction);

	// Category 5 - Bit operations
	ResultInfo executeBitSet(Instruction instruction);
	ResultInfo executeBitClear(Instruction instruction);
	ResultInfo executeBitToggle(Instruction instruction);

	// Category 6 - Load/Store
	ResultInfo executeLoadByte(Instruction instruction);
	ResultInfo executeStoreByte(Instruction instruction);
	ResultInfo executeLoadWord(Instruction instruction);
	ResultInfo executeStoreWord(Instruction instruction);

	// Category 7 - Dynamic allocation
	ResultInfo executeMalloc(Instruction instruction);
	ResultInfo executeMallocImmediate(Instruction instruction);
	ResultInfo executeCalloc(Instruction instruction);
	ResultInfo executeCallocImmediate(Instruction instruction);
	ResultInfo executeRealloc(Instruction instruction);
	ResultInfo executeReallocImmediate(Instruction instruction);
	ResultInfo executeFree(Instruction instruction);
};

//
// Implementation
//

template< typename Settings >
ResultInfo Processor<Settings>::execute(Instruction instruction)
{
	switch (instruction.getOpcode())
	{
		// Category 0 - Basic control
	case Opcode::Nop: return executeNop(instruction);
	case Opcode::End: return executeEnd(instruction);
	case Opcode::Break: return executeBreak(instruction);
	case Opcode::PrintInt: return executePrintInt(instruction);
	case Opcode::PrintChar: return executePrintChar(instruction);
	case Opcode::PrintLine: return executePrintLine(instruction);
	case Opcode::PrintStack: return executePrintStack(instruction);

		// Category 1 - Stack Manipulation
	case Opcode::Push: return executePush(instruction);
	case Opcode::Drop: return executeDrop(instruction);
	case Opcode::Pick: return executePick(instruction);
	case Opcode::Roll: return executeRoll(instruction);
	case Opcode::Duplicate: return executeDuplicate(instruction);
	case Opcode::Swap: return executeSwap(instruction);
	case Opcode::Rotate: return executeRotate(instruction);
	case Opcode::Over: return executeOver(instruction);

		// Category 2 - Flow Control
	case Opcode::Call: return executeCall(instruction);
	case Opcode::CallIndirect: return executeCallIndirect(instruction);
	case Opcode::Return: return executeReturn(instruction);
	case Opcode::JumpRelative: return executeJumpRelative(instruction);
	case Opcode::JumpAbsolute: return executeJumpAbsolute(instruction);

		// Category 3 - Arithmetic
	case Opcode::Add: return executeAdd(instruction);
	case Opcode::AddImmediate: return executeAddImmediate(instruction);
	case Opcode::Subtract: return executeSubtract(instruction);
	case Opcode::SubtractImmediate: return executeSubtractImmediate(instruction);
	case Opcode::Negate: return executeNegate(instruction);

		// Category 4 - Bitwise operations
	case Opcode::And: return executeAnd(instruction);
	case Opcode::AndImmediate: return executeAndImmediate(instruction);
	case Opcode::Or: return executeOr(instruction);
	case Opcode::OrImmediate: return executeOrImmediate(instruction);
	case Opcode::ExclusiveOr: return executeExclusiveOr(instruction);
	case Opcode::ExclusiveOrImmediate: return executeExclusiveOrImmediate(instruction);
	case Opcode::ShiftLeft: return executeShiftLeft(instruction);
	case Opcode::ShiftLeftImmediate: return executeShiftLeftImmediate(instruction);
	case Opcode::ShiftRight: return executeShiftRight(instruction);
	case Opcode::ShiftRightImmediate: return executeShiftRightImmediate(instruction);
	case Opcode::Not: return executeNot(instruction);

		// Category 5 - Bit operations
	case Opcode::BitSet: return executeBitSet(instruction);
	case Opcode::BitClear: return executeBitClear(instruction);
	case Opcode::BitToggle: return executeBitToggle(instruction);

		// Category 6 - Load/Store
	case Opcode::LoadByte: return executeLoadByte(instruction);
	case Opcode::StoreByte: return executeStoreByte(instruction);
	case Opcode::LoadWord: return executeLoadWord(instruction);
	case Opcode::StoreWord: return executeStoreWord(instruction);

		// Category 7 - Dynamic allocation
	case Opcode::Malloc: return executeMalloc(instruction);
	case Opcode::MallocImmediate: return executeMallocImmediate(instruction);
	case Opcode::Calloc: return executeCalloc(instruction);
	case Opcode::CallocImmediate: return executeCallocImmediate(instruction);
	case Opcode::Free: return executeFree(instruction);

	default: return resultError("Unrecognised opcode");
	}
}

template< typename Settings >
ResultInfo Processor<Settings>::assertDataStackSize(std::size_t amount)
{
	if (this->state.getDataStack().getCount() < amount)
		return resultError("Data stack underflow");

	return resultSuccess();
}

//
// Category 0 - Basic control
//

template< typename Settings >
ResultInfo Processor<Settings>::executeNop(Instruction instruction)
{
	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeEnd(Instruction instruction)
{
	this->complete();
	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeBreak(Instruction instruction)
{
	if (this->breakHandler != nullptr)
		this->breakHandler(this->environment, this->state);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executePrintInt(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	const Word word = this->state.getDataStack().peek();
	this->environment.getPrinter().print(word);
	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executePrintChar(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	const Word word = this->state.getDataStack().peek();
	this->environment.getPrinter().print(static_cast<char>(word));
	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executePrintLine(Instruction instruction)
{
	this->environment.getPrinter().printLine();
	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executePrintStack(Instruction instruction)
{
	auto & printer = this->environment.getPrinter();

	printer.print('[');

	const auto & stack = this->state.getDataStack();
	if (!stack.isEmpty())
	{
		printer.print(stack[0]);

		for (std::uint8_t i = 1; i < stack.getCount(); ++i)
			printer.printMany(", ", stack[i]);
	}

	printer.printLine(']');

	return resultSuccess();
}



//
// Category 1 - Stack Manipulation
//
template< typename Settings >
ResultInfo Processor<Settings>::executePush(Instruction instruction)
{
	if (this->state.getDataStack().getCount() >= this->state.getDataStack().getCapacity())
		return resultError("Data stack overflow");

	this->state.getDataStack().push(instruction.getOperand());

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeDrop(Instruction instruction)
{
	const Word dropCount = instruction.getOperand();

	const ResultInfo resultInfo = assertDataStackSize(dropCount);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	for (std::size_t i = 0; i < dropCount; ++i)
		this->state.getDataStack().drop();

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executePick(Instruction instruction)
{
	const Word offset = instruction.getOperand() + 1;

	const ResultInfo resultInfo = assertDataStackSize(offset);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const auto end = stack.getCount() - 1;
	const auto index = (end - offset);
	const auto element = stack[index];

	stack.push(element);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeRoll(Instruction instruction)
{
	const auto offset = instruction.getOperand();

	const ResultInfo resultInfo = assertDataStackSize(offset);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const auto end = stack.getCount() - 1;
	const auto index = (end - offset);
	const auto element = stack[index];

	stack.removeAt(index);
	stack.push(element);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeDuplicate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const auto value = stack.peek();

	stack.push(value);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeSwap(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	/*const auto end = this->state.getDataStack().end();
	const auto ap = (end - 1);
	const auto bp = (end - 2);
	std::swap(*ap, *bp);*/

	auto & stack = this->state.getDataStack();

	const Word a = stack.peek();
	stack.drop();

	const Word b = stack.peek();
	stack.drop();

	stack.push(a);
	stack.push(b);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeRotate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(3);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word a = stack.peek();
	stack.drop();

	const Word b = stack.peek();
	stack.drop();

	const Word c = stack.peek();
	stack.drop();

	stack.push(b);
	stack.push(a);
	stack.push(c);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeOver(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word a = stack.peek();
	stack.drop();

	const Word b = stack.peek();

	stack.push(a);
	stack.push(b);

	return resultSuccess();
}



//
// Category 2 - Flow Control
//

template< typename Settings >
ResultInfo Processor<Settings>::executeCall(Instruction instruction)
{
	const Word address = instruction.getOperand();

	this->state.functionCall(address);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeCallIndirect(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word address = stack.peek();
	stack.drop();

	this->state.functionCall(address);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeReturn(Instruction instruction)
{
	if (this->state.getReturnStack().isEmpty())
		return resultError("Call stack underflow");

	this->state.functionReturn();

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeJumpRelative(Instruction instruction)
{
	const SWord offset = instruction.getSignedOperand();

	this->state.jumpRelative(offset);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeJumpAbsolute(Instruction instruction)
{
	const Word address = instruction.getOperand();

	this->state.jumpAbsolute(address);

	return resultSuccess();
}



//
// Category 3 - Arithmetic
//
template< typename Settings >
ResultInfo Processor<Settings>::executeAdd(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	stack.peek() += value;

	/*const Word b = stack.peek();
	stack.drop();

	const Word a = stack.peek();
	stack.drop();

	const Word result = a + b;
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeAddImmediate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	stack.peek() += instruction.getOperand();

	/*const Word value = stack.peek();
	stack.drop();

	const Word result = value + instruction.getOperand();
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeSubtract(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	stack.peek() -= value;

	/*const Word b = stack.peek();
	stack.drop();

	const Word a = stack.peek();
	stack.drop();

	const Word result = a - b;
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeSubtractImmediate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	stack.peek() -= instruction.getOperand();

	/*const Word value = stack.peek();
	stack.drop();

	const Word result = value - instruction.getOperand();
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeNegate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const SWord value = static_cast<SWord>(stack.peek());
	const SWord result = -value;
	stack.peek() = static_cast<Word>(result);

	/*const SWord value = static_cast<SWord>(stack.peek());
	stack.drop();

	const SWord result = -value;
	stack.push(static_cast<Word>(result));*/

	return resultSuccess();
}



//
// Category 4 - Bitwise operations
//
template< typename Settings >
ResultInfo Processor<Settings>::executeAnd(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	stack.peek() &= value;

	/*const Word b = stack.peek();
	stack.drop();

	const Word a = stack.peek();
	stack.drop();

	const Word result = a & b;
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeAndImmediate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	stack.peek() &= instruction.getOperand();

	/*const Word value = stack.peek();
	stack.drop();

	const Word result = value & instruction.getOperand();
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeOr(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	stack.peek() |= value;

	/*const Word b = stack.peek();
	stack.drop();

	const Word a = stack.peek();
	stack.drop();

	const Word result = a | b;
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeOrImmediate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	stack.peek() |= instruction.getOperand();

	/*const Word value = stack.peek();
	stack.drop();

	const Word result = value | instruction.getOperand();
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeExclusiveOr(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	stack.peek() ^= value;

	/*const Word b = stack.peek();
	stack.drop();

	const Word a = stack.peek();
	stack.drop();

	const Word result = a ^ b;
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeExclusiveOrImmediate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	stack.peek() ^= instruction.getOperand();

	/*const Word value = stack.peek();
	stack.drop();

	const Word result = value ^ instruction.getOperand();
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeShiftLeft(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	stack.peek() <<= value;

	/*const Word b = stack.peek();
	stack.drop();

	const Word a = stack.peek();
	stack.drop();

	const Word result = a << b;
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeShiftLeftImmediate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	stack.peek() <<= instruction.getOperand();

	/*const Word value = stack.peek();
	stack.drop();

	const Word result = value << instruction.getOperand();
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeShiftRight(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	stack.peek() >>= value;

	/*const Word b = stack.peek();
	stack.drop();

	const Word a = stack.peek();
	stack.drop();

	const Word result = a >> b;
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeShiftRightImmediate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	stack.peek() >>= instruction.getOperand();

	/*const Word value = stack.peek();
	stack.drop();

	const Word result = value >> instruction.getOperand();
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeNot(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	stack.peek() = ~(stack.peek());

	/*const Word value = stack.peek();
	stack.drop();

	const Word result = ~value;
	stack.push(result);*/

	return resultSuccess();
}



//
// Category 5 - Bit operations
//
template< typename Settings >
ResultInfo Processor<Settings>::executeBitSet(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	stack.peek() |= (1 << value);

	/*const Word b = stack.peek();
	stack.drop();

	const Word a = stack.peek();
	stack.drop();

	const Word result = a | (1 << b);
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeBitClear(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	stack.peek() &= ~(1 << value);

	/*const Word b = stack.peek();
	stack.drop();

	const Word a = stack.peek();
	stack.drop();

	const Word result = a & ~(1 << b);
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeBitToggle(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	stack.peek() ^= (1 << value);

	/*const Word b = stack.peek();
	stack.drop();

	const Word a = stack.peek();
	stack.drop();

	const Word result = a ^ (1 << b);
	stack.push(result);*/

	return resultSuccess();
}



//
// Category 6 - Load/Store
//
template< typename Settings >
ResultInfo Processor<Settings>::executeLoadByte(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word address = stack.peek();
	const Byte * pointer = reinterpret_cast<const Byte *>(address);
	stack.peek() = *pointer;

	/*const Word address = stack.peek();
	stack.drop();

	const Byte * pointer = reinterpret_cast<const Byte *>(address);

	const Word result = *pointer;
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeStoreByte(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	const Word address = stack.peek();
	stack.drop();

	Byte * pointer = reinterpret_cast<Byte *>(address);

	*pointer = value;

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeLoadWord(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word address = stack.peek();
	const Word * pointer = reinterpret_cast<const Word *>(address);
	stack.peek() = *pointer;

	/*const Word address = stack.peek();
	stack.drop();

	const Word * pointer = reinterpret_cast<const Word *>(address);

	const Word result = *pointer;
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeStoreWord(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	const Word address = stack.peek();
	stack.drop();

	Word * pointer = reinterpret_cast<Word *>(address);

	*pointer = value;

	return resultSuccess();
}



//
// Category 7 - Dynamic allocation
//
template< typename Settings >
ResultInfo Processor<Settings>::executeMalloc(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word size = stack.peek();

	const void * data = std::malloc(size);

	stack.peek() = reinterpret_cast<Word>(data);

	/*const Word size = stack.peek();
	stack.drop();

	void * raw = std::malloc(size);
	const char * data = reinterpret_cast<const char*>(raw);

	const Word result = reinterpret_cast<Word>(data);
	stack.push(result);*/

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeMallocImmediate(Instruction instruction)
{
	// Todo: assert stack overflow

	auto & stack = this->state.getDataStack();

	const Word size = instruction.getOperand();

	void * raw = std::malloc(size);
	const char * data = reinterpret_cast<const char*>(raw);

	const Word result = reinterpret_cast<Word>(data);
	stack.push(result);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeCalloc(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word count = stack.peek();
	stack.drop();

	const Word size = stack.peek();
	stack.drop();

	void * raw = std::calloc(count, size);
	const char * data = reinterpret_cast<const char*>(raw);

	const Word result = reinterpret_cast<Word>(data);
	stack.push(result);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeCallocImmediate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word count = stack.peek();
	stack.drop();

	const Word size = instruction.getOperand();

	void * raw = std::calloc(count, size);
	const char * data = reinterpret_cast<const char*>(raw);

	const Word result = reinterpret_cast<Word>(data);
	stack.push(result);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeRealloc(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(2);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word size = stack.peek();
	stack.drop();

	const Word address = stack.peek();
	stack.drop();

	void * pointer = reinterpret_cast<void *>(address);
	const char * data = reinterpret_cast<const char *>(std::realloc(pointer, size));

	const Word result = reinterpret_cast<Word>(data);
	stack.push(result);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeReallocImmediate(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word size = instruction.getOperand();

	const Word address = stack.peek();
	stack.drop();

	void * pointer = reinterpret_cast<void *>(address);
	const char * data = reinterpret_cast<const char *>(std::realloc(pointer, size));

	const Word result = reinterpret_cast<Word>(data);
	stack.push(result);

	return resultSuccess();
}

template< typename Settings >
ResultInfo Processor<Settings>::executeFree(Instruction instruction)
{
	const ResultInfo resultInfo = assertDataStackSize(1);
	if (resultInfo.getStatus() == ResultStatus::Error)
		return resultInfo;

	auto & stack = this->state.getDataStack();

	const Word value = stack.peek();
	stack.drop();

	char * data = reinterpret_cast<char *>(value);

	std::free(data);

	return resultSuccess();
}