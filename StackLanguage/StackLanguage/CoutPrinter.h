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

#include <iostream>

class CoutPrinter
{
public:
	void print(char character)
	{
		std::cout << character;
	}

	void print(const char * nullString)
	{
		std::cout << nullString;
	}

	void print(signed char value)
	{
		std::cout << value;
	}

	void print(signed short value)
	{
		std::cout << value;
	}

	void print(signed int value)
	{
		std::cout << value;
	}

	void print(signed long value)
	{
		std::cout << value;
	}

	void print(signed long long value)
	{
		std::cout << value;
	}

	void print(unsigned char value)
	{
		std::cout << value;
	}

	void print(unsigned short value)
	{
		std::cout << value;
	}

	void print(unsigned int value)
	{
		std::cout << value;
	}

	void print(unsigned long value)
	{
		std::cout << value;
	}

	void print(unsigned long long value)
	{
		std::cout << value;
	}

	void print(float value)
	{
		std::cout << value;
	}

	void print(double value)
	{
		std::cout << value;
	}

	void print(long double value)
	{
		std::cout << value;
	}

	void printLine(void)
	{
		std::cout << '\n';
	}
};
