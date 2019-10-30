#include "ImageEditor.h"
ImageEditor::ImageEditor()
{
	active = NULL;
	first = NULL;
	last = NULL;
	image = NULL;
	name = NULL;
	width = 0;
	height = 0;
	aktive = new Piksel(0, 0, 0);
}

bool ImageEditor::loadImage(unsigned char* image)
{
	long int i = 0;
	if (*image != 'B' || *(image + 1) != 'M')
	{
		return false;
	}
	i += 2;
	if (image[i] == '=')
	{
		int j = 0;
		i++;
		while (image[i+j] != '=')
			j++;
		name = new char[j+1];
		char* name1 = name;
		while (image[i] != '=')
		{
			*name1 = image[i];
			i++;
			name1++;
		}
		*name1 = 0;
		i++;
	}
	for (; i % 4 != 0; i++);
	width = image[i] + 16 * 16 * image[i + 1] + 256 * 256 * image[i + 2] + 256 * 256 * 256 * image[i + 3];
	i = i + 4;
	height = image[i] + 16 * 16 * image[i + 1] + 256 * 256 * image[i + 2] + 256 * 256 * 256 * image[i + 3];
	i += 4;
	this->image = new Piksel * *[height];
	for (int j = height-1; j >=0; j--)
	{
		this->image[j] = new Piksel * [width];
		for (int k = 0; k < width; k++)
		{
			this->image[j][k] = new Piksel(image[i], image[i + 1], image[i + 2]);
			i += 3;
		}
		for (; i % 4 != 0; i++);
	}
	return true;
}

unsigned char* ImageEditor::saveImage()
{
	long int size = 0;
	long int i = 0;
	unsigned char* slika;
	size += 2;//BM
	if (name)
	{
		size += 2;//= 
		char* name1 = name;
		while (*name1 != 0)
		{
			name1++;
			size++;
		}
	}
	for (; size % 4 != 0; size++);
	size += 8;
	size += 3*height * width;
	int p = (3 * width) % 4;
	size += p * height;
	slika = new unsigned char[size];
	i = 0;
	slika[i] = 'B';
	slika[i+1] = 'M';
	if (name)
	{
		slika[i+2] = '=';
		i = 3;
		char* name1 = name;
		while (*name1)
		{
			slika[i] = *name1;
			i++;
			name1++;
		}
		slika[i] = '=';
		i++;
	}
	for (; i % 4 != 0; i++)
		slika[i] = 1;
	slika[i + 3] = (((width / 256) / 256) / 256);
	slika[i + 2] = ((width / 256) / 256) % 256;
	slika[i + 1] = (width / 256) % 256;
	slika[i] = width % 256;
	i += 4;
	slika[i + 3] = (((height / 256) / 256) / 256);
	slika[i + 2] = ((height / 256) / 256) % 256;
	slika[i + 1] = (height / 256) % 256;
	slika[i] = height % 256;
	i += 4;
	for (int j = height-1; j >= 0; j--)
	{
		for (int k = 0; k < width; k++)
		{
			int b = 0, r = 0, g = 0;
			double procenat = 0;
			Layer* last1 = last;
			while (last1)
			{
				if (last1->slika[j][k]->b() != -1)
				{
					b += last1->slika[j][k]->b() * (1 - procenat) * last1->neprovidnost / 100;
					r += last1->slika[j][k]->r() * (1 - procenat) * last1->neprovidnost / 100;
					g += last1->slika[j][k]->g() * (1 - procenat) * last1->neprovidnost / 100;
					procenat += last1->neprovidnost*(1-procenat)*1. / 100;
				}
				last1 = last1->prev;
			}
			b += image[j][k]->b() * (1 - procenat);
			r += image[j][k]->r() * (1 - procenat);
			g += image[j][k]->g() * (1 - procenat);
			slika[i] = b;
			i++;
			slika[i] = g;
			i++;
			slika[i] = r;
			i++;
		}
		for (; i % 4 != 0; i++)
			slika[i] = 1;
	}
	return slika;
}
void ImageEditor::addLayer()
{
	Layer* novi;
	novi = new Layer;
	novi->next = NULL;
	novi->prev = NULL;
	novi->neprovidnost = 100;
	novi->slika = new Piksel * *[height];
	for (int i = 0; i < height; i++)
	{
		novi->slika[i] = new Piksel * [width];
		for (int j = 0; j < width; j++)
			novi->slika[i][j] = new Piksel();
	}
	if (!first && !last)
	{
		active = first = last = novi;
		return;
	}
	novi->prev = last;
	last->next = novi;
	last = novi;
	active = novi;
}

void ImageEditor::deleteLayer()
{
	if (active == NULL)
		return;
	if (active->next == NULL)
	{
		last = last->prev;
		last->next = NULL;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				delete active->slika[i][j];
			}
			delete active->slika[i];
		}
		delete active->slika;
		delete active;
		active = last;
		return;
	}
	if (last->prev == NULL)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				delete active->slika[i][j];
			}
			delete active->slika[i];
		}
		delete active->slika;
		delete active;
		return;
	}
	if (active->prev == NULL)
	{
		first = first->next;
		first->prev = NULL;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				delete active->slika[i][j];
			}
			delete active->slika[i];
		}
		delete active->slika;
		delete active;
		active = last;
		return;
	}
	active->prev->next = active->next;
	active->next->prev = active->prev;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			delete active->slika[i][j];
		}
		delete active->slika[i];
	}
	delete active->slika;
	delete active;
	active = last;
}

void ImageEditor::selectLayer(int i)
{
	if (i == 0)
	{
		active == NULL;
		return;
	}
	Layer* current=first;
	for (int j = 1; j < i; j++)
	{
		if (current == NULL)
			return;
		current = current->next;
	}
	active = current;
}

