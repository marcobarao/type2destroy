#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include <float.h>
#include "objects.h"

#define PI 3.14159265

const int NUM_BULLETS = 5;
const int NUM_ASTEROIDS = 10;
enum KEYS { F, B, SPACE };
bool keys[3] = { false, false, false };

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* queue = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_DISPLAY_MODE mode;
ALLEGRO_FONT* font = NULL;

void initShip(SpaceShip& ship);
void drawShip(SpaceShip& ship);

void drawInfos(SpaceShip& ship);

void initAsteroids(Asteroid asteroids[], int size);
void drawAsteroids(Asteroid asteroids[], int size);
void updateAsteroids(Asteroid asteroids[], int size, SpaceShip &ship);
void createAsteroid(Asteroid asteroids[], int size);
int* generateAsteroidXY();

void initBlackHole(BlackHole &blackHole);
void drawBlackHole(BlackHole &blackHole);
void updateBlackHole(BlackHole &blackHole);
void createBlackHole(BlackHole &blackHole);
struct Coord* generateOriginDestiny();

void initBullets(Bullet bullets[], int size);
void drawBullets(Bullet bullets[], int size);
void updateBullets(Bullet bullets[], int size, Asteroid asteroids[], SpaceShip& ship);
void createBullet(Bullet bullet[], int sizeBullet, SpaceShip &ship, Asteroid asteroids[], int sizeAsteroids);
int findTarget(SpaceShip& ship, Asteroid asteroids[], int size);

int random(int lower, int upper);

void showErrorMsg(const char *text) {
	al_show_native_message_box(display, "ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

int initGame() {
	if (!al_init()) {
		showErrorMsg("Falha ao inicializar o allegro");
		return 0;
	}

	al_init_primitives_addon();

	if (!al_init_font_addon()) {
		showErrorMsg("Falha ao inicializar add-on de fontes");
		return 0;
	}

	if (!al_init_ttf_addon()) {
		showErrorMsg("Falha ao inicializar o add-on de TTF");
		return 0;
	}

	font = al_load_ttf_font("./assets/fonts/kenvector_future.ttf", 24, 0);
	if (!font) {
		showErrorMsg("Falha ao carregar a fonte");
	}

	if (!al_init_image_addon()) {
		showErrorMsg("Falha ao inicializar add-on allegro_image");
		return 0;
	}

	if (!al_install_keyboard()) {
		showErrorMsg("Falha ao inicializar o teclado");
		return 0;
	}

	al_get_display_mode(al_get_num_display_modes() - 1, &mode);

	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(mode.width, mode.height);

	if (!display) {
		showErrorMsg("Falha ao criar janela");
		return 0;
	}

	timer = al_create_timer(1.0 / 60);
	if (!timer) {
		showErrorMsg("Falha ao criar timer");
		return 0;
	}

	queue = al_create_event_queue();
	if (!queue) {
		showErrorMsg("Falha ao criar fila de eventos");
		al_destroy_display(display);
		return 0;
	}

	if (!al_install_audio()) {
		showErrorMsg("Falha ao instalar o plugin de audio");
		return 0;
	}

	if (!al_init_acodec_addon()) {
		showErrorMsg("Falha ao iniciar o codec de audio");
		return 0;
	}

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_display_event_source(display));

	al_start_timer(timer);

	return 1;
}

int main()
{
	srand(time(NULL));
	bool close = false;
	bool redraw = true;

	SpaceShip ship;
	Asteroid asteroids[NUM_ASTEROIDS];
	Bullet bullets[NUM_BULLETS];
	BlackHole blackHole;

	if (!initGame()) {
		return -1;
	}

	ALLEGRO_BITMAP* bg = NULL;
	int mapColumns = 10;
	int mapSize = 100;
	int tileSize = 256;

	ALLEGRO_SAMPLE* laser = al_load_sample("assets/audio/sfx_laser2.ogg");
	ALLEGRO_SAMPLE* env = al_load_sample("assets/audio/loop_env.wav");

	al_reserve_samples(2);

	al_play_sample(env, 0.2, 0.0, 1.3, ALLEGRO_PLAYMODE_LOOP, 0);

	initShip(ship);
	initAsteroids(asteroids, NUM_ASTEROIDS);
	initBullets(bullets, NUM_BULLETS);
	initBlackHole(blackHole);
	bg = al_load_bitmap("assets/sprites/bg.png");

	while (!close) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);

		switch (ev.type) {
			case ALLEGRO_EVENT_TIMER:
				redraw = true;

				updateAsteroids(asteroids, NUM_ASTEROIDS, ship);
				updateBlackHole(blackHole);
				updateBullets(bullets, NUM_BULLETS, asteroids, ship);
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				close = true;
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				switch (ev.keyboard.keycode) {
					case ALLEGRO_KEY_SPACE:
						keys[SPACE] = true;
						createAsteroid(asteroids, NUM_ASTEROIDS);
						break;
					case ALLEGRO_KEY_F:
						keys[F] = true;
						al_play_sample(laser, 1.0, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, 0);
						createBullet(bullets, NUM_BULLETS, ship, asteroids, NUM_ASTEROIDS);
						break;
					case ALLEGRO_KEY_B:
						keys[B] = true;
						createBlackHole(blackHole);
						break;
				}
			case ALLEGRO_EVENT_KEY_UP:
				switch (ev.keyboard.keycode) {
				case ALLEGRO_KEY_SPACE:
					keys[SPACE] = false;
					break;
				case ALLEGRO_KEY_F:
					keys[F] = false;
					break;
				}
				case ALLEGRO_KEY_B:
					keys[B] = false;
					break;
		}

		if (redraw && al_is_event_queue_empty(queue)) {
			redraw = false;

			for (int i = 0; i < mapSize; i++)
			{
				al_draw_bitmap(bg, tileSize * (i % mapColumns), tileSize * (i / mapColumns), 0);
			}

			drawBlackHole(blackHole);
			drawAsteroids(asteroids, NUM_ASTEROIDS);
			drawInfos(ship);
			drawBullets(bullets, NUM_BULLETS);
			drawShip(ship);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

	al_destroy_display(display);
	al_destroy_font(font);
	al_destroy_event_queue(queue);

	return 0;
}

void initShip(SpaceShip& ship) {
	ship.x = mode.width / 2 - 32;
	ship.y = mode.height / 2 - 32;
	ship.ID = PLAYER;
	ship.lives = 3;
	ship.score = 0;
	ship.bitmap = al_load_bitmap("assets/sprites/ship.png");
}

void drawShip(SpaceShip& ship) {
	//al_draw_scaled_bitmap(ship.bitmap,
	//	0, 0,                                // source origin
	//	al_get_bitmap_width(ship.bitmap),    // source width
	//	al_get_bitmap_height(ship.bitmap),   // source height
	//	ship.x, ship.y,                      // target origin
	//	32, 32,                              // target dimensions
	//	0                                    // flags
	//);
	al_draw_rotated_bitmap(ship.bitmap, al_get_bitmap_width(ship.bitmap) / 2, al_get_bitmap_height(ship.bitmap) / 2, ship.x, ship.y, ship.angle, 0);
}

void initAsteroids(Asteroid asteroids[], int size)
{
	for (int i = 0; i < size; i++)
	{
		asteroids[i].ID = ASTEROID;
		asteroids[i].speed = 7;
		asteroids[i].live = false;
		asteroids[i].level = 1;
		asteroids[i].bitmap = al_load_bitmap("assets/sprites/asteroid_tiny.png");
	}
}

void drawAsteroids(Asteroid asteroids[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (asteroids[i].live)
			al_draw_bitmap(asteroids[i].bitmap, asteroids[i].x, asteroids[i].y, 0);
	}
}

void createAsteroid(Asteroid asteroids[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!asteroids[i].live)
		{
			int* xy = generateAsteroidXY();
			asteroids[i].x = xy[0];
			asteroids[i].y = xy[1];
			asteroids[i].live = true;
			break;
		}
	}
}

