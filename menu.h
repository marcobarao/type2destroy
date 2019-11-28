void menuState(ALLEGRO_EVENT& ev, bool& redraw) {
	if (getInit()) {
		setInit(false);
		al_reserve_samples(1);
		env = al_load_sample("assets/audio/menu.wav");

		if (&envId) {
			al_stop_sample(&envId);
		}

		al_play_sample(env, 1, 0.0, 1.3, ALLEGRO_PLAYMODE_ONCE, &envId);
		initLogo(logo);
		initStart(start);
		initScore(score);
		initQuitGame(quitGame);
	}
	switch (ev.type) {
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			clickOnButton(start, cursor, hasClickedOnStart);
			clickOnButton(score, cursor, hasClickedOnScore);
			clickOnButton(quitGame, cursor, hasClickedOnQuitGame);
			break;
	}

	if (redraw && al_is_event_queue_empty(queue)) {
		al_draw_bitmap(logo.bitmap, logo.x, logo.y, 0);
		al_draw_bitmap(start.bitmap, start.x, start.y, 0);
		al_draw_bitmap(score.bitmap, score.x, score.y, 0);
		al_draw_bitmap(quitGame.bitmap, quitGame.x, quitGame.y, 0);
	}
}

void hasClickedOnStart()
{
	setGameState(PLAYING);
}

void hasClickedOnScore()
{
	setGameState(HIGHSCORE);
}

void hasClickedOnQuitGame()
{
	done = true;
}

void initLogo(Logo& logo)
{
	logo.bitmap = al_load_bitmap("assets/sprites/logow.png");
	logo.x = centerBitmapX(logo.bitmap, mode.width);
	logo.y = mode.height * 0.15;
}

void initStart(Button& button)
{
	button.bitmap = al_load_bitmap("assets/sprites/startBtn.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height * 0.4;
}

void initScore(Button& button)
{
	button.bitmap = al_load_bitmap("assets/sprites/scoreBtn.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height * 0.5;
}

void initQuitGame(Button& button)
{
	button.bitmap = al_load_bitmap("assets/sprites/quitBtn.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height * 0.6;
}
