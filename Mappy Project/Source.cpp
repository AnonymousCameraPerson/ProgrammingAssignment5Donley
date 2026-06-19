#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Sprite.h"
#include "ghost.h"
#include "arrow.h"
#include <string>
#include <stdio.h>
#include <allegro5/allegro_native_dialog.h>
#include <string.h>
#include "mappy_A5.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <thread>
#include <string>
//Lucia Donley
using namespace std;

int collided(int x, int y);  //Tile Collision
int collideWithCeiling(int x, int y); //Collision with ceiling
bool endValue(int x, int y); //End Block with the User Value = 8
void drawStatus(int xOff, int yOff, Sprite& player, string& health, bool& isDead, bool& wasHit);
int main(void)
{
	bool hasWon = false;
	string health = "Perfect";
	const int WIDTH = 900;
	const int HEIGHT = 480;
	const int NUM_ghostS = 40;
	int ghosts_shown;
	const int FPS = 60;
	bool keys[] = { false, false, false, false, false };
	enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
	//variables
	bool done = false;
	bool render = false;
	//Player Variable
	Sprite player;
	const int JUMPIT = 1600;
	int jump = JUMPIT;
	double current_seconds = 0;
	int not_double_secs = 0;
	bool timesUp = false;
	int level = 1;
	bool levelOver = false;
	char name[50];
	//bool gameOver = 0;
	double startTime = 0.0;
	int MAX_SECS = 45;
	int timeLeft = 30;
	bool isDead = false;
	int numHits = 0;
	bool wasHit = false;
	int finishedTimes[3];

	//allegro variable
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer;
	ALLEGRO_FONT* font = NULL;
	ALLEGRO_FONT* time_font = NULL;
	ALLEGRO_SAMPLE* sample = NULL;

	//program init
	if (!al_init())										//initialize Allegro
		return -1;

	if (!al_install_audio()) {
		return -1;
	}
	if (!al_init_acodec_addon()) {
		return -1;
	}


	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if (!display)										//test display object
		return -1;

	//addon init
	al_install_keyboard();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	
	

	ghost ghosts[NUM_ghostS];

	player.InitSprites(WIDTH, HEIGHT);

	int xOff = 0;
	int yOff = 0;
	if (MapLoad("FinalProjectLevel1.FMP", 1))
		return -5;

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	//srand(time(NULL));

	font = al_load_ttf_font("college.ttf", 54, 0);
	time_font = al_load_ttf_font("Coolvetica Hv Comp.otf", 36, 0);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_textf(font, al_map_rgb(0, 255, 255), WIDTH / 3, 100, 0, "Welcome!");
	al_draw_textf(time_font, al_map_rgb(0, 255, 100), WIDTH / 4, 200, 0, "The goal is to get to the end of all 3 levels before time runs out");
	al_draw_textf(time_font, al_map_rgb(0, 255, 100), WIDTH / 5, 250, 0, "Use the arrow keys to move around. Don't touch the blocks!");
	al_draw_textf(time_font, al_map_rgb(0, 255, 100), WIDTH / 3, 300, 0, "If you do, you will go crazy!");
	al_draw_textf(time_font, al_map_rgb(0, 255, 100), WIDTH / 3, HEIGHT - 100, 0, "Continue in 10 seconds...");
	al_flip_display();
	//cin.get();
	this_thread::sleep_for(chrono::seconds(10));

	//draw the background tiles
	MapDrawBG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1);

	//draw foreground tiles
	MapDrawFG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);

	player.DrawSprites(0, 0);
	
	al_flip_display();
	
	
	startTime = al_get_time();
	
	while (!done)
	{
		if (numHits == 5) {
			break;
		}

		//Pollack pseudo code:

		if (levelOver) {

			timer = 0;
			timesUp = false;
			level++;
			if (level > 3) {
				done = true;
				break;
			}
			MapFreeMem();
			snprintf(name, sizeof(name), "FinalProjectLevel%i.FMP", level);

			player.InitSprites(WIDTH, HEIGHT);

			int xOff = 0;
			int yOff = 0;
			current_seconds = 0;

			int loadResult = MapLoad(name, 1);

			if (loadResult != 0)
			{
				al_show_native_message_box(NULL, "Error", "Map Failure", "Could not load level file!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
				exit(0);
			} //load map
			//check to see if time is over
			startTime = al_get_time();
			levelOver = false;

		}
		double secondsGoneBy = al_get_time() - startTime;
		timeLeft = MAX_SECS - (int)secondsGoneBy;

		current_seconds = al_get_time();
		not_double_secs = (int)current_seconds;

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);


		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (numHits == 5) {
				break;
			}
			//draw status bar
			//al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Health:");
			//al_draw_filled_rectangle(20, 30, 130, 150, al_map_rgb(0, 255, 0));
			render = true;
			MapUpdateAnims();
			if (keys[UP])
				player.UpdateSprites(WIDTH, HEIGHT, 1);
			else if (keys[DOWN])
				player.UpdateSprites(WIDTH, HEIGHT, 0);
			else if (keys[LEFT])
				player.UpdateSprites(WIDTH, HEIGHT, 2);
			else if (keys[RIGHT])
				player.UpdateSprites(WIDTH, HEIGHT, 3);
			else if (keys[SPACE])
				;
			else
				player.UpdateSprites(WIDTH, HEIGHT, 5);
			//for (int i = 0; i < NUM_ghostS; i++) {
			//	ghosts[i].Startghost(WIDTH, HEIGHT);
			//}
			//for (int i = 0; i < NUM_ghostS; i++) {
			//	ghosts[i].Updateghost();
			//}
			//for (int i = 0; i < NUM_ghostS; i++) {
			//	ghosts[i].Collideghost(player);
			//}
			render = true;

		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				jump = 30;

			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		if (render && al_is_event_queue_empty(event_queue))
		{
			if (numHits == 5) {
				break;
			}
			render = false;

			//update the map scroll position
			xOff = player.getX() + player.getWidth() - WIDTH / 2;
			yOff = player.getY() + player.getHeight() - HEIGHT / 2;

			//avoid moving beyond the map edge
			if (xOff < 0) xOff = 0;

			if (xOff > (mapwidth * mapblockwidth - WIDTH))
				xOff = mapwidth * mapblockwidth - WIDTH;
			if (yOff < 0)
				yOff = 0;
			if (yOff > (mapheight * mapblockheight - HEIGHT))
				yOff = mapheight * mapblockheight - HEIGHT;

			//draw the background tiles
			MapDrawBG(xOff, yOff, 0, 0, WIDTH, HEIGHT);

			//draw foreground tiles
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, HEIGHT, 0);
			//jump = player.jumping(jump, JUMPIT);
			player.DrawSprites(xOff, yOff);
			drawStatus(xOff, yOff, player, health, isDead, wasHit);
			for (int i = 0; i < NUM_ghostS; i++) {
				ghosts[i].Drawghost();
			}
			
			al_draw_textf(time_font, al_map_rgb(0, 0, 255), WIDTH - 250, HEIGHT - 35, 0, "Time Left: %d", timeLeft);

			if (player.CollisionEndBlock()) {
				hasWon = true;
				levelOver = true;
				al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2 - 200, 150, 0, "Done in %d seconds!", MAX_SECS - timeLeft);
				if (name[17] == '1') {
					finishedTimes[0] = MAX_SECS - timeLeft;
				}
				else if (name[17] == '2') {
					finishedTimes[1] = MAX_SECS - timeLeft;
				}
				else if (name[17] == '3') {
					finishedTimes[2] = MAX_SECS - timeLeft;
				}
			}
			else if (timeLeft <= 0) {
				timeLeft = 0;
				timesUp = true;
				al_draw_text(font, al_map_rgb(255, 0, 0), WIDTH / 2, 150, 0, "Time's Up!");
			}
			//}



			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
			if (isDead) {
				al_rest(3.0);
				break;
			}
			if (hasWon) {
				bool lastLevel = false;
				al_rest(3.0);
				for (char e : name) {
					if (e == '3') {
						lastLevel = true;
						break;
					}
				}

				if (lastLevel) {
					hasWon = true;
				}
				else 
					hasWon = false;
				
			}
			if (timesUp) {
				done = true;
			}
		}
	}
	if (isDead) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font, al_map_rgb(255, 0, 0), WIDTH / 3, 150, 0, "YOU DIED!");
		al_flip_display();
	}
	else if (hasWon) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font, al_map_rgb(255, 0, 0), WIDTH / 3, 150, 0, "YOU WON!");
		al_draw_textf(font, al_map_rgb(255, 0, 0), WIDTH / 3, 15, 200, "HEALTH: %s", health.c_str());
		al_draw_textf(font, al_map_rgb(255, 0, 0), WIDTH / 3, 15, 250, "FINISHED LEVEL 1 IN %i seconds", finishedTimes[0]);
		al_draw_textf(font, al_map_rgb(255, 0, 0), WIDTH / 3, 15, 300, "FINISHED LEVEL 2 IN %i seconds", finishedTimes[1]);
		al_draw_textf(font, al_map_rgb(255, 0, 0), WIDTH / 3, 15, 350, "FINISHED LEVEL 2 IN %i seconds, finishedTimes[2]");
		al_flip_display();
	}
	al_rest(7.0);
	MapFreeMem();
	al_destroy_font(font);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object

	return 0;
}



