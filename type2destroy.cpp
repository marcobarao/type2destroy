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
#include <Windows.h>

#define PI 3.14159265

const int NUM_BULLETS = 5;
const int NUM_ASTEROIDS = 10;

const char* tiny[] = {
	"IR", "UM", "EM", "EU", "DE", "SE", "OU", "ME", "DA", "AO", "LI", "NO", "OI", "NA", "NU", "TU", "DO", "TI", "OS", "AS", "TE", "EI", "RI", "LA", "KG", "SOB", "PAZ", "MAL", "VER", "SER", "EGO", "TER", "BEM", "VIR", "DAR", "BOM", "MAS", "ELO", "VO", "ERA", "TAL", "DIA", "ORA", "LUZ", "TEZ", "COM", "ATO", "DOR", "DOU", "EIS", "FE", "NO", "MIM", "LER", "FIZ", "PRO", "VOO", "DOM", "NUM", "HA", "SEM", "MAU", "UMA", "SO", "RUA", "JA", "SOL", "LEI", "QUE", "AJO", "LUA", "FOI", "RIO", "PAU", "NAU", "SEU", "ODE", "SA", "DO", "ECO", "VOZ", "FIM", "FEZ", "NEM", "IDO", "MEU", "SIM", "VEZ", "VIA", "BOI", "REI", "JUS", "PAI", "ASA", "SUB", "SOM", "TEM", "UNS", "ALI", "FOR", "RIR", "VO", "AGI", "AI", "POR", "LHE", "BOA", "USO", "TOA", "SAL", "SE", "RE", "AMO", "UFA", "SUA", "VAN", "PRA", "COR", "CIA", "LA", "CAL", "OVO", "NOS", "PAR", "TOM", "KIT", "FIO", "MAR", "IRA", "GIZ", "LAR", "UAU", "TA", "RIM", "ANO", "PUS", "NOZ", "TOP", "MEL", "ELE", "CEM", "FA", "PIA", "CIO", "OBA", "FOZ", "TIO", "ELA", "IDA", "AVE", "TO", "PE", "TUA", "PO", "UVA", "DUM", "MA", "PA", "TEU", "ARO", "VE", "CRU", "GOL", "DEZ", "AMA", "TIA", "RA", "EXU", "PIO", "AS", "NE", "DEU", "LEU", "AOS", "ZEN", "NAS", "CA", "GAZ", "OCO", "ALA", "DEI", "MIL", "RUM", "OPA", "LIA", "UE", "NUA", "SUL", "OCA", "TE", "WEB", "RIA", "FAX", "PO", "SAI", "TIL", "GEL", "LA", "OVA", "XIS"
};

const char* smalll[] = {
	"AMOR", "FATO", "MITO", "CEU", "SEDE", "NAO", "APTO", "POIS", "CRER", "VAO", "CAOS", "AUGE", "REU", "TOLO", "COTA", "RUIM", "ENTE", "SOAR", "URGE", "PUDE", "VIDA", "CEDO", "COMO", "RIMA", "ZELO", "MEDO", "SELA", "CELA", "FASE", "CUJO", "NOJO", "ONDE", "FACE", "POSE", "ALVA", "TEVE", "CASA", "ALVO", "BASE", "NEXO", "TEOR", "NUMA", "MAE", "ANTE", "REGE", "AUTO", "TUDO", "RUDE", "VALE", "SAGA", "MAIS", "TRAZ", "IDEM", "TODO", "ALTA", "LOGO", "MERO", "ALTO", "DOCE", "DEUS", "PARA", "JOIA", "META", "FORA", "EITA", "PROL", "ALMA", "PELO", "NOVO", "ALGO", "FRIO", "PELA", "TESE", "HAJA", "ATE", "SAGAZ", "MEXER", "TERMO", "SENSO", "NOBRE", "PLENA", "AFETO", "SUTIL", "DESDE", "INATO", "VIGOR", "SERIA", "FAZER","IDEIA", "SANAR", "ANEXO", "APOS", "TORPE", "PODER", "JUSTO", "LAPSO", "ASSIM", "HONRA", "MORAL", "MUITO", "EXPOR", "POSSE", "DIGNO", "PESAR", "GENRO", "AREA", "DIZER", "ALEM", "FOSSE", "CAUSA", "DENSO", "CEDER", "COMUM", "DEVER", "CENSO", "CULTO", "SABER", "LOUCO", "FLUIR", "ONTEM", "MANSO", "IMPOR", "PLENO", "REGRA", "JEITO", "DESSE", "SONHO", "VALHA", "TEMOR", "PUDOR", "MUNDO", "SOBRE", "CISMA", "TEMPO", "CRIAR", "SENDO", "ENFIM", "FORTE", "SERVO", "AINDA", "ETNIA", "GERAR", "VOCE", "ESTAR", "SENIL", "OBTER", "PEDIR", "FALAR", "VISAR", "AMIGO"
};

