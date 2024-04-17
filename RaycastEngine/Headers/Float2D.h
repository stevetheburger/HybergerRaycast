#pragma once

//Struct to represent a 2D floating point vertex with double precision
struct Float2D
{
	double X, Y;
};

//Arithmetic operators.
struct Float2D fAdd(struct Float2D, struct Float2D);
struct Float2D fSub(struct Float2D, struct Float2D);
struct Float2D fMult(struct Float2D, struct Float2D);

//Comparison operators.
char fGreaterThan(struct Float2D, struct Float2D);
char fLessThan(struct Float2D, struct Float2D);
char fEqual(struct Float2D, struct Float2D);
char fNotEqual(struct Float2D, struct Float2D);
char fGreaterThanOrEqual(struct Float2D, struct Float2D);
char fLessThanOrEqual(struct Float2D, struct Float2D);

//A vertex at (0,0)
struct Float2D FloatZero();