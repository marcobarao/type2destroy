void pauseState(ALLEGRO_EVENT& ev, bool& redraw) {
	if (getInit()) {
		setInit(false);
		initPause(pause);
		initMenu(menu);
		initPaused(paused);
	}
	switch (ev.type) {
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			clickOnButton(pause, cursor, hasClickedOnContinue);
			clickOnButton(menu, cursor, hasClickedOnMenu);
			break;
	}

	if (redraw && al_is_event_queue_empty(queue)) {
		al_draw_bitmap(paused.bitmap, paused.x, paused.y, 0);
		al_draw_bitmap(pause.bitmap, pause.x, pause.y, 0);
		al_draw_bitmap(menu.bitmap, menu.x, menu.y, 0);
	}
}

void hasClickedOnContinue()
{
	setGameState(PLAYING);
}

void hasClickedOnMenu()
{
	setGameState(MENU);
}

void initPaused(Logo& paused)
{
	paused.bitmap = al_load_bitmap("assets/sprites/pausedw.png");
	paused.x = centerBitmapX(paused.bitmap, mode.width);
	paused.y = mode.height * 0.15;
}

void initPause(Button& button)
{
	button.bitmap = al_load_bitmap("assets/sprites/continue.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height * 0.5;
}

void initMenu(Button& button)
{
	button.bitmap = al_load_bitmap("assets/sprites/mainmenu.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height * 0.6;
}
