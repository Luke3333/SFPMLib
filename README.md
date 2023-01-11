# SFPMLib

# Simple Fixed Point Math Library

---

SFPMLib is a library conceived to perform mathematical operations on FixedPoint numbers.
It is suitable for systems when a FPU (Floating Point Unit) is missing but it's desirable to achieve 
precision in mathematical operations without the overhead of floating point mathematics emulation.

It is a single header file library written in C++ 11.

To use it, simply add the header file in your project and include it in the C++ module you want to 
work with.

Among the basic operations (adding, subtracting, multiplicating and dividing) there are other 
possible operations like:

- **Comparison**
- **Power to an exponent**
- **Logarithm** (in any base)
- **Square root**
- **Trigonometric functions (sine, cosine and tangent)**
- **Radians<=>Degrees conversions**

Three possible FixedPoint types are available:

- **FixedPointSmall** (8 bits of integer part, 8 bits of decimal part)
- **FixedPointMedium** (16 bits of integer part, 16 bits of decimal part)
- **FixedPointLarge** (32 bits of integer part, 32 bits of decimal part)

Along with FixedPoint numbers, there are other 2 available objects.

***Vectors of FixedPoint objects (2, 3 and 4 components)***

The following operations for vectors are available:

- **Adding/Subratcting**
- **Dot product** (2 vectors)
- **Cross product** (2 vectors made of 3 components)
- **Product of a vector by a scalar**
- **Angle between 2 vectors**
- **Normalization**
- **Length**

***Matrices of FixedPoint objects (2x2, 3x3, 4x4 components)***

The following operations for matrices are available:

- **Adding/Subratcting**
- **Product of 2 matrices** (row per column)
- **Product of a matrix by a scalar**
- **Determinant**
- **Inverse**
- **Transpose**
