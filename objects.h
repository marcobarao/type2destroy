#define PI 3.14159265

#define AFX_INIFILE_H__99976B4B_DBA1_4D1E_AA14_CBEB63042FD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int NUM_BULLETS = 5;
const int NUM_ASTEROIDS = 10;
const int mapColumns = 8;
const int mapSize = 40;
const int tileSize = 256;

//Object IDs
enum IDS { PLAYER, BLACK_HOLE, BULLET, ASTEROID };
enum GAME_STATE { MENU, PLAYING, PAUSE, GAME_OVER, ADD_HIGHSCORE, HIGHSCORE };

//Our Player
struct SpaceShip
{
	int ID;
	int x;
	int y;
	int lives;
	int speed;
	float angle;
	float step;
	float stepCount;
	float diff;
	float targetAngle;
	int level;
	int score;
	int kill;
	int target;

	ALLEGRO_BITMAP* bitmap;
};

struct Asteroid
{
	int ID;
	double x;
	double y;
	float delta;
	bool live;
	float speed;
	int level;
	ALLEGRO_USTR *word;
	int idxChar;
	ALLEGRO_BITMAP* bitmap;
};

struct Bullet
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	float angle;
	int target;
	ALLEGRO_BITMAP* bitmap;
};

struct Cursor
{
	int x;
	int y;
	ALLEGRO_BITMAP* bitmap;
};

struct Button
{
	int x;
	int y;
	ALLEGRO_BITMAP* bitmap;
};

struct Coord
{
	int x;
	int y;
};

struct Guarda {
	int x;
	int y;
};

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
ALLEGRO_FONT* wordFont;
ALLEGRO_SAMPLE* laser;
ALLEGRO_SAMPLE* env;
ALLEGRO_USTR* name = al_ustr_new("");

ALLEGRO_BITMAP* bg = NULL;
SpaceShip ship;
Asteroid asteroids[NUM_ASTEROIDS];
Bullet bullets[NUM_BULLETS];
Cursor cursor;
Button restart;

void initShip(SpaceShip& ship);
void drawShip(SpaceShip& ship);
void updateShip(SpaceShip& ship, Asteroid asteroids[]);

void initCursor(Cursor& cursor);
void hasClickedOnRestart();
void initRestart(Button& button);

void drawInfos(SpaceShip& ship);

void initAsteroids(Asteroid asteroids[], int size);
void drawAsteroids(Asteroid asteroids[], int size);
void updateAsteroids(Asteroid asteroids[], int size, SpaceShip& ship, float seconds);
void createAsteroid(Asteroid asteroids[], int size, int level, float seconds);
int* generateAsteroidXY();

void initBullets(Bullet bullets[], int size);
void drawBullets(Bullet bullets[], int size);
void updateBullets(Bullet bullets[], int size, Asteroid asteroids[], SpaceShip& ship);
void createBullet(Bullet bullet[], int sizeBullet, SpaceShip& ship, Asteroid asteroids[], int sizeAsteroids, int target);
int findBulletTarget(SpaceShip& ship, Asteroid asteroids[], int size);

void removeChar(int keyCode, Bullet bullets[], int sizeBullet, SpaceShip& ship, Asteroid asteroids[], int sizeAsteroids, ALLEGRO_SAMPLE* laser);
int findAsteroidToRemoveChar(int keyCode, SpaceShip& ship, Asteroid asteroids[], int size);

int random(int lower, int upper);
void showErrorMsg(const char* text);
void clickOnButton(Button button, Cursor cursor, void (*callback)());

void setGameState(GAME_STATE newState);
int getGameState();
void setInit(bool newInit);
bool getInit();

void playingState(ALLEGRO_EVENT& ev, bool& redraw);
void gameOverState(ALLEGRO_EVENT& ev, bool& redraw);

void hasClickedOnSave();
void hasClickedOnBack();