int collided(int x, int y)
{
	if (x < 0 || y < 0 || x >= mapwidth * mapblockwidth || y >= mapheight * mapblockheight)
		return 0;

	BLKSTR* blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return (blockdata->tl || blockdata->tr);
}
int collideWithCeiling(int x, int y) {

	if (x < 0 || y < 0 || x >= mapwidth * mapblockwidth || y >= mapheight * mapblockheight)
		return 0;

	BLKSTR* blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return (blockdata->bl || blockdata->br);
}

bool endValue(int x, int y)
{

	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	//if( data->user1 == 8 )
	//{
	//	return true;
	//}
	if (data->user1 == 9) {
		return true;
	}
	else {
		return false;
	}
}

void drawStatus(int xOff, int yOff, Sprite& player, string& health, bool& isDead, bool& wasHit) {
	int x = 25;
	int y= 7;
	int height = y+20;
	if (player.getHits() == 0) {
		al_draw_filled_rectangle(x, y, x + 150, height, al_map_rgb(0, 255, 0));
		health = "Perfect";
		wasHit = true;
	}
	else if (player.getHits() <= 4) {
		al_draw_filled_rectangle(x, y, x + 120, height, al_map_rgb(0, 255, 0));
		health = "Pretty Good";
		wasHit = true;
	}
	else if (player.getHits() <= 8) {
		al_draw_filled_rectangle(x, y, x + 100, height, al_map_rgb(0, 255, 0));
		health = "Mid";
		wasHit = true;
	}
	else if (player.getHits() <= 12) {
		al_draw_filled_rectangle(x, y, x + 70, height, al_map_rgb(0, 255, 0));
		health = "Careless";
		wasHit = true;
	}
	else if (player.getHits() <= 16) {
		al_draw_filled_rectangle(x, y, x + 45, height, al_map_rgb(0, 255, 0));
		health = "Dying";
		wasHit = true;
	}
	else if (player.getHits() > 16) {
		al_draw_filled_rectangle(x, y, 25, height, al_map_rgb(0, 255, 0));
		health = "Dead";
		wasHit = true;
		isDead = true;
	}
}