const char* medium[] = {
	"EXCETO", "VEREDA", "AMAGO", "EXITO", "PRESSA", "INFAME", "CASUAL", "ADORNO", "NOCIVO", "ALHEIO", "ESCOPO", "GENTIL", "MUTUA", "HOSTIL", "POREM", "DIFUSO", "LEGADO", "AFERIR", "FORMAL", "SOLENE", "ETICA", "EFICAZ", "ASTUTO", "JULGAR", "ABSTER", "LIMIAR", "DISPOR", "ACAO", "OCIOSO", "MUTUO", "EXIMIR", "ISENTO", "RECEIO", "ACENTO", "DETEM", "RAZAO", "EMBORA", "ALOCAR", "FUTIL", "HABIL", "OBJETO", "RANCOR", "PROVER", "COAGIR", "ANSIA", "MAROTO", "VEDADO", "ICONE", "BUSCAR", "VULGAR", "EMPATIA", "PROLIXO", "AMBITO", "CINICO", "SUBLIME", "SUCINTO", "INDOLE", "CONVEM", "RECESSO", "CINISMO", "INFERIR", "MERITO", "REFUTAR", "CORDIAL", "ENFASE", "EMERGIR", "VERBETE", "ADESAO", "TRIVIAL", "SECAO", "JUBILO", "ASPECTO", "EXCESSO", "EXILADO", "REDIMIR", "ALMEJAR", "CONCISO", "VIRTUDE", "PADECER", "DADIVA", "COLOSSO", "CESSAO", "MITIGAR", "ALCUNHA", "SALUTAR", "INCAUTO", "SENSATO", "SADICO", "HABITO", "INTENSO", "ASSENTO", "PAIXAO", "EMOTIVO", "EXTASE", "EXILIO", "INDAGAR", "HESITAR", "PARCIAL", "SUCESSO", "GLORIA"
};

const char* big[] = {
	"INERENTE", "PECULIAR", "PRUDENTE", "PESAMES", "DEFERIDO", "INVASIVO", "GENUINO", "RESPEITO", "ALIENADO", "REITERAR", "AUDACIA", "PRODIGO", "ABSTRATO", "ESTAVEL", "CONSERTO", "APOLOGIA", "ASTUCIA", "CONCEITO", "PREMISSA", "DEVANEIO", "PERSPICAZ", "RETIFICAR", "PLENITUDE", "EXTENSAO", "ESSENCIAL", "PARADIGMA", "HEGEMONIA", "RATIFICAR", "DELIBERAR", "PRODIGIO", "INCIDENTE", "RESIGNADO", "PERSUADIR", "DICOTOMIA", "PASSIVEL", "DEMASIADO", "ASCENSAO", "DESDENHAR", "MESQUINHO", "MODESTIA", "PRESCINDIR", "IMPRESSAO", "CORROBORAR", "CONCESSAO", "SUPERFLUO", "ESCRUPULO", "IMPLICITO", "DETRIMENTO", "MATURIDADE", "INDULGENTE"
};

const char** words[] = { tiny, smalll, medium, big };

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
void updateAsteroids(Asteroid asteroids[], int size, SpaceShip& ship, float seconds);
void createAsteroid(Asteroid asteroids[], int size, int level, float seconds);
int* generateAsteroidXY();

void initBlackHole(BlackHole& blackHole);
void drawBlackHole(BlackHole& blackHole);
void updateBlackHole(BlackHole& blackHole);
void createBlackHole(BlackHole& blackHole);
struct Coord* generateOriginDestiny();

