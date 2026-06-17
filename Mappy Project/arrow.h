#include "Sprite.h"
#include "ghost.h"

class Arrow
{
public:
	Arrow();
	~Arrow();
	void DrawArrow();
	void FireArrow(Sprite& player);
	void UpdateArrow(int WIDTH);
	void CollideArrow(ghost ghosts[], int cSize, Sprite& player);
private:
	int x;
	int y;
	bool live;
	int speed;
	ALLEGRO_BITMAP* image;
};

