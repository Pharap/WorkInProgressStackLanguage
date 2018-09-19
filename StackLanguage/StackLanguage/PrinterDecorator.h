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

#include "Utility.h"

//
// Mocks inheritance at compile time.
// Attempts to look for print and printLine functions on printer.
// If it can't find a suitable overload, it uses its own function definitions.
//

template< typename Printer >
class PrinterDecorator
{
public:
	using PrinterType = Printer;

private:
	PrinterType printer;

public:

	//
	// I'd like to figure out how to make these 'overridable' at some point
	//

	template< std::size_t size >
	void print(const char (&array)[size])
	{
		this->print(&array[0], size);
	}

	template< std::size_t size >
	void printLine(const char (&array)[size])
	{
		this->print<size>(array);
		this->printLine();
	}

	template< typename Type >
	void printMany(Type && value)
	{
		this->print(std::forward<Type>(value));
	}

	template< typename ... Args >
	void printMany(Args && ... args)
	{
		this->printMany(std::forward<Args>(args)...);
	}

	template< typename Head, typename ... Tail >
	void printMany(Head && head, Tail && ... tail)
	{
		this->printMany(std::forward<Head>(head));
		this->printMany(std::forward<Tail>(tail)...);
	}

private:

	//
	// printLine
	//

	void printLineImplementation(void)
	{
		this->print('\n');
	}

	template< typename T >
	void printLineImplementation(T value)
	{
		this->print(value);
		this->printLine();
	}

	//
	// print
	//
	
	void printImplementation(const char * string)
	{
		for(const char * pointer = string; *pointer != '\0'; ++pointer)
			this->print(*pointer);
	}

	void printImplementation(const char * string, std::size_t length)
	{
		for (std::size_t index = 0; index < length; ++index)
			this->print(string[index]);
	}

	void printImplementation(char value)
	{
		(void)value;
	}

	void printImplementation(signed char value)
	{
		this->print(static_cast<signed short>(value));
	}

	void printImplementation(signed short value)
	{
		this->print(static_cast<signed int>(value));
	}

	void printImplementation(signed int value)
	{
		this->print(static_cast<signed long>(value));
	}

	void printImplementation(signed long value)
	{
		this->print(static_cast<signed long long>(value));
	}

	void printImplementation(signed long long value)
	{
		(void)value;
	}

	void printImplementation(unsigned char value)
	{
		this->print(static_cast<unsigned short>(value));
	}

	void printImplementation(unsigned short value)
	{
		this->print(static_cast<unsigned int>(value));
	}

	void printImplementation(unsigned int value)
	{
		this->print(static_cast<unsigned long>(value));
	}

	void printImplementation(unsigned long value)
	{
		this->print(static_cast<unsigned long long>(value));
	}

	void printImplementation(unsigned long long value)
	{
		(void)value;
	}

	void printImplementation(float value)
	{
		this->print(static_cast<double>(value));
	}

	void printImplementation(double value)
	{
		this->print(static_cast<long double>(value));
	}

private:
	struct WeakMatch
	{
		constexpr WeakMatch(void) = default;
	};

	struct StrongMatch
	{
		constexpr StrongMatch(void) = default;

		constexpr operator WeakMatch(void) const
		{
			return WeakMatch();
		}
	};

	template< typename T >
	auto printDisambiguate(T value, StrongMatch)
		-> decltype(printer.print(value), void())
	{
		printer.print(value);
	}

	template< typename T >
	auto printDisambiguate(T value, WeakMatch)
		-> decltype(printImplementation(value), void())
	{
		printImplementation(value);
	}

	template< typename T, typename U >
	auto printDisambiguate(T value0, U value1, StrongMatch)
		-> decltype(printer.print(value0, value1), void())
	{
		printer.print(value0, value1);
	}

	template< typename T, typename U >
	auto printDisambiguate(T value0, U value1, WeakMatch)
		-> decltype(printImplementation(value0, value1), void())
	{
		printImplementation(value0, value1);
	}

	template< typename T >
	auto printLineDisambiguate(T value, StrongMatch unused)
		-> decltype(printer.printLine(value), void())
	{
		printer.printLine(value);
	}

	template< typename T >
	auto printLineDisambiguate(T value, WeakMatch)
		-> decltype(printLineImplementation(value), void())
	{
		printLineImplementation(value);
	}

	template< typename C >
	auto printLineDisambiguate(StrongMatch)
		-> decltype(&C::printLine, void())
	{
		printer.printLine();
	}

	auto printLineDisambiguate(WeakMatch)
		-> decltype(printLineImplementation(), void())
	{
		printLineImplementation();
	}

public:
	template< typename T >
	auto print(T value)
		-> decltype(printDisambiguate(value, StrongMatch()), void())
	{
		printDisambiguate(value, StrongMatch());
	}

	template< typename T, typename U >
	auto print(T value0, U value1)
		-> decltype(printDisambiguate(value0, value1, StrongMatch()), void())
	{
		printDisambiguate(value0, value1, StrongMatch());
	}

	template< typename T >
	auto printLine(T value)
		-> decltype(printLineDisambiguate(value, StrongMatch()), void())
	{
		printLineDisambiguate(value, StrongMatch());
	}

	auto printLine()
		-> decltype(printLineDisambiguate<Printer>(StrongMatch()), void())
	{
		printLineDisambiguate<Printer>(StrongMatch());
	}
};