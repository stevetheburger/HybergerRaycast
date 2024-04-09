#include "Float2D.h"

//Arithmetic functions.
struct Float2D fAdd(struct Float2D l, struct Float2D r)
{
	struct Float2D sum;
	sum.X = l.X + r.X;
	sum.Y = l.Y + r.Y;
	return sum;
}
struct Float2D fSub(struct Float2D l, struct Float2D r)
{
	struct Float2D dif;
	dif.X = l.X - r.X;
	dif.Y = l.Y - r.Y;
	return dif;
}
struct Float2D fMult(struct Float2D l, struct Float2D r)
{
	struct Float2D prod;
	prod.X = l.X * r.X;
	prod.Y = l.Y * r.Y;
	return prod;
}

//Comparative functions.
char fGreaterThan(struct Float2D l, struct Float2D r)
{
    return l.Y > r.Y || l.Y == r.Y && l.X > r.X;
}
char fLessThan(struct Float2D l, struct Float2D r)
{
    return l.Y < r.Y || l.Y == r.Y && l.X < r.X;
}
char fEqual(struct Float2D l, struct Float2D r)
{
    return l.Y == r.Y && l.X == r.X;
}
char fNotEqual(struct Float2D l, struct Float2D r)
{
    return l.Y != r.Y && l.X != r.X;
}
char fGreaterThanOrEqual(struct Float2D l, struct Float2D r)
{
    return l.Y > r.Y || l.Y == r.Y && l.X >= r.X;
}
char fLessThanOrEqual(struct Float2D l, struct Float2D r)
{
    return l.Y < r.Y || l.Y == r.Y && l.X <= r.X;
}

//Vertex at (0,0)
struct Float2D FloatZero()
{
	struct Float2D zero;
	zero.X = zero.Y = 0;
	return zero;
}