// ExpLog.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "..\..\src\SFPMLib.h"

using namespace sfpmlib;

template <class T>
void LogFP(T start, T end, T increment)
{
	std::string s("LogFP");
	if (sizeof(T::IntegerPart) == 1)
	{
		s += "Small";
	}
	else if (sizeof(T::IntegerPart) == 2)
	{
		s += "Medium";
	}
	else
	{
		s += "Large";
	}
	s += ".txt";

	FILE* f = fopen(s.c_str(), "w");
	if (f)
	{
		for(T arg = start; arg < end; arg += increment)
		{
			T val = T::Log(arg, T(false, 10, 0));
			fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
		}

		fclose(f);
	}
}

template <class T>
void ExpFP(T start, T end, T increment)
{
	std::string s("ExpFP");
	if (sizeof(T::IntegerPart) == 1)
	{
		s += "Small";
	}
	else if (sizeof(T::IntegerPart) == 2)
	{
		s += "Medium";
	}
	else
	{
		s += "Large";
	}
	s += ".txt";

	FILE* f = fopen(s.c_str(), "w");
	if (f)
	{
		for(T arg = start; arg < end; arg += increment)
		{
			T val = T::Exp(arg);
			fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
		}

		fclose(f);
	}
}

template <class T>
void ParabolicFP(T start, T end, T increment)
{
	std::string s("ParabolicFP");
	if (sizeof(T::IntegerPart) == 1)
	{
		s += "Small";
	}
	else if (sizeof(T::IntegerPart) == 2)
	{
		s += "Medium";
	}
	else
	{
		s += "Large";
	}
	s += ".txt";

	FILE* f = fopen(s.c_str(), "w");
	if (f)
	{
		for(T arg = start; arg < end; arg += increment)
		{
			T val = T::Pow(arg, 2);
			fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
		}

		fclose(f);
	}
}

int main()
{
	// 8.8 fixed point numbers
	FixedPointSmall startSmall = FixedPointSmall::FromString("0.005");
	FixedPointSmall endSmall = FixedPointSmall::FromString("200.0");
	FixedPointSmall incrementSmall = FixedPointSmall::FromString("0.1");

	std::cout << "Test Log with 8.8 fixed point numbers" << std::endl;
	LogFP(startSmall, endSmall, incrementSmall);
	std::cout << "Done" << std::endl;

	startSmall = FixedPointSmall::FromString("-5.0");
	endSmall = FixedPointSmall::FromString("5.0");
	incrementSmall = FixedPointSmall::FromString("0.01");
	std::cout << "Test Exp with 8.8 fixed point numbers" << std::endl;
	ExpFP(startSmall, endSmall, incrementSmall);
	std::cout << "Done" << std::endl;

	startSmall = FixedPointSmall::FromString("-15.5");
	endSmall = FixedPointSmall::FromString("15.5");
	incrementSmall = FixedPointSmall::FromString("0.01");
	std::cout << "Test x^2 with 8.8 fixed point numbers" << std::endl;
	ParabolicFP(startSmall, endSmall, incrementSmall);
	std::cout << "Done" << std::endl;

	// 16.16 fixed point numbers
	FixedPointMedium startMedium = FixedPointMedium::FromString("0.0001");
	FixedPointMedium endMedium = FixedPointMedium::FromString("10000.0");
	FixedPointMedium incrementMedium = FixedPointMedium::FromString("0.1");

	std::cout << "Test Log with 16.16 fixed point numbers" << std::endl;
	LogFP(startMedium, endMedium, incrementMedium);
	std::cout << "Done" << std::endl;

	startMedium = FixedPointMedium::FromString("-5.0");
	endMedium = FixedPointMedium::FromString("5.0");
	incrementMedium = FixedPointMedium::FromString("0.01");
	std::cout << "Test Exp with 16.16 fixed point numbers" << std::endl;
	ExpFP(startMedium, endMedium, incrementMedium);
	std::cout << "Done" << std::endl;

	startMedium = FixedPointMedium::FromString("-100.0");
	endMedium = FixedPointMedium::FromString("100.0");
	incrementMedium = FixedPointMedium::FromString("0.01");
	std::cout << "Test x^2 with 16.16 fixed point numbers" << std::endl;
	ParabolicFP(startMedium, endMedium, incrementMedium);
	std::cout << "Done" << std::endl;

	// 32.32 fixed point numbers
	FixedPointLarge startLarge = FixedPointLarge::FromString("0.0001");
	FixedPointLarge endLarge = FixedPointLarge::FromString("10000.0");
	FixedPointLarge incrementLarge = FixedPointLarge::FromString("0.1");

	std::cout << "Test Log with 32.32 fixed point numbers" << std::endl;
	LogFP(startLarge, endLarge, incrementLarge);
	std::cout << "Done" << std::endl;

	startLarge = FixedPointLarge::FromString("-5.0");
	endLarge = FixedPointLarge::FromString("5.0");
	incrementLarge = FixedPointLarge::FromString("0.01");
	std::cout << "Test Exp with 32.32 fixed point numbers" << std::endl;
	ExpFP(startLarge, endLarge, incrementLarge);
	std::cout << "Done" << std::endl;

	startLarge = FixedPointLarge::FromString("-100.0");
	endLarge = FixedPointLarge::FromString("100.0");
	incrementLarge = FixedPointLarge::FromString("0.01");
	std::cout << "Test x^2 with 16.16 fixed point numbers" << std::endl;
	ParabolicFP(startLarge, endLarge, incrementLarge);
	std::cout << "Done" << std::endl;
}