int random(int lower, int upper) {
	return (rand() %
		(upper - lower + 1)) + lower;
}

int* generateAsteroidXY() {
	int coord[2];
	int side = random(0, 3);

	switch (side)
	{
		case 0:
			coord[0] = random(0, mode.width);
			coord[1] = 0;
			break;
		case 1:
			coord[0] = mode.width;
			coord[1] = random(0, mode.height);
			break;
		case 2:
			coord[0] = random(0, mode.width);
			coord[1] = mode.height;
			break;
		case 3:
			coord[0] = 0;
			coord[1] = random(0, mode.height);
			break;
	}

	return coord;
}

void updateAsteroids(Asteroid asteroids[], int size, SpaceShip &ship)
{
	for (int i = 0; i < size; i++)
	{
		if (asteroids[i].live)
		{
			int deltaX = ship.x - asteroids[i].x;
			int deltaY = ship.y - asteroids[i].y;
			float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
			float directionX = deltaX / distance;
			float directionY = deltaY / distance;

			asteroids[i].x = asteroids[i].x + lroundf(directionX * asteroids[i].speed);
			asteroids[i].y = asteroids[i].y + lroundf(directionY * asteroids[i].speed);

			if (asteroids[i].x > ship.x - 12 &&
				asteroids[i].x < ship.x + 12 &&
				asteroids[i].y > ship.y - 12 &&
				asteroids[i].y < ship.y + 12) {
				asteroids[i].live = false;
				ship.lives--;
			}
		}
	}
}