void ImageEditor::setLayerOpacity(int i)
{
	active->neprovidnost = i;
}

void ImageEditor::invertColors()
{
	if (active == NULL)
	{
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				image[i][j]->invert();
		return;
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (active->slika[i][j]->r() != -1)
				active->slika[i][j]->invert();
		}
	}
}

void ImageEditor::toGrayScale()
{
	if (active == NULL)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				image[i][j]->grey();
			}
		}
		return;
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (active->slika[i][j]->r() != -1)
				active->slika[i][j]->grey();
		}
	}
}

void ImageEditor::blur(int size)
{
	Piksel*** slika;
	slika = new Piksel * *[height];
	for (int i = 0; i < height; i++)
	{
		slika[i] = new Piksel * [width];
		for (int j = 0; j < width; j++)
		{
			int R = image[i][j]->r(), G = image[i][j]->g(), B = image[i][j]->b(), iR = 1;
			if (R != -1 || G != -1 || B != -1)
			{
				for (int p = -size; p <= size; p++)
				{
					if (p == 0)
						continue;
					for (int q = -size; q <= size; q++)
					{
						if (i + p < 0 || i + p >= height || j + q < 0 || j + q >= width || image[i+p][j+q]->r() == -1)
							continue;
						iR++;
						R += image[i + p][j + q]->r();
						G += image[i + p][j + q]->g();
						B += image[i + p][j + q]->b();
					}
				}
			}
			slika[i][j] = new Piksel(B / iR, G / iR, R / iR);
		}
	}
	if (active)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				delete active->slika[i][j];
			}
			delete active->slika[i];
		}
		active->slika = slika;
	}
	else
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				delete image[i][j];
			}
			//delete image[i];
		}
		image = slika;
	}
}

void ImageEditor::flipHorizontal()
{
	Piksel* zamena;
	if (active == NULL)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width / 2; j++)
			{
				zamena = image[i][j];
				image[i][j] = image[i][width - 1 - j];
				image[i][width - 1 - j] = zamena;
			}
		}
		return;
	}
	for (int i = 0; i < height / 2; i++)
	{			
		for (int j = 0; j < width / 2; j++)
		{
			zamena = active->slika[i][j];
			active->slika[i][j] = active->slika[i][width-1-j];
			image[i][width - j - 1] = zamena;
		}
	}
}

void ImageEditor::flipVertical()
{
	Piksel** zamena;
	if (active == NULL)
	{
		for (int i = 0; i < height / 2; i++)
		{
			zamena = image[i];
			image[i] = image[height - i - 1];
			image[height - i - 1] = zamena;
		}
		return;
	}
	for (int i = 0; i < height / 2; i++)
	{
		zamena = active->slika[i];
		active->slika[i] = active->slika[height - i - 1];
		image[height - i - 1] = zamena;
	}
}

void ImageEditor::crop(int x, int y, int w, int h)
{

	Piksel*** slika1;
	slika1 = new Piksel * *[h];
	for (int i = 0; i < h; i++)
	{
		slika1[i] = new Piksel * [w];
		for (int j = 0; j < w; j++)
		{
			slika1[i][j] = new Piksel(image[i + y][j + x]->b(), image[i + y][i + x]->g(), image[i + y][i + x]->r());
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			delete image[i][j];
		}
		delete image[i];
	}
	image = slika1;
	Layer* tr = first;
	while (tr)
	{
		Piksel*** slika2;
		slika2 = new Piksel * *[h];
		for (int i = 0; i < h; i++)
		{
			slika2[i] = new Piksel * [w];
			for (int j = 0; j < w; j++)
			{
				slika2[i][j] = new Piksel(tr->slika[i + y][j + x]->b(), tr->slika[i + y][j + x]->g(), tr->slika[i + y][j + x]->r());
			}
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				delete tr->slika[i][j];
			}
			delete tr->slika[i];
		}
		tr->slika = slika2;
		tr = tr->next;
	}
	width = w;
	height = h;
}

void ImageEditor::setActiveColor(string hex)
{
	int r=0, g=0, b=0;
	for (int i = 1; i <= 6; i++)
	{
		int p=0;
		if (hex[i] >= 'A' && hex[i] <= 'F')
			p = hex[i] - 'A' + 10;
		if (hex[i] >= '0' && hex[i] <= '9')
			p = hex[i] - '0';
		if (i == 1)
		{
			r += 16 * p;
		}
		if (i == 2)
		{
			r += p;
		}
		if (i == 3)
		{
			b += 16 * p;
		}
		if (i == 4)
		{
			b += p;
		}
		if (i == 5)
		{
			g += 16 * p;
		}
		if (i == 6)
		{
			g += p;
		}
	}
	aktive->ubaci(b, g, r);
}

void ImageEditor::fillRect(int x, int y, int w, int h)
{
	if (active == NULL)
	{
		for (int i = y; i < y + h; i++)
		{
			for (int j = x; j < x + w; j++)
			{
				image[i][j]->ubaci(aktive->b(), aktive->g(), aktive->r());
			}
		}
		return;
	}
	for (int i = y; i < y + h; i++)
	{
		for (int j = x; j < x + w; j++)
		{
			active->slika[i][j]->ubaci(aktive->b(), aktive->g(), aktive->r());
		}
	}
}

void ImageEditor::eraseRect(int x, int y, int w, int h)
{
	if (active == NULL)
	{
		for (int i = y; i < y + h; i++)
		{
			for (int j = x; j < x + w; j++)
			{
				image[i][j]->ubaci(-1, -1, -1);
			}
		}
		return;
	}
	for (int i = y; i < y + h; i++)
	{
		for (int j = x; j < x + w; j++)
		{
			active->slika[i][j]->ubaci(-1, -1, -1);
		}
	}
}
