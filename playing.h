void playingState(ALLEGRO_EVENT &ev, bool &redraw) {
	if(getInit()) {
		setInit(false);
		al_reserve_samples(5);
		laser = al_load_sample("assets/audio/sfx_laser2.ogg");
		env = al_load_sample("assets/audio/loop_env.wav");
		shipBoom = al_load_sample("assets/audio/shipBoom.wav");
		boom = al_load_sample("assets/audio/boom.wav");
		lose = al_load_sample("assets/audio/sfx_lose.ogg");
		initShip(ship);
		initAsteroids(asteroids, NUM_ASTEROIDS);
		initBullets(bullets, NUM_BULLETS);
		initPauseButton(pauseGame);
		al_stop_sample(&envId);
		al_play_sample(env, 0.2, 0.0, 1.3, ALLEGRO_PLAYMODE_LOOP, &envId);
	}

	float seconds;
	switch (ev.type) {
		case ALLEGRO_EVENT_TIMER:
			seconds = al_get_timer_count(timer) / 100.0;
			srand(time(NULL));
			if (ship.kill > ship.level * 4) ship.level++;
			if (((int)seconds == seconds) && ((int)seconds) % 3 == 0) createAsteroid(asteroids, NUM_ASTEROIDS, ship.level, seconds);

			updateShip(ship, asteroids);
			updateAsteroids(asteroids, NUM_ASTEROIDS, ship, seconds);
			updateBullets(bullets, NUM_BULLETS, asteroids, ship);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				setGameState(PAUSE);
				break;
			}
			removeChar(ev.keyboard.keycode + 64, bullets, NUM_BULLETS, ship, asteroids, NUM_ASTEROIDS, laser);
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			clickOnButton(pauseGame, cursor, hasClickedOnPause);
			break;
	}

	if (redraw && al_is_event_queue_empty(queue)) {
		if (ship.lives <= 0) {
			if (ship.score > highScoreScores[9]) {
				setGameState(ADD_HIGHSCORE);
			}
			else {
				setGameState(GAME_OVER);
				al_play_sample(lose, 1, 0.0, 1.3, ALLEGRO_PLAYMODE_ONCE, 0);
			}
		}

		drawAsteroids(asteroids, NUM_ASTEROIDS);
		drawInfos(ship);
		drawBullets(bullets, NUM_BULLETS);
		drawShip(ship);
		if (explosion.lives > 0) {
			explosion.lives--;
			al_draw_bitmap(explosion.bitmap, explosion.x, explosion.y, 0);
		}
		al_draw_scaled_bitmap(
			pauseGame.bitmap,
			0, 0,
			al_get_bitmap_width(pauseGame.bitmap), al_get_bitmap_height(pauseGame.bitmap),
			pauseGame.x, pauseGame.y,
			al_get_bitmap_width(pauseGame.bitmap) * 0.33, al_get_bitmap_height(pauseGame.bitmap) * 0.33,
			0
		);
	}
}

void initShip(SpaceShip& ship) {
	ship.bitmap = al_load_bitmap("assets/sprites/ship.png");
	ship.x = centerBitmapX(ship.bitmap, mode.width) + al_get_bitmap_width(ship.bitmap) / 2;
	ship.y = centerBitmapY(ship.bitmap, mode.height) + al_get_bitmap_height(ship.bitmap) / 2;
	ship.ID = PLAYER;
	ship.lives = 5;
	ship.kill = 0;
	ship.level = 1;
	ship.score = 0;
	ship.target = -1;
	ship.speed = 8;
}

void drawShip(SpaceShip& ship) {
	al_draw_scaled_rotated_bitmap(ship.bitmap,
		al_get_bitmap_width(ship.bitmap) / 2, al_get_bitmap_height(ship.bitmap) / 2,
		ship.x, ship.y, 0.5, 0.5,
		ship.angle, 0);
}

void updateShip(SpaceShip& ship, Asteroid asteroids[]) {
	if (ship.target >= 0) {
		if (ship.step <= 0) {
			al_play_sample(laser, 1, 0.0, 1.3, ALLEGRO_PLAYMODE_ONCE, 0);
			createBullet(bullets, NUM_BULLETS, ship, asteroids, NUM_ASTEROIDS, ship.target);
			ship.target = -1;
		} else {
			ship.angle += ship.diff / ship.stepCount;
			ship.step--;
		}
	}
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
			al_draw_rotated_bitmap(asteroids[i].bitmap,
				al_get_bitmap_width(asteroids[i].bitmap) / 2, al_get_bitmap_height(asteroids[i].bitmap) / 2,
				asteroids[i].x, asteroids[i].y, 0, 0);
			int x = asteroids[i].x - al_get_ustr_width(wordFont, asteroids[i].word) / 2;
			int y = asteroids[i].y + al_get_bitmap_height(asteroids[i].bitmap) / 2 + 10;
			al_draw_ustr(wordFont, al_map_rgb(255, 255, 255), x, y, 0, asteroids[i].word);
			al_draw_ustr(wordFont, al_map_rgb(255, 255, 0), x, y, 0, al_ustr_dup_substr(asteroids[i].word, 0, asteroids[i].idxChar));
		}
	}
}

