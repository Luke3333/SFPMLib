#include "..\SFPMLib\src\SFPMLib.h"

using namespace sfpmlib;

void CosineFPMedium()
{
	FixedPointMedium endAngle = FixedPointMedium::FromString("6.283185307");
	FixedPointMedium increment = FixedPointMedium::FromString("0.001");

	FILE* f = fopen("C:\\Temp\\CosineFPMedium.txt", "w");
	if (f)
	{
		for(FixedPointMedium arg = FixedPointMedium(false, 0); arg < endAngle; arg += increment)
		{
			FixedPointMedium val = FixedPointMedium::Cos(arg);
			fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
		}
		fclose(f);
	}
}

void SineFPMedium()
{
	FixedPointMedium endAngle = FixedPointMedium::FromString("6.283185307");
	FixedPointMedium increment = FixedPointMedium::FromString("0.001");

	FILE* f = fopen("C:\\Temp\\SineFPMedium.txt", "w");
	if (f)
	{
		for(FixedPointMedium arg = FixedPointMedium(false, 0); arg < endAngle; arg += increment)
		{
			FixedPointMedium val = FixedPointMedium::Sin(arg);
			fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
		}
		fclose(f);
	}
}

void TanFPMedium()
{
	static FixedPointMedium pi_2(FixedPointMedium::FromString("1.570796326"));
	static FixedPointMedium pi(FixedPointMedium::FromString("3.141592653"));
	static FixedPointMedium pi_3_2(FixedPointMedium::FromString("4.712388980"));
	static FixedPointMedium pi2(FixedPointMedium::FromString("6.283185307"));

	FixedPointMedium endAngle = FixedPointMedium::FromString("6.283185307");
	FixedPointMedium increment = FixedPointMedium::FromString("0.001");

	FILE* f = fopen("C:\\Temp\\TanFPMedium.txt", "w");
	if (f)
	{
		for(FixedPointMedium arg = FixedPointMedium(false, 0); arg < endAngle; arg += increment)
		{
			try
			{
				FixedPointMedium val = FixedPointMedium::Tan(arg);
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

void LogFPMedium()
{
	FixedPointMedium start = FixedPointMedium::FromString("0.0001");
	FixedPointMedium end = FixedPointMedium::FromString("10000.0");
	FixedPointMedium increment = FixedPointMedium::FromString("0.1");

	FILE* f = fopen("C:\\Temp\\LogFPMedium.txt", "w");
	if (f)
	{
		for(FixedPointMedium arg = start; arg < end; arg += increment)
		{
			FixedPointMedium val = FixedPointMedium::Log(arg, FixedPointMedium(false, 10, 0));
			fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
		}

		fclose(f);
	}
}

void ExpFPLarge()
{
	FixedPointLarge start = FixedPointLarge::FromString("-5.0");
	FixedPointLarge end = FixedPointLarge::FromString("5.0");
	FixedPointLarge increment = FixedPointLarge::FromString("0.01");

	FILE* f = fopen("C:\\Temp\\ExpFPLarge.txt", "w");
	if (f)
	{
		for(FixedPointLarge arg = start; arg < end; arg += increment)
		{
			FixedPointLarge val = FixedPointLarge::Exp(arg);
			fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
		}

		fclose(f);
	}
}

void ParabolicFPMedium()
{
	FixedPointLarge start = FixedPointLarge::FromString("-100.0");
	FixedPointLarge end = FixedPointLarge::FromString("100.0");
	FixedPointLarge increment = FixedPointLarge::FromString("0.01");

	FILE* f = fopen("C:\\Temp\\ParabolicFPMedium.txt", "w");
	if (f)
	{
		for(FixedPointLarge arg = start; arg < end; arg += increment)
		{
			FixedPointLarge val = FixedPointLarge::Pow(arg, 2);
			fprintf(f, "%s;%s\n", arg.ToString().c_str(), val.ToString().c_str());
		}

		fclose(f);
	}
}

void TestSFPMLib()
{
	FixedPointMedium add1 = FixedPointMedium::FromString("-10.5");
	FixedPointMedium add2 = FixedPointMedium::FromString("31.25");
	FixedPointMedium sum = add1 + add2;
	std::string s = sum.ToString();
	std::string s2;

	add1 = FixedPointMedium::FromString("31.25");
	add2 = FixedPointMedium::FromString("-32.5");
	sum = add1 + add2;
	s = sum.ToString();

	FixedPointMedium arg = FixedPointMedium::FromString("1.569061278");
	FixedPointMedium res = FixedPointMedium::Cos(arg);
	s = res.ToString();

	arg = FixedPointMedium::FromString("0.007934570");
	res = FixedPointMedium::Sin(arg);
	s = res.ToString();

	add1 = FixedPointMedium::FromString("5.0");
	add2 = FixedPointMedium::FromString("5.0");
	res = add1 / add2;
	s = res.ToString();

	add1 = FixedPointMedium::FromString("5.0");
	add2 = FixedPointMedium::FromString("2.5");
	res = add1 / add2;
	s = res.ToString();

	add1 = FixedPointMedium::FromString("5.0");
	add2 = FixedPointMedium::FromString("2.0");
	res = add1 / add2;
	s = res.ToString();

	add1 = FixedPointMedium::FromString("9488.363082883");
	add2 = FixedPointMedium::FromString("40320");
	sum = add1 / add2;
	s = sum.ToString();

	add1 = FixedPointMedium::FromString("32820.26297518");
	add2 = FixedPointMedium::FromString("40320");
	sum = add1 / add2;
	s = sum.ToString();

	add1 = FixedPointMedium::FromString("32767");
	add2 = FixedPointMedium::FromString("0.5");
	sum = add1 / add2;
	s = sum.ToString();

	add1 = FixedPointMedium::FromString("32767");
	add2 = FixedPointMedium::FromString("0.5");
	sum = add1 * add2;
	s = sum.ToString();

	add1 = FixedPointMedium::FromString("32767");
	add2 = FixedPointMedium::FromString("2.0");
	sum = add1 * add2;
	s = sum.ToString();

	add1 = FixedPointMedium::FromString("32767");
	add2 = FixedPointMedium::FromString("2.0");
	sum = add1 / add2;
	s = sum.ToString();

	add1 = FixedPointMedium::FromString("32767.5");
	add2 = FixedPointMedium::FromString("0.5");
	sum = add1 / add2;
	s = sum.ToString();

	add1 = FixedPointMedium::FromString("32767.5");
	add2 = FixedPointMedium::FromString("2.0");
	sum = add1 * add2;
	s = sum.ToString();

	arg = FixedPointMedium::FromString("3.668746946");
	res = FixedPointMedium::Cos(arg);
	s = res.ToString();

	arg = FixedPointMedium::FromString("0.001");
	arg += FixedPointMedium::FromString("0.1");
	s = arg.ToString();
	res = FixedPointMedium::Log(arg, FixedPointMedium(false, 10, 0));
	s = res.ToString();

	add1 = FixedPointMedium::FromString("5.0");
	res = FixedPointMedium::Log(add1, FixedPointMedium(false, 10, 0));
	s = res.ToString();

	add2 = FixedPointMedium::FromString("2.5");
	res = FixedPointMedium::Pow(add1, add2);
	s = res.ToString();

	add2 = FixedPointMedium::FromString("-2.5");
	res = FixedPointMedium::Pow(add1, add2);
	s = res.ToString();

	res = FixedPointMedium::Pow(add1, 2);
	s = res.ToString();
	res = FixedPointMedium::Pow(add1, -2);
	s = res.ToString();

	ParabolicFPMedium();

	CosineFPMedium();
	SineFPMedium();
	TanFPMedium();

	FixedPointLarge argLarge = FixedPointLarge::FromString("-5.0");
	s = argLarge.ToString();
	FixedPointLarge resLarge = FixedPointLarge::Exp(argLarge);
	s = resLarge.ToString();

	FILE* f = fopen("C:\\Temp\\PowLog.txt", "w");
	for(int i = 1; i < 12; i++)
	{
		FixedPointLarge neper = FixedPointLarge::FromString("2.718281828");
		neper = FixedPointLarge::Pow(neper, FixedPointLarge(false, i, 0));
		fprintf(f, "e^%d = %s\n", i, neper.ToString().c_str());
	}
	fclose(f);

	add1 = FixedPointMedium::FromString("11.09");
	res = FixedPointMedium::Exp(add1);
	s = res.ToString();

	ExpFPLarge();
	LogFPMedium();

	FixedPointMedium fixedPoint(false, 10);
	FixedPointMedium fixedPoint2(false, 20);

	if (fixedPoint == fixedPoint2)
	{
		std::cout << "fixedPoint == fixedPoint2" << std::endl;
	}

	FixedPointMedium signedFractional = FixedPointMedium::FromString("-0.5");
	s = signedFractional.ToString();

	FixedPointMedium fpMedium;
	fpMedium = FixedPointMedium::FromString("10.5");
	fpMedium = FixedPointMedium::Pow(fpMedium, 2);
	s = fpMedium.ToString();

	FixedPointMedium fp(false, 10, 0);
	fp = FixedPointMedium::Pow(fp, 2);
	s = fp.ToString();

	FixedPointMedium log = FixedPointMedium::Log(fp, FixedPointMedium(false, 10, 0));
	s = log.ToString();
	fp = FixedPointMedium(false, 1, 0);
	s = FixedPointMedium::Log(fp, FixedPointMedium(false, 10, 0)).ToString();

	fp = FixedPointMedium(false, 64, 0);
	fp = FixedPointMedium::Sqrt(fp);
	s = fp.ToString();
	fp = FixedPointMedium(false, 63, 0);
	fp = FixedPointMedium::Sqrt(fp);
	s = fp.ToString();

	FixedPointMedium fps1(false, 10, 0);
	FixedPointMedium fps2(false, 5, 0);
	FixedPointMedium fps3 = fps1 / fps2;
	s = fps3.ToString();

	fps1 = FixedPointMedium::FromString("0.125");
	fps2 = FixedPointMedium::FromString("0.125");
	fps3 = fps1 * fps2;
	s = fps3.ToString();

	fps1 = FixedPointMedium::FromString("1.567077636");
	fps1 = FixedPointMedium::Pow(fps1, 2);
	fps1 /= FixedPointMedium(true, 2, 0);
	s = fps1.ToString();

	fps2 = FixedPointMedium::FromString("-1.0");
	fps2 += fps1;
	s = fps2.ToString();

	FixedPointMedium a = FixedPointMedium::FromString("20.0");
	FixedPointMedium b = FixedPointMedium::FromString("8.0");
	FixedPointMedium c = a / b;
	s = c.ToString();

	a = FixedPointMedium::FromString("1.0");
	b = FixedPointMedium::FromString("128.0");
	c = a / b;
	s = c.ToString();

	a = FixedPointMedium::FromString("10.5");
	b = FixedPointMedium::FromString("2.1");
	c = a / b;
	s = c.ToString();

	a = FixedPointMedium::FromString("6.3");
	b = FixedPointMedium::FromString("0.1");
	c = a / b;
	s = c.ToString();

	a = FixedPointMedium::FromString("0.1");
	b = FixedPointMedium::FromString("0.25");
	c = a / b;
	s = c.ToString();

	a = FixedPointMedium::FromString("0.1");
	b = FixedPointMedium::FromString("10.0");
	c = a / b;
	s = c.ToString();

	a = FixedPointMedium::FromString("5.0");
	b = FixedPointMedium::FromString("0.75");
	c = a / b;
	s = c.ToString();

	FixedPointMedium as = FixedPointMedium::FromString("0.5");
	FixedPointMedium bs = FixedPointMedium::FromString("2.0");
	FixedPointMedium cs = as / bs;
	s = cs.ToString();

	Vec3Large vl1{ FixedPointLarge::FromString("10"), FixedPointLarge::FromString("20"), FixedPointLarge::FromString("30") };
	Vec3Large vl2{ FixedPointLarge::FromString("5"), FixedPointLarge::FromString("67"), FixedPointLarge::FromString("113") };
	Vec3Large vl3 = Vec3Large::Cross(vl1, vl2);
	s = vl3.ToString();

	resLarge = vl1 * vl2;
	s = resLarge.ToString();
	vl3 = vl1 * FixedPointLarge::FromString("3.5");
	s = vl3.ToString();

	resLarge = FixedPointLarge::Degrees(Vec3Large::Angle(vl1, vl2));
	s = resLarge.ToString();

	Vec2Medium v1{ FixedPointMedium(false, 3, 0), FixedPointMedium(false, 0, 0) };
	Vec2Medium v2{ FixedPointMedium(false, 0, 0), FixedPointMedium(false, 4, 0) };
	Vec2Medium v3 = v1 + v2;
	fps1 = v3.Mod();
	s = fps1.ToString();
	v3 = v1 - v2;
	fps1 = v3.Mod();
	s = fps1.ToString();

	v3 = v1 + v2;
	Vec2Medium v4 = v3.Normalize();
	s = v4.ToString();
	fps1 = v4.Mod();
	s = fps1.ToString();

	Mat3x3Medium mm{ FixedPointMedium(false, 1, 0), FixedPointMedium(false, 5, 0), FixedPointMedium(false, 7, 0),
					 FixedPointMedium(false, 2, 0), FixedPointMedium(false, 4, 0), FixedPointMedium(false, 6, 0),
					 FixedPointMedium(false, 3, 0), FixedPointMedium(false, 11, 0), FixedPointMedium(false, 19, 0) };

	Mat3x3Medium mm2{ FixedPointMedium(false, 11, 0), FixedPointMedium(false, 15, 0), FixedPointMedium(false, 17, 0),
					  FixedPointMedium(false, 21, 0), FixedPointMedium(false, 43, 0), FixedPointMedium(false, 67, 0),
					  FixedPointMedium(false, 35, 0), FixedPointMedium(false, 116, 0), FixedPointMedium(false, 193, 0) };

	std::cout << "mm = " << mm.ToString() << std::endl;
	FixedPointMedium det(mm.Det());
	std::cout << "Det(mm) = " << det.ToString() << std::endl;

	Mat3x3Medium mm3;
	mm3 = mm * mm2;
	std::cout << "mm3 = mm * mm2 = " << mm3.ToString() << std::endl;

	mm3 = mm + mm2;
	std::cout << "mm3 = mm + mm2 = " << mm3.ToString() << std::endl;

	mm3 = mm - mm2;
	std::cout << "mm3 = mm - mm2 = " << mm3.ToString() << std::endl;

	mm3 = mm * FixedPointMedium::FromString("3.5");
	std::cout << "mm3 = mm * 3.5 = " << mm3.ToString() << std::endl;

	mm3 = Mat3x3Medium::Inv(mm);
	std::cout << "Inverse(mm) = " << mm3.ToString() << std::endl;

	mm3 = Mat3x3Medium::Transpose(mm);
	std::cout << "Transpose(mm) = " << mm3.ToString() << std::endl;

	mm3 = mm * Mat3x3Medium::Inv(mm);
	std::cout << "mm * Inverse(mm) = " << mm3.ToString() << std::endl;

	std::cout << "mm[0][0] = " << mm[0][0].ToString() << std::endl;
}

int main(int argc, char** argv)
{
	TestSFPMLib();

	std::cout << "Floating Points" << std::endl;
	float f = 1.25f;
	std::cout << "floor(f) = " << floor(f) << " - ceil(f) = " << ceil(f) << std::endl;
	f = -2.75f;
	std::cout << "floor(f) = " << floor(f) << " - ceil(f) = " << ceil(f) << std::endl;
	f = 1.4f;
	std::cout << "round(f) = " << round(f) << std::endl;
	f = -4.6f;
	std::cout << "round(f) = " << round(f) << std::endl;

	std::cout << "Fixed Points" << std::endl;
	FixedPointMedium fpm = FixedPointMedium::FromString("1.25");
	std::cout << "floor(fpm) = " << FixedPointMedium::Floor(fpm).ToString() << " - ceil(fpm) = " << FixedPointMedium::Ceil(fpm).ToString() << std::endl;
	fpm = FixedPointMedium::FromString("-2.75");
	std::cout << "floor(fpm) = " << FixedPointMedium::Floor(fpm).ToString() << " - ceil(fpm) = " << FixedPointMedium::Ceil(fpm).ToString() << std::endl;
	fpm = FixedPointMedium::FromString("1.4");
	std::cout << "round(fpm) = " << FixedPointMedium::Round(fpm).ToString() << std::endl;
	std::cout << "int(fpm) = " << FixedPointMedium::Int(fpm).ToString() << std::endl;
	std::cout << "Fract(fpm) = " << FixedPointMedium::Fract(fpm).ToString() << std::endl;
	fpm = FixedPointMedium::FromString("-4.6");
	std::cout << "round(fpm) = " << FixedPointMedium::Round(fpm).ToString() << std::endl;
	std::cout << "int(fpm) = " << FixedPointMedium::Int(fpm).ToString() << std::endl;
	std::cout << "Fract(fpm) = " << FixedPointMedium::Fract(fpm).ToString() << std::endl;

	return 0;
}