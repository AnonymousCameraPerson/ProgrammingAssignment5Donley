#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include "ghost.h"

ghost::ghost()
{
	image = al_load_bitmap("boar.png");
	al_convert_mask_to_alpha(image, al_map_rgb(255, 255, 255));
	live = false;
	speed = 3;
	boundx = al_get_bitmap_width(image);
	boundy = al_get_bitmap_height(image);


}
ghost::~ghost()
{
	al_destroy_bitmap(image);
}
void ghost::Drawghost()
{
	if (live)
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		//al_draw_tinted_bitmap(image, al_map_rgba_f(r, g, b, 1), x, y, 0);
		al_draw_tinted_scaled_bitmap(image, al_map_rgba_f(r, g, b, 1), 0, 0, boundx, boundy, x, y, boundx/3, boundy/3, 0);
	}
}
void ghost::Startghost(int WIDTH, int HEIGHT)
{
	if (!live)
	{
		
		live = true;
		x = rand() % (WIDTH - boundx);
		y = rand() % (HEIGHT - boundy);
		
	}
}
void ghost::Updateghost()
{
	if (live)
	{
		 //y= speed;
	}
}


void ghost::Collideghost(Sprite& player)
{
	if (live)
	{
		if (x - boundx < player.getX() + player.getBoundX() &&
			x + boundx > player.getX() - player.getBoundX() &&
			y - boundy < player.getY() + player.getBoundY() &&
			y + boundy > player.getY() - player.getBoundY()
			)
		{
			player.increaseHits();

			live = false;
		}
		else if (x < 0)
		{
			live = false;
		}
	}
}
