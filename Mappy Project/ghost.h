#ifndef GHOSTH
#define GHOSTH
#include "Sprite.h"
class ghost
{
public:
	ghost();
	~ghost();
	void Drawghost();
	void Startghost(int WIDTH, int HEIGHT);
	void Updateghost();
	void Collideghost(Sprite& player);
	int getBoundX() { return boundx; }
	int getBoundY() { return boundy; }
	int getX() { return x; }
	int getY() { return y; }
	bool getLive() { return live; }
	void setLive(bool l) { live = l; }
	int getHits();
private:
	int x;
	int y;
	bool live;
	int hits;
	int speed;
	int boundx;
	int boundy;

	ALLEGRO_BITMAP* image;
};
#endif
