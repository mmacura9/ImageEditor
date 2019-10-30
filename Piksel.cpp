#include "Piksel.h"

Piksel::Piksel()
{
	R = G = B = -1;
}

Piksel::Piksel(int b,int g,int r)
{
	R = r;
	G = g;
	B = b;
}

void Piksel::ubaci(int b, int g, int r)
{
	R = r;
	G = g;
	B = b;
}

int Piksel::r()
{
	return R;
}

int Piksel::g()
{
	return G;
}

int Piksel::b()
{
	return B;
}

void Piksel::invert()
{
	R = 255 - R;
	G = 255 - G;
	B = 255 - B;
}

void Piksel::grey()
{
	R = G = B = 0.3 * R + 0.59 * G + 0.11 * B;
}
