#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include "Sprite.h"

//Lucia Donley

Sprite::Sprite()
{
	sample = NULL;
	image = NULL;

}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
	al_destroy_sample(sample);
}

void Sprite::InitSprites(int width, int height)
{
	x = 120;
	y = 60;

	//need to check if dir == something for jumping
	//maxFrame = 12
	//new if statement in draw function for jumping
	curAngle = 0;
	destAngle = 0;
	currentlyTurning = false;
	turned = 0;
	maxFrame = 16;
	curFrame = 0;
	//jumpFrame = 8;
	//jumpFrameDelay = 6;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 333;
	frameHeight = 499;
	animationColumns = 4;
	animationDirection = 0;
	speed = 10;
	hits = 0;
	sample = al_load_sample("bamboo_stick.flac");

	if (!sample) {
		exit(9);
	}

	image = al_load_bitmap("man_walking.png");
	al_convert_mask_to_alpha(image, al_map_rgb(254, 254, 254));
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;

	if (dir == 3) { //right key
		//curFrame = 12;Z
		animationDirection = 3;
		goingLeft = false;
		//curAngle = 0;
		//destAngle = 0;
		x += speed;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame >= maxFrame)
				curFrame = 12;
		}
	}
	else if (dir == 2) { //left key
		//curFrame = 8;
		animationDirection = 2;
		goingLeft = true;
		//curAngle = 0;
		//destAngle = 0;
		x -= speed;
		if (++frameCount >= frameDelay)
		{
			frameCount = 0;
			if (++curFrame >= 12)
				curFrame = 8;
		}

	}
	else if (dir == 1) {//up key
		//curFrame = 4;
		//curAngle = -ALLEGRO_PI / 2;
		//destAngle = -ALLEGRO_PI / 2;
		animationDirection = 1;
		y -= speed;
		if (++frameCount >= frameDelay)
		{
			frameCount = 0;
			if (++curFrame >= 8)
				curFrame = 4;
		}
	}

	else if (dir == 0) {//down key
		//curFrame = 0;
		//curAngle = ALLEGRO_PI / 2;
		//destAngle = ALLEGRO_PI / 2;
		animationDirection = 0;
		y += speed;
		if (++frameCount >= frameDelay)
		{
			frameCount = 0;
			if (++curFrame >= 4)
				curFrame = 0;
		}
	}

	else { //represent that they hit the space bar and that mean direction = 0
		goingLeft = false;
		animationDirection = dir;

		//isJumping = false;
	}

	charWidth = 48;
	charHeight = 72;

	//check for collided with foreground tiles
	if (animationDirection == 2)
	{
		if (collided(x, y + charWidth / 2)) { //collision detection to the left
			al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			x = oldx;
			y = oldy;
			increaseHits();
			currentlyTurning = true;
			destAngle += ALLEGRO_PI;
			//destAngle += ALLEGRO_PI;
		}
		//curAngle = 0;
		//destAngle = 0;
	}
	else if (animationDirection == 3)
	{
		if (collided(x + charWidth, y + 10) || collided(x + charWidth, y + charHeight - 10) || collided(x + charWidth, y + charHeight / 2)) { //collision detection to the right
			al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			x = oldx;
			y = oldy;
			currentlyTurning = true;
			increaseHits();
			destAngle += ALLEGRO_PI;
		}
		//curAngle = ALLEGRO_PI;
		//destAngle = ALLEGRO_PI;
		
	}

	if (animationDirection == 1)
	{
		if (collided(x, y) || collided(x + charWidth, y)) { //collision detection up
			al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			x = oldx;
			y = oldy;
			//destAngle += ALLEGRO_PI;
			//currentlyTurning = true;
			increaseHits();
			
		}
		//curAngle = ALLEGRO_PI / 2;
		//destAngle = ALLEGRO_PI / 2;

	}
	if (animationDirection == 0) {
		if (collided(x, y + charHeight) || collided(x + charWidth, y + charHeight)) {
			al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			x = oldx;
			y = oldy;
			//destAngle += ALLEGRO_PI;
			//currentlyTurning = true;

			increaseHits();
		}
		//curAngle = -ALLEGRO_PI / 2;
		//destAngle = -ALLEGRO_PI / 2;
	}
	if (currentlyTurning) {
		if (curAngle <= destAngle) {
			curAngle += 0.05f;

			if (curAngle >= destAngle) {
				curAngle = destAngle;
				currentlyTurning = false;
			}
		}
	}
	//collision detection down?

}