void drawInfos(SpaceShip& ship)
{
	char lives[12];
	sprintf_s(lives, "%d", ship.lives);
	al_draw_text(font, al_map_rgb(255, 255, 255), 50, 13, 0, lives);

	al_draw_text(font, al_map_rgb(255, 255, 255), (mode.width / 2 - 90), 25, 0, "Level 1");


	al_draw_scaled_bitmap(ship.bitmap,
		0, 0,                                // source origin
		al_get_bitmap_width(ship.bitmap),    // source width
		al_get_bitmap_height(ship.bitmap),   // source height
		15, 15,                              // target origin
		24, 24,                              // target dimensions
		0                                    // flags
	);
}

void initBlackHole(BlackHole& blackHole)
{
	blackHole.ID = BLACK_HOLE;
	blackHole.angle = 0;
	blackHole.live = false;
	blackHole.bitmap = al_load_bitmap("assets/sprites/black-hole.jpeg");
};

void drawBlackHole(BlackHole& blackHole)
{
	al_draw_rotated_bitmap(blackHole.bitmap,
		al_get_bitmap_width(blackHole.bitmap) / 2, al_get_bitmap_height(blackHole.bitmap) / 2,
		blackHole.origin.x, blackHole.origin.y,
		blackHole.angle,
		0);
	al_draw_rotated_bitmap(blackHole.bitmap,
		al_get_bitmap_width(blackHole.bitmap) / 2,
		al_get_bitmap_height(blackHole.bitmap) / 2,
		blackHole.destiny.x,
		blackHole.destiny.y,
		blackHole.angle,
		0);
}

void updateBlackHole(BlackHole& blackHole)
{
	blackHole.angle += (PI / 150);
}

void createBlackHole(BlackHole& blackHole)
{
	struct Coord* result = generateOriginDestiny();
	blackHole.origin = result[0];
	blackHole.destiny = result[1];
	blackHole.live = true;
}

struct Coord* generateOriginDestiny()
{
	struct Coord originDestiny[2];

	for (int i = 0; i < 2; i++)
	{
		originDestiny[i].x = random(80, mode.width - 80);
		originDestiny[i].y = random(35, mode.height - 35);
	}

	return originDestiny;
}  

void initBullets(Bullet bullets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		bullets[i].ID = BULLET;
		bullets[i].speed = 12;
		bullets[i].live = false;
		bullets[i].bitmap = al_load_bitmap("assets/sprites/laser.png");
	}
}

void drawBullets(Bullet bullets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bullets[i].live) {
			al_draw_rotated_bitmap(bullets[i].bitmap,
				al_get_bitmap_width(bullets[i].bitmap) / 2,
				al_get_bitmap_height(bullets[i].bitmap) / 2,
				bullets[i].x, bullets[i].y,
				bullets[i].angle,
				0);
		}
	}
}

void updateBullets(Bullet bullets[], int size, Asteroid asteroids[], SpaceShip &ship)
{
	for (int i = 0; i < size; i++)
	{
		if (bullets[i].live)
		{
			int bulletX = asteroids[bullets[i].target].x + al_get_bitmap_height(asteroids[bullets[i].target].bitmap) / 2;
			int bulletY = asteroids[bullets[i].target].y + al_get_bitmap_width(asteroids[bullets[i].target].bitmap) / 2;
			int deltaX = bulletX - bullets[i].x;
			int deltaY = bulletY - bullets[i].y;
			float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
			float angle = atan2(deltaX, deltaY);
			float directionX = deltaX / distance;
			float directionY = deltaY / distance;

			bullets[i].x = bullets[i].x + lroundf(directionX * bullets[i].speed);
			bullets[i].y = bullets[i].y + lroundf(directionY * bullets[i].speed);

			bullets[i].angle = PI - angle;
			ship.angle = PI - angle;

			if (bullets[i].x > asteroids[bullets[i].target].x - 16 &&
				bullets[i].x < asteroids[bullets[i].target].x + 16 &&
				bullets[i].y > asteroids[bullets[i].target].y - 16 &&
				bullets[i].y < asteroids[bullets[i].target].y + 16) {
				bullets[i].live = false;
				asteroids[bullets[i].target].live = false;
			}
		}
	}
}

void createBullet(Bullet bullet[], int sizeBullet, SpaceShip& ship, Asteroid asteroids[], int sizeAsteroids)
{
	int target = findTarget(ship, asteroids, sizeAsteroids);

	for (int i = 0; i < sizeBullet; i++)
	{
		if (!bullet[i].live && target >= 0)
		{
			bullet[i].x = ship.x;
			bullet[i].y = ship.y;
			bullet[i].live = true;
			bullet[i].target = target;
			break;
		}
	}
}

int findTarget(SpaceShip& ship, Asteroid asteroids[], int size)
{
	struct target {
		int idx;
		int min;
	};

	struct target min;
	min.idx = -1;
	min.min = INT_MAX;

	for (int i = 0; i < size; i++)
	{
		if (asteroids[i].live) {
			int deltaX = ship.x - asteroids[i].x;
			int deltaY = ship.y - asteroids[i].y;
			float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

			if (distance < min.min) {
				min.idx = i;
				min.min = distance;
			}
		}
	}

	return min.idx;
}
