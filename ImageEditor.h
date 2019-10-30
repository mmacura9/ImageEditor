#ifndef IMAGEEDITOR
#define IMAGEEDITOR
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include "Piksel.h"
#include "Layer.h"
using namespace std;

class ImageEditor
{
public:
	ImageEditor();
	bool loadImage(unsigned char* image);
	unsigned char* saveImage();
	void addLayer();
	void deleteLayer();
	void selectLayer(int i);
	void setLayerOpacity(int i);
	void invertColors();
	void toGrayScale();
	void blur(int  size);
	void flipHorizontal();
	void flipVertical();
	void crop(int x, int y, int w, int h);
	void setActiveColor(string hex);
	void fillRect(int x, int y, int w, int h);
	void eraseRect(int x, int y, int w, int h);

private:

	Piksel*** image;
	long int width, height;
	char* name;
	Layer* first, * last, * active;
	Piksel* aktive;
};

#endif