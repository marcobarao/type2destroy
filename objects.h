//Object IDs
enum IDS { PLAYER, BLACK_HOLE, BULLET, ASTEROID };

//Our Player
struct SpaceShip
{
	int ID;
	int x;
	int y;
	int lives;
	int speed;
	float angle;
	int score;
	ALLEGRO_BITMAP* bitmap;
};

struct Asteroid
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int level;
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

struct Coord
{
	int x;
	int y;
};

struct BlackHole
{
	int ID;
	struct Coord origin;
	struct Coord destiny;
	float angle;
	bool live;
	ALLEGRO_BITMAP* bitmap;
};
