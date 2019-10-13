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
ALLEGRO_FONT* font[5];

void initShip(SpaceShip& ship);
void drawShip(SpaceShip& ship);

void initCursor(Cursor& cursor);
void hasClickedOnRestart(
	Cursor& cursor,
	Button& button,
	SpaceShip& ship,
	Asteroid asteroids[],
	int NUM_ASTEROIDS,
	Bullet bullets[],
	int NUM_BULLETS,
	BlackHole& blackHole,
	bool& gameover
);
void initRestart(Button& button);

void drawInfos(SpaceShip& ship);

void initAsteroids(Asteroid asteroids[], int size);
void drawAsteroids(Asteroid asteroids[], int size);
void updateAsteroids(Asteroid asteroids[], int size, SpaceShip &ship);
void createAsteroid(Asteroid asteroids[], int size, int level);
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

	for (int i = 0; i < 5; i++) {
		font[i] = al_load_ttf_font("./assets/fonts/kenvector_future.ttf", 24 + i * 8, 0);
		if (!font[i]) {
			showErrorMsg("Falha ao carregar a fonte");
		}
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

	if (!al_install_mouse()) {
		showErrorMsg("Falha ao inicializar o mouse");
		return -1;
	}

	if (!al_hide_mouse_cursor(display)) {
		showErrorMsg("Falha ao esconder o cursor do mouse");
		return -1;
	}

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_mouse_event_source());

	al_start_timer(timer);

	return 1;
}

int main()
{
	bool close = false;
	bool gameover = false;
	bool redraw = true;

	SpaceShip ship;
	Asteroid asteroids[NUM_ASTEROIDS];
	Bullet bullets[NUM_BULLETS];
	BlackHole blackHole;
	Cursor cursor;
	Button restart;

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
	initCursor(cursor);
	initRestart(restart);
	initAsteroids(asteroids, NUM_ASTEROIDS);
	initBullets(bullets, NUM_BULLETS);
	initBlackHole(blackHole);
	bg = al_load_bitmap("assets/sprites/bg.png");

	while (!close) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);
		float seconds;
		
		switch (ev.type) {
			case ALLEGRO_EVENT_TIMER:
				redraw = true;
				seconds = al_get_timer_count(timer) / 100.0;
				srand(time(NULL));

				if (ship.kill > ship.level * 4) {
					ship.level++;
				}

				if (((int) seconds == seconds) && ((int) seconds) % 2 == 0) {
					createAsteroid(asteroids, NUM_ASTEROIDS, ship.level);
				}

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
						createAsteroid(asteroids, NUM_ASTEROIDS, ship.level);
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
				case ALLEGRO_EVENT_MOUSE_AXES:
					cursor.x = ev.mouse.x;
					cursor.y = ev.mouse.y;
					break;
				case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
					hasClickedOnRestart(cursor, restart, ship, asteroids, NUM_ASTEROIDS, bullets, NUM_BULLETS, blackHole, gameover);
					break;
		}

		if (redraw && al_is_event_queue_empty(queue)) {
			redraw = false;
			gameover = ship.lives <= 0;

			for (int i = 0; i < mapSize; i++)
			{
				al_draw_bitmap(bg, tileSize * (i % mapColumns), tileSize * (i / mapColumns), 0);
			}

			if (gameover) {
				char string[] = "GAMEOVER";
				al_clear_to_color(al_map_rgba(0, 0, 0, 0.5));
				int midX = al_get_text_width(font[4], string) / 2;
				int midY = al_get_font_line_height(font[4]) / 2;
				al_draw_text(font[4], al_map_rgb(255, 255, 255), (mode.width / 2 - midX), mode.height / 2 - midY, 20, string);
				al_draw_bitmap(restart.bitmap, restart.x, restart.y, 0);
			}
			else {
				drawBlackHole(blackHole);
				drawAsteroids(asteroids, NUM_ASTEROIDS);
				drawInfos(ship);
				drawBullets(bullets, NUM_BULLETS);
				drawShip(ship);
			}

			al_draw_bitmap(cursor.bitmap, cursor.x, cursor.y, 0);


			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

	al_destroy_display(display);
	for (int i = 0; i < 5; i++) {
		al_destroy_font(font[i]);
	}
	al_destroy_event_queue(queue);

	return 0;
}

void initRestart(Button& button)
{
	button.x = mode.width / 2 - 140;
	button.y = mode.height / 2 + 80;
	button.bitmap = al_load_bitmap("assets/sprites/restart.png");
}

void initShip(SpaceShip& ship) {
	ship.x = mode.width / 2 - 32;
	ship.y = mode.height / 2 - 32;
	ship.ID = PLAYER;
	ship.lives = 3;
	ship.kill = 0;
	ship.level = 1;
	ship.bitmap = al_load_bitmap("assets/sprites/ship.png");
}

