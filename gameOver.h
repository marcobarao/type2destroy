void gameOverState(ALLEGRO_EVENT& ev, bool& redraw) {
	switch (ev.type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		clickOnButton(restart, cursor, hasClickedOnRestart);
		break;
	}

	if (redraw && al_is_event_queue_empty(queue)) {
		char string[] = "GAMEOVER";
		al_clear_to_color(al_map_rgba(0, 0, 0, 0.5));
		int midX = al_get_text_width(font[4], string) / 2;
		int midY = al_get_font_line_height(font[4]) / 2;
		al_draw_text(font[4], al_map_rgb(255, 255, 255), (mode.width / 2 - midX), mode.height / 2 - (midY + 60), 20, string);
		al_draw_bitmap(restart.bitmap, restart.x, restart.y, 0);
	}
}

void hasClickedOnRestart()
{
	setGameState(PLAYING);
}

void initRestart(Button& button)
{
	button.bitmap = al_load_bitmap("assets/sprites/restart.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height / 2 + 20;
}
