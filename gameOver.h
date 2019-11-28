void gameOverState(ALLEGRO_EVENT& ev, bool& redraw) {
	if (getInit()) {
		setInit(false);
		al_stop_sample(&envId);
		initRestart(restart);
		initMenuPause(menu);
		initGameOver(gameOver);
	}

	switch (ev.type) {
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			clickOnButton(restart, cursor, hasClickedOnRestart);
			clickOnButton(menu, cursor, hasClickedOnMenu);
			break;
	}

	if (redraw && al_is_event_queue_empty(queue)) {
		al_clear_to_color(al_map_rgba(0, 0, 0, 0.5));
		char string[] = "GAMEOVER";
		char score[] = "SCORE: ";
		int midX = al_get_text_width(font[4], string) / 2;
		int midY = al_get_font_line_height(font[4]) / 2;
		al_draw_bitmap(gameOver.bitmap, gameOver.x, gameOver.y, 0);
		al_draw_textf(font[4], al_map_rgb(255, 255, 255), mode.width / 2, mode.height / 3 + 15, ALLEGRO_ALIGN_CENTRE, "%s %d", score, ship.score);
		al_draw_bitmap(restart.bitmap, restart.x, restart.y, 0);
		al_draw_bitmap(menu.bitmap, menu.x, menu.y, 0);
	}
}

void hasClickedOnRestart()
{
	setGameState(PLAYING);
}

void initGameOver(Logo& logo) {
	logo.bitmap = al_load_bitmap("assets/sprites/gameOver.png");
	logo.x = centerBitmapX(logo.bitmap, mode.width);
	logo.y = mode.height * 0.15;
}

void initRestart(Button& button)
{
	button.bitmap = al_load_bitmap("assets/sprites/restart.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height * 0.5;
}

void initMenuPause(Button& button)
{
	button.bitmap = al_load_bitmap("assets/sprites/main.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height * 0.63;
}
