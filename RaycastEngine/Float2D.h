#pragma once

struct Float2D
{
	float X, Y;
};

struct Float2D fAdd(struct Float2D, struct Float2D);
struct Float2D fSub(struct Float2D, struct Float2D);
struct Float2D fMult(struct Float2D, struct Float2D);
char fGreaterThan(struct Float2D, struct Float2D);
char fLessThan(struct Float2D, struct Float2D);
char fEqual(struct Float2D, struct Float2D);
char fNotEqual(struct Float2D, struct Float2D);
char fGreaterThanOrEqual(struct Float2D, struct Float2D);
char fLessThanOrEqual(struct Float2D, struct Float2D);
struct Float2D FloatZero();