void Sprite::increaseHits() {
	hits++;
}

int Sprite::getHits() {
	return hits;
}

bool Sprite::CollisionEndBlock()
{

	if (endValue(x + charWidth / 2, y + charHeight + 5)) {
		sample = NULL;
		return true;
	}
	else
		return false;
}

int Sprite::getBoundX() {
	return al_get_bitmap_width(image);
}

int Sprite::getBoundY() {
	return al_get_bitmap_height(image);
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{

	float newWidth = 48.0f;
	float newHeight = 72.0f;
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = animationDirection * frameHeight;

	float minimizeX = (float)newWidth / charWidth;
	float minimizeY = (float)newHeight / charHeight;


	if (animationDirection == 5 && getHits() == 0) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 0, frameWidth, frameHeight, al_map_rgb(255, 255, 255), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f), (72.0f / 499.0f), curAngle, 0);
	}
	else if (animationDirection == 5 && getHits() <= 4) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 0, frameWidth, frameHeight, al_map_rgb(255, 191, 191), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f)/2, (72.0f / 499.0f)/2, curAngle, 0);
	}
	else if (animationDirection == 5 && getHits() <= 8) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 0, frameWidth, frameHeight, al_map_rgb(231, 133, 135), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f)/2, (72.0f / 499.0f)/2, curAngle, 0);
	}
	else if (animationDirection == 5 && getHits() <= 12) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 0, frameWidth, frameHeight, al_map_rgb(255, 0, 0), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f)/2, (72.0f / 499.0f)/2, curAngle, 0);
	}
	else if (animationDirection == 5 && getHits() <= 16) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 0, frameWidth, frameHeight, al_map_rgb(200, 0, 0), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f)/2, (72.0f / 499.0f)/2, curAngle, 0);
	}
	else if (animationDirection == 5 && getHits() > 16) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 0, frameWidth, frameHeight, al_map_rgb(150, 0, 0), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f)/2, (72.0f / 499.0f)/2, curAngle, 0);
	}
	else if (animationDirection < 5 && getHits() == 0) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, fx, fy, frameWidth, frameHeight, al_map_rgb(255, 255, 255), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f), (72.0f / 499.0f), curAngle, 0);
	}
	else if (animationDirection < 5 && getHits() <= 4) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, fx, fy, frameWidth, frameHeight, al_map_rgb(255, 191, 191), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f) / 2, (72.0f / 499.0f) / 2, curAngle, 0);
	}
	else if (animationDirection < 5 && getHits() <= 8) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, fx, fy, frameWidth, frameHeight, al_map_rgb(231, 133, 135), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f) / 2, (72.0f / 499.0f) / 2, curAngle, 0);
	}
	else if (animationDirection < 5 && getHits() <= 12) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, fx, fy, frameWidth, frameHeight, al_map_rgb(255, 0, 0), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f) / 2, (72.0f / 499.0f) / 2, curAngle, 0);
	}
	else if (animationDirection < 5 && getHits() <= 16) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, fx, fy, frameWidth, frameHeight, al_map_rgb(200, 0, 0), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f) / 2, (72.0f / 499.0f) / 2, curAngle, 0);
	}
	else if (animationDirection < 5 && getHits() > 16) {
		al_draw_tinted_scaled_rotated_bitmap_region(image, fx, fy, frameWidth, frameHeight, al_map_rgb(150, 0, 0), 0, 0, x - xoffset, y - yoffset, (48.0f / 333.0f) / 2, (72.0f / 499.0f) / 2, curAngle, 0);
	}
}