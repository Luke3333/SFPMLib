// Trigonometrics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "..\..\src\SFPMLib.h"

using namespace sfpmlib;

template <class T>
void CosineFP(T startAngle, T endAngle, T increment)
{
	std::string s("CosineFP");
	if (sizeof(T::IntegerPart) == 1)
	{
		s += "Short";
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
		for(T arg = startAngle; arg < endAngle; arg += increment)
		{
			T val = T::Cos(arg);
			fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
		}
		fclose(f);
	}
}

template <class T>
void SineFP(T startAngle, T endAngle, T increment)
{
	std::string s("SineFP");
	if (sizeof(T::IntegerPart) == 1)
	{
		s += "Short";
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
		for(T arg = startAngle; arg < endAngle; arg += increment)
		{
			T val = T::Sin(arg);
			fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
		}
		fclose(f);
	}
}

template <class T>
void TanFP(T startAngle, T endAngle, T increment)
{
	static T pi_2(T::FromString("1.570796326"));
	static T pi(T::FromString("3.141592653"));
	static T pi_3_2(T::FromString("4.712388980"));
	static T pi2(T::FromString("6.283185307"));

	std::string s("TanFP");
	if (sizeof(T::IntegerPart) == 1)
	{
		s += "Short";
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
		for(T arg = startAngle; arg < endAngle; arg += increment)
		{
			try
			{
				T val = T::Tan(arg);
				fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
			}
			catch(FPException&)
			{
				if (arg <= pi_2)
				{
					fprintf(f, "arg = %s - Tan(arg) = %s\n", arg.ToString().c_str(), "inf");
				}
				else if (arg <= pi)
				{
					fprintf(f, "arg = %s - Tan(arg) = %s\n", arg.ToString().c_str(), "-inf");
				}
				else if (arg <= pi_3_2)
				{
					fprintf(f, "arg = %s - Tan(arg) = %s\n", arg.ToString().c_str(), "inf");
				}
				else
				{
					fprintf(f, "arg = %s - Tan(arg) = %s\n", arg.ToString().c_str(), "-inf");
				}
			}
		}
		fclose(f);
	}
}

int main()
{
	// 8.8 fixed point numbers
	FixedPointSmall startAngleSmall = FixedPointSmall::FromString("0.0");
	FixedPointSmall endAngleSmall = FixedPointSmall::FromString("6.283185307");
	FixedPointSmall incrementSmall = FixedPointSmall::FromString("0.005");

	std::cout << "Test Cosine with 8.8 fixed point numbers" << std::endl;
	CosineFP(startAngleSmall, endAngleSmall, incrementSmall);
	std::cout << "Done" << std::endl;

	std::cout << "Test Sine with 8.8 fixed point numbers" << std::endl;
	SineFP(startAngleSmall, endAngleSmall, incrementSmall);
	std::cout << "Done" << std::endl;

	std::cout << "Test Tan with 8.8 fixed point numbers" << std::endl;
	TanFP(startAngleSmall, endAngleSmall, incrementSmall);
	std::cout << "Done" << std::endl;

	// 16.16 fixed point numbers
	FixedPointMedium startAngleMedium = FixedPointMedium::FromString("0.0");
	FixedPointMedium endAngleMedium = FixedPointMedium::FromString("6.283185307");
	FixedPointMedium incrementMedium = FixedPointMedium::FromString("0.001");

	std::cout << "Test Cosine with 16.16 fixed point numbers" << std::endl;
	CosineFP(startAngleMedium, endAngleMedium, incrementMedium);
	std::cout << "Done" << std::endl;

	std::cout << "Test Sine with 16.16 fixed point numbers" << std::endl;
	SineFP(startAngleMedium, endAngleMedium, incrementMedium);
	std::cout << "Done" << std::endl;

	std::cout << "Test Tan with 16.16 fixed point numbers" << std::endl;
	TanFP(startAngleMedium, endAngleMedium, incrementMedium);
	std::cout << "Done" << std::endl;

	// 32.32 fixed point numbers
	FixedPointLarge startAngleLarge = FixedPointLarge::FromString("0.0");
	FixedPointLarge endAngleLarge = FixedPointLarge::FromString("6.283185307");
	FixedPointLarge incrementLarge = FixedPointLarge::FromString("0.001");

	std::cout << "Test Cosine with 32.32 fixed point numbers" << std::endl;
	CosineFP(startAngleLarge, endAngleLarge, incrementLarge);
	std::cout << "Done" << std::endl;

	std::cout << "Test Sine with 32.32 fixed point numbers" << std::endl;
	SineFP(startAngleLarge, endAngleLarge, incrementLarge);
	std::cout << "Done" << std::endl;

	std::cout << "Test Tan with 32.32 fixed point numbers" << std::endl;
	TanFP(startAngleLarge, endAngleLarge, incrementLarge);
	std::cout << "Done" << std::endl;

	return 0;
}
