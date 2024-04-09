#include "Int2D.h"

//Arithmetic functions.
struct Int2D iAdd(struct Int2D l, struct Int2D r)
{
	struct Int2D sum;
	sum.X = l.X + r.X;
	sum.Y = l.Y + r.Y;
	return sum;
}
struct Int2D iSub(struct Int2D l, struct Int2D r)
{
	struct  Int2D dif;
	dif.X = l.X - r.X;
	dif.Y = l.Y - r.Y;
	return dif;
}
struct Int2D iMult(struct Int2D l, struct Int2D r)
{
	struct Int2D prod;
	prod.X = l.X * r.X;
	prod.Y = l.Y * r.Y;
	return prod;
}

//Comparative functions.
char iGreaterThan(struct Int2D l, struct Int2D r)
{
    return l.Y > r.Y || l.Y == r.Y && l.X > r.X;
}
char iLessThan(struct Int2D l, struct Int2D r)
{
    return l.Y < r.Y || l.Y == r.Y && l.X < r.X;
}
char iEqual(struct Int2D l, struct Int2D r)
{
    return l.Y == r.Y && l.X == r.X;
}
char iNotEqual(struct Int2D l, struct Int2D r)
{
    return l.Y != r.Y && l.X != r.X;
}
char iGreaterThanOrEqual(struct Int2D l, struct Int2D r)
{
    return l.Y > r.Y || l.Y == r.Y && l.X >= r.X;
}
char iLessThanOrEqual(struct Int2D l, struct Int2D r)
{
    return l.Y < r.Y || l.Y == r.Y && l.X <= r.X;
}

//Vertex at (0,0)
struct Int2D IntZero()
{
	struct Int2D zero;
	zero.X = zero.Y = 0;
	return zero;
}

//Functions to convert vertex to array index or to array size.
unsigned int iToIndex(struct Int2D pos, struct Int2D dim)
{
	return dim.X * pos.Y + pos.X;
}
unsigned int iToSize(struct Int2D dim)
{
	return dim.X * dim.Y;
}