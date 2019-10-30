#ifndef K
#define K

class Piksel
{
public:
	Piksel();
	Piksel(int b, int g, int r);
	void ubaci(int b, int g, int r);
	int r();
	int g();
	int b();
	void invert();
	void grey();
private:
	int R, G, B;
};

#endif