void initBullets(Bullet bullets[], int size);
void drawBullets(Bullet bullets[], int size);
void updateBullets(Bullet bullets[], int size, Asteroid asteroids[], SpaceShip& ship);
void createBullet(Bullet bullet[], int sizeBullet, SpaceShip& ship, Asteroid asteroids[], int sizeAsteroids);
int findBulletTarget(SpaceShip& ship, Asteroid asteroids[], int size);
void updateShip(BlackHole& blackhole, SpaceShip& ship);
void hasClickedOnBlackHole(Cursor& cursor, SpaceShip& ship, BlackHole& blackHole);

void removeChar(int keyCode, Bullet bullets[], int sizeBullet, SpaceShip& ship, Asteroid asteroids[], int sizeAsteroids, ALLEGRO_SAMPLE* laser);
int findAsteroidToRemoveChar(int keyCode, SpaceShip& ship, Asteroid asteroids[], int size);

int random(int lower, int upper);

void showErrorMsg(const char* text) {
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
	char input[1] = "";

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

	al_reserve_samples(2);

	ALLEGRO_SAMPLE* laser = al_load_sample("assets/audio/sfx_laser2.ogg");
	ALLEGRO_SAMPLE* env = al_load_sample("assets/audio/loop_env.wav");

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

			if (((int)seconds == seconds) && ((int)seconds) % 2 == 0) {
				createAsteroid(asteroids, NUM_ASTEROIDS, ship.level, seconds);
			}

			updateAsteroids(asteroids, NUM_ASTEROIDS, ship, seconds);
			updateBlackHole(blackHole);
			updateBullets(bullets, NUM_BULLETS, asteroids, ship);
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			close = true;
			break;
		case ALLEGRO_EVENT_KEY_UP:
			removeChar(ev.keyboard.keycode + 64, bullets, NUM_BULLETS, ship, asteroids, NUM_ASTEROIDS, laser);
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			cursor.x = ev.mouse.x;
			cursor.y = ev.mouse.y;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			hasClickedOnRestart(cursor, restart, ship, asteroids, NUM_ASTEROIDS, bullets, NUM_BULLETS, blackHole, gameover);
			hasClickedOnBlackHole(cursor, ship, blackHole);
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
				al_draw_text(font[4], al_map_rgb(255, 255, 255), (mode.width / 2 - midX), mode.height / 2 - (midY + 60), 20, string);
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
	button.y = mode.height / 2 + 20;
	button.bitmap = al_load_bitmap("assets/sprites/restart.png");
}

void initShip(SpaceShip& ship) {
	ship.x = mode.width / 2;
	ship.y = mode.height / 2;
	ship.ID = PLAYER;
	ship.lives = 5;
	ship.kill = 0;
	ship.level = 1;
	ship.speed = 8;
	ship.bitmap = al_load_bitmap("assets/sprites/ship.png");
}

void drawShip(SpaceShip& ship) {
	al_draw_scaled_rotated_bitmap(ship.bitmap,
		al_get_bitmap_width(ship.bitmap) / 4, al_get_bitmap_height(ship.bitmap) / 4,
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
		asteroids[i].idxChar = 0;
		asteroids[i].word = al_ustr_new("");
		asteroids[i].bitmap = al_load_bitmap("assets/sprites/asteroid_1.png");
	}
}

void drawAsteroids(Asteroid asteroids[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (asteroids[i].live) {
			al_draw_bitmap(asteroids[i].bitmap, asteroids[i].x, asteroids[i].y, 0);
			int x = asteroids[i].x + al_get_bitmap_width(asteroids[i].bitmap) / 2 - al_get_ustr_width(font[0], asteroids[i].word) / 2;
			int y = asteroids[i].y + al_get_bitmap_height(asteroids[i].bitmap) + 10;
			al_draw_ustr(font[0], al_map_rgb(255, 255, 255), x, y, 0, asteroids[i].word);
			al_draw_ustr(font[0], al_map_rgb(255, 255, 0), x, y, 0, al_ustr_dup_substr(asteroids[i].word, 0, asteroids[i].idxChar));
		}
	}
}

void createAsteroid(Asteroid asteroids[], int size, int level, float seconds)
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
				asteroids[j].delta = seconds;

				asteroids[j].level = random(1, 4);

				asteroids[j].speed = max(1.25, (5 - asteroids[j].level) / 2);

				asteroids[j].idxChar = 0;

				asteroids[j].word = al_ustr_new(words[asteroids[j].level - 1][random(0, 199 - (asteroids[j].level - 1) * 50)]);

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