void drawShip(SpaceShip& ship) {
	al_draw_scaled_rotated_bitmap(ship.bitmap,
		al_get_bitmap_width(ship.bitmap) / 2, al_get_bitmap_height(ship.bitmap) / 2,
		ship.x, ship.y, 0.5, 0.5,
		ship.angle, 0);
}

void initAsteroids(Asteroid asteroids[], int size)
{
	for (int i = 0; i < size; i++)
	{
		asteroids[i].ID = ASTEROID;
		asteroids[i].speed = 8;
		asteroids[i].live = false;
		asteroids[i].level = 1;
		asteroids[i].bitmap = al_load_bitmap("assets/sprites/asteroid_1.png");
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

void createAsteroid(Asteroid asteroids[], int size, int level)
{
	int qtAsteroids = random(1, 3);

	for (int i = 1; i <= qtAsteroids; i++) {
		for (int j = 0; j < size; j++)
		{
			if (!asteroids[j].live)
			{
				int* xy = generateAsteroidXY();
				asteroids[j].x = xy[0];
				asteroids[j].y = xy[1];

				asteroids[j].level = random(1, 4);

				asteroids[j].speed = 2 * 3 / asteroids[j].level + 1;

				char path[100];
				sprintf_s(path, "assets/sprites/asteroid_%d.png", asteroids[j].level);
				asteroids[j].bitmap = al_load_bitmap(path);
				asteroids[j].live = true;
				break;
			}
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

			int shipWidth = al_get_bitmap_width(ship.bitmap) / 2;
			int shipHeight = al_get_bitmap_height(ship.bitmap) / 2;

			if (asteroids[i].x > ship.x - shipWidth &&
				asteroids[i].x < ship.x + shipWidth &&
				asteroids[i].y > ship.y - shipHeight &&
				asteroids[i].y < ship.y + shipHeight) {
				asteroids[i].live = false;
				ship.lives--;
			}
		}
	}
}

void initCursor(Cursor& cursor)
{
	cursor.bitmap = al_load_bitmap("assets/sprites/cursor.png");
}

void hasClickedOnRestart(
	Cursor& cursor,
	Button& button,
	SpaceShip& ship,
	Asteroid asteroids[],
	int NUM_ASTEROIDS,
	Bullet bullets[],
	int NUM_BULLETS,
	BlackHole& blackHole,
	bool& gameover
)
{
	int endXButton = button.x + al_get_bitmap_width(button.bitmap);
	int endYButton = button.y + al_get_bitmap_height(button.bitmap);

	bool onXRange = cursor.x > button.x && cursor.x < endXButton;
	bool onYRange = cursor.y > button.y && cursor.y < endYButton;
	if (onXRange && onYRange) {
		initShip(ship);
		initAsteroids(asteroids, NUM_ASTEROIDS);
		initBullets(bullets, NUM_BULLETS);
		initBlackHole(blackHole);
		gameover = false;
	}
}

void drawInfos(SpaceShip& ship)
{
	char lives[12];
	char level[14];
	sprintf_s(lives, "%d", ship.lives);
	al_draw_text(font[2], al_map_rgb(255, 255, 255), 90, 30, 0, lives);

	sprintf_s(level, "Level %d", ship.level);
	int midX = al_get_text_width(font[2], level) / 2;
	al_draw_text(font[2], al_map_rgb(255, 255, 255), (mode.width / 2 - midX), 32, 0, level);


	al_draw_scaled_bitmap(ship.bitmap,
		0, 0,                                // source origin
		al_get_bitmap_width(ship.bitmap),    // source width
		al_get_bitmap_height(ship.bitmap),   // source height
		32, 32,                              // target origin
		40, 40,                              // target dimensions
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
	al_draw_scaled_rotated_bitmap(blackHole.bitmap,
		al_get_bitmap_width(blackHole.bitmap) / 2, al_get_bitmap_height(blackHole.bitmap) / 2,
		blackHole.origin.x, blackHole.origin.y, 0.5, 0.5,
		blackHole.angle, 0);
	al_draw_scaled_rotated_bitmap(blackHole.bitmap,
		al_get_bitmap_width(blackHole.bitmap) / 2, al_get_bitmap_height(blackHole.bitmap) / 2,
		blackHole.destiny.x, blackHole.destiny.y, 0.5, 0.5,
		blackHole.angle, 0);
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

			int asteroidWidth = al_get_bitmap_width(asteroids[bullets[i].target].bitmap);
			int asteroidHeight = al_get_bitmap_height(asteroids[bullets[i].target].bitmap);

			if (bullets[i].x > asteroids[bullets[i].target].x &&
				bullets[i].x < asteroids[bullets[i].target].x + asteroidWidth &&
				bullets[i].y > asteroids[bullets[i].target].y &&
				bullets[i].y < asteroids[bullets[i].target].y + asteroidHeight) {
				bullets[i].live = false;
				asteroids[bullets[i].target].live = false;
				ship.kill++;
				ship.score += asteroids[bullets[i].target].level * 2;
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
