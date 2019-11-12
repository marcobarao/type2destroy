void highscore(ALLEGRO_EVENT& ev, bool& redraw) {
	switch (ev.type) {
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			clickOnButton(cancel, cursor, hasClickedOnBack);
			break;
	}

	if (redraw && al_is_event_queue_empty(queue)) {
		char string[] = "Highscore";
		al_clear_to_color(al_map_rgba(0, 0, 0, 0.5));
		al_draw_text(font[4], al_map_rgb(255, 255, 255), (mode.width / 2 - al_get_text_width(font[4], string) / 2), mode.height * 0.1, 20, string);
		for (int i = 0; i < 10; i++) {
			al_draw_text(font[2], al_map_rgb(255, 255, 255), (mode.width / 2 - al_get_text_width(font[2], "Marco...........................................................................................0") / 2), mode.height * (0.05 * i) + mode.height * 0.2, 20, "Marco...........................................................................................0");
		}
		al_draw_bitmap(cancel.bitmap, cancel.x, cancel.y, 0);
	}
}

void initCancel(Button& button)
{
	button.bitmap = al_load_bitmap("assets/sprites/restart.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height * 0.8;
}

void hasClickedOnBack()
{
	setGameState(MENU);
}

void getHighscore()
{
	CStringList* list = getSectionData("Players", "./highscore.ini");
	highscoreRank = list;
}