void createAsteroid(Asteroid asteroids[], int size, int level, float seconds)
{
	int qtAsteroids = random(1, min(level, 3));

	for (int i = 1; i <= qtAsteroids; i++) {
		for (int j = 0; j < size; j++)
		{
			if (!asteroids[j].live)
			{
				int* xy = generateAsteroidXY();

				asteroids[j].x = xy[0];
				asteroids[j].y = xy[1];
				asteroids[j].delta = seconds;
				asteroids[j].level = random(max(1, min(round(level / 3), 4)), 4);
				asteroids[j].speed = max(1.25, (6 - asteroids[j].level) / 2);
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
			double theta = atan2(ship.y - asteroids[i].y, ship.x - asteroids[i].x);
			float delta = seconds - asteroids[i].delta;

			asteroids[i].x += (delta * asteroids[i].speed) * cos(theta);
			asteroids[i].y += (delta * asteroids[i].speed) * sin(theta);

			int shipWidth = al_get_bitmap_width(ship.bitmap) / 2;
			int shipHeight = al_get_bitmap_height(ship.bitmap) / 2;

			int asteroidWidth = al_get_bitmap_width(asteroids[i].bitmap) / 2;
			int asteroidHeight = al_get_bitmap_height(asteroids[i].bitmap) / 2;

			if (asteroids[i].x > (ship.x - shipWidth) &&
				asteroids[i].x < (ship.x + shipWidth) &&
				asteroids[i].y >(ship.y - shipHeight) &&
				asteroids[i].y < (ship.y + shipHeight)) {
				asteroids[i].live = false;
				ship.lives--;
				explosion.lives = 20;
				explosion.x = ship.x - al_get_bitmap_width(ship.bitmap) / 2;
				explosion.y = ship.y - al_get_bitmap_height(ship.bitmap) / 2;
				explosion.bitmap = al_load_bitmap("assets/sprites/playerShip3_damage2.png");
				al_play_sample(shipBoom, 1, 0.0, 1.3, ALLEGRO_PLAYMODE_ONCE, 0);
			}
		}
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
			float angle = atan2(deltaX, deltaY);
			float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
			float directionX = deltaX / distance;
			float directionY = deltaY / distance;
			bullets[i].angle = PI - angle;

			bullets[i].x = bullets[i].x + lroundf(directionX * bullets[i].speed);
			bullets[i].y = bullets[i].y + lroundf(directionY * bullets[i].speed);


			int asteroidWidth = al_get_bitmap_width(asteroids[bullets[i].target].bitmap);
			int asteroidHeight = al_get_bitmap_height(asteroids[bullets[i].target].bitmap);

			if (bullets[i].x > asteroids[bullets[i].target].x &&
				bullets[i].x < asteroids[bullets[i].target].x + asteroidWidth &&
				bullets[i].y > asteroids[bullets[i].target].y &&
				bullets[i].y < asteroids[bullets[i].target].y + asteroidHeight) {
				explosion.bitmap = al_load_bitmap("assets/sprites/explosion3.png");
				explosion.lives = 20;
				explosion.x = bullets[i].x - al_get_bitmap_width(explosion.bitmap) / 2;
				explosion.y = bullets[i].y - al_get_bitmap_height(explosion.bitmap) / 2;
				al_play_sample(boom, 1, 0.0, 1.3, ALLEGRO_PLAYMODE_ONCE, 0);
				bullets[i].live = false;
				asteroids[bullets[i].target].live = false;
				ship.kill++;
				ship.score += asteroids[bullets[i].target].level * 2;
			}
		}
	}
}

void createBullet(Bullet bullet[], int sizeBullet, SpaceShip& ship, Asteroid asteroids[], int sizeAsteroids, int target)
{
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
			ship.target = index;
			int bulletX = asteroids[ship.target].x + al_get_bitmap_height(asteroids[ship.target].bitmap) / 2;
			int bulletY = asteroids[ship.target].y + al_get_bitmap_width(asteroids[ship.target].bitmap) / 2;
			int deltaX = bulletX - ship.x;
			int deltaY = bulletY - ship.y;
			float angle = atan2(deltaX, deltaY);
			ship.targetAngle = (PI - angle);
			ship.diff = (ship.targetAngle - ship.angle);
			ship.stepCount = 30.0 * fabsf(ship.diff) / (2 * PI);
			ship.step = 30.0 * fabsf(ship.diff) / (2 * PI);
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

void initPauseButton(Button& button) {
	button.bitmap = al_load_bitmap("assets/sprites/esc.png");
	button.x = mode.width - (al_get_bitmap_width(button.bitmap) * 0.33 + 32);
	button.y = 32;
}

void hasClickedOnPause()
{
	setGameState(PAUSE);
}
