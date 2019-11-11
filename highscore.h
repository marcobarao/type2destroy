void highscore(ALLEGRO_EVENT& ev, bool& redraw) {

	switch (ev.type) {
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			clickOnButton(restart, cursor, hasClickedOnBack);
			break;
	}

	if (redraw && al_is_event_queue_empty(queue)) {
		char string[] = "Highscore";
		al_clear_to_color(al_map_rgba(0, 0, 0, 0.5));
		al_draw_text(font[4], al_map_rgb(255, 255, 255), (mode.width / 2 - al_get_text_width(font[4], string) / 2), mode.height * 0.1, 20, string);
		al_draw_line(mode.width * 0.3, mode.height * 0.55, mode.width - mode.width * 0.3, mode.height * 0.55, al_map_rgb(255, 255, 255), 4);
		al_draw_bitmap(restart.bitmap, restart.x, restart.y, 0);
	}
}

void hasClickedOnBack()
{
	setGameState(MENU);
}
