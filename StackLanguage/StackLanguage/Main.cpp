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

#include <iostream>
#include <fstream>

#include "Processor.h"
#include "ResultInfo.h"
#include "CoutPrinter.h"
#include "Settings.h"

using Settings = DefaultSettings<CoutPrinter>;
using ProcessorType = Processor<Settings>;
using EnvironmentType = typename ProcessorType::EnvironmentType;
using ProcessorStateType = typename ProcessorType::ProcessorStateType;
using PrinterType = typename EnvironmentType::PrinterType;

void breakHandler(const EnvironmentType & environment, const ProcessorStateType & state)
{
	(void)std::cin.get();
}

EnvironmentType createEnvironment(PrinterType & printer)
{
	auto result = EnvironmentType(printer);

	auto & instructions = result.getInstructions();

	instructions.add(Instruction(Opcode::Push, 15));
	instructions.add(Instruction(Opcode::Push, 30));
	instructions.add(Instruction(Opcode::PrintStack));
	instructions.add(Instruction(Opcode::Add));
	instructions.add(Instruction(Opcode::PrintStack));
	instructions.add(Instruction(Opcode::Break));

	instructions.add(Instruction(Opcode::Push, 'A'));
	instructions.add(Instruction(Opcode::PrintChar));
	instructions.add(Instruction(Opcode::Break));
	instructions.add(Instruction(Opcode::End));

	return result;
}

int mainNoArguments(void)
{
	auto printer = PrinterType();
	auto environment = createEnvironment(printer);
	auto processor = ProcessorType(environment, breakHandler);

	std::cout << "<Begin>\n";

	auto result = processor.run();

	if (result.isError())
		std::cerr << "<ERROR>: " << result.getErrorMessage();

	std::cout << "<End>\n";

	(void)std::cin.get();

	return result.isError() ? -1 : 0;
}

int mainReadFile(const char * file)
{
	auto printer = PrinterType();
	auto environment = EnvironmentType(printer);

	{
		auto inStream = std::ifstream(file, std::ios::binary | std::ios::in);

		if (inStream.fail())
			return -1;

		while (!inStream.eof())
		{
			std::uint32_t value;
			inStream.read(reinterpret_cast<char *>(&value), sizeof(value));
			environment.getInstructions().add(Instruction(value));
		}

		inStream.close();
	}

	auto processor = ProcessorType(environment, breakHandler);

	std::cout << "<Begin>\n";

	auto result = processor.run();

	if (result.isError())
		std::cerr << "<ERROR>: " << result.getErrorMessage();

	std::cout << "<End>\n";

	return result.isError() ? -1 : 0;
}

int main(int count, const char * args[])
{
	if (count == 1)
		return mainNoArguments();
	
	if (count == 2)
		return mainReadFile(args[1]);

	std::cerr << "Takes one or zero arguments\n";

	return -1;
}