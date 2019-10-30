#ifndef LAYER
#define LAYER
#include "Piksel.h"
struct Layer
{
	Piksel*** slika;
	Layer* next, * prev;
	int neprovidnost;
};

#endif