void updateAsteroids(Asteroid asteroids[], int size, SpaceShip& ship, float seconds)
{
	for (int i = 0; i < size; i++)
	{
		if (asteroids[i].live)
		{
			//int deltaX = ship.x - asteroids[i].x;
			//int deltaY = ship.y - asteroids[i].y;
			//float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
			//float directionX = deltaX / distance;
			//float directionY = deltaY / distance;

			//asteroids[i].x = asteroids[i].x + lroundf(directionX * asteroids[i].speed);
			//asteroids[i].y = asteroids[i].y + lroundf(directionY * asteroids[i].speed);

			double theta = atan2(ship.y - asteroids[i].y, ship.x - asteroids[i].x);
			float delta = seconds - asteroids[i].delta;

			asteroids[i].x += (delta * asteroids[i].speed) * cos(theta);
			asteroids[i].y += (delta * asteroids[i].speed) * sin(theta);

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

void updateShip(BlackHole& blackhole, SpaceShip& ship)
{
	if (blackhole.live)
	{
		int deltaX = ship.x - blackhole.origin.x;
		int deltaY = ship.y - blackhole.origin.y;
		float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
		float directionX = deltaX / distance;
		float directionY = deltaY / distance;

		int deltaX2 = ship.x - blackhole.destiny.x;
		int deltaY2 = ship.y - blackhole.destiny.y;
		float distance2 = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
		float directionX2 = deltaX2 / distance2;
		float directionY2 = deltaY2 / distance2;



		ship.x = blackhole.origin.x + lroundf(directionX * ship.speed);
		ship.y = blackhole.origin.y + lroundf(directionY * ship.speed);

		Sleep(1);

		ship.x = blackhole.destiny.x + lroundf(directionX2 * ship.speed);
		ship.y = blackhole.destiny.y + lroundf(directionY2 * ship.speed);

		int shipWidth = al_get_bitmap_width(ship.bitmap) / 2;
		int shipHeight = al_get_bitmap_height(ship.bitmap) / 2;

		if (ship.x > ship.x - shipWidth &&
			ship.x < ship.x + shipWidth &&
			ship.y > ship.y - shipHeight &&
			ship.y < ship.y + shipHeight) {
			blackhole.live = false;
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

void hasClickedOnBlackHole(
	Cursor& cursor,
	SpaceShip& ship,
	BlackHole& blackHole
)
{
	int endXButton = blackHole.origin.x + al_get_bitmap_width(blackHole.bitmap);
	int endYButton = blackHole.origin.y + al_get_bitmap_height(blackHole.bitmap) + 10;

	bool onXRange = cursor.x > blackHole.origin.x && cursor.x < endXButton;
	bool onYRange = cursor.y > blackHole.origin.y && cursor.y < endYButton;
	if (onXRange && onYRange) {
		updateShip(blackHole, ship);
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

void updateBullets(Bullet bullets[], int size, Asteroid asteroids[], SpaceShip& ship)
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
	int target = findBulletTarget(ship, asteroids, sizeAsteroids);

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

int findBulletTarget(SpaceShip& ship, Asteroid asteroids[], int size)
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

			if (distance < min.min && asteroids[i].idxChar >= al_ustr_length(asteroids[i].word)) {
				min.idx = i;
				min.min = distance;
			}
		}
	}

	return min.idx;
}

void removeChar(int keyCode, Bullet bullets[], int sizeBullet, SpaceShip& ship, Asteroid asteroids[], int sizeAsteroids, ALLEGRO_SAMPLE* laser) {
	int index = findAsteroidToRemoveChar(keyCode, ship, asteroids, sizeAsteroids);
	if (index >= 0) {
		asteroids[index].idxChar++;

		if (asteroids[index].idxChar >= al_ustr_length(asteroids[index].word)) {
			al_play_sample(laser, 1, 0.0, 1.3, ALLEGRO_PLAYMODE_ONCE, 0);
			createBullet(bullets, sizeBullet, ship, asteroids, sizeAsteroids);
		}
	}
}

int findAsteroidToRemoveChar(int keyCode, SpaceShip& ship, Asteroid asteroids[], int size) {
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

			int nextKeyCode = al_ustr_get(asteroids[i].word, asteroids[i].idxChar);
			if (distance < min.min && keyCode == nextKeyCode) {
				min.idx = i;
				min.min = distance;
			}
		}
	}

	return min.idx;
}