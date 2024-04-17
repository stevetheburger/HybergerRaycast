#pragma once

//Structure for 2D integer vertex
struct Int2D
{
	int X, Y;
};

//Arithmetic functions.
struct Int2D iAdd(struct Int2D, struct Int2D);
struct Int2D iSub(struct Int2D, struct Int2D);
struct Int2D iMult(struct Int2D, struct Int2D);

//Comparative functions.
char iGreaterThan(struct Int2D, struct Int2D);
char iLessThan(struct Int2D, struct Int2D);
char iEqual(struct Int2D, struct Int2D);
char iNotEqual(struct Int2D, struct Int2D);
char iGreaterThanOrEqual(struct Int2D, struct Int2D);
char iLessThanOrEqual(struct Int2D, struct Int2D);

//Vertex at (0,0)
struct Int2D IntZero();

//Functions to convert vertex to array index or to array size.
unsigned int iToIndex(struct Int2D pos, struct Int2D dim);
unsigned int iToSize(struct Int2D dim);