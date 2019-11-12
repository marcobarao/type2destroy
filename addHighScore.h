void addHighScore(ALLEGRO_EVENT& ev, bool& redraw) {

	switch (ev.type) {
		case ALLEGRO_EVENT_KEY_UP:
			if (ev.keyboard.keycode == 63) {
				al_ustr_remove_chr(name, al_ustr_length(name) - 1);
			}
			else if(ev.keyboard.keycode < 27) {
				al_ustr_append_chr(name, ev.keyboard.keycode + 64);
			}
			else if (ev.keyboard.keycode > 26 && ev.keyboard.keycode < 37) {
				al_ustr_append_chr(name, ev.keyboard.keycode + 21);
			}
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			clickOnButton(save, cursor, hasClickedOnSave);
			break;
	}

	if (redraw && al_is_event_queue_empty(queue)) {
		char string[] = "Sua pontuacao foi de %d digite seu nome:";
		al_clear_to_color(al_map_rgba(0, 0, 0, 0.5));
		al_draw_textf(font[2], al_map_rgb(255, 255, 255), (mode.width / 2 - al_get_text_width(font[2], string) / 2), mode.height * 0.35, 20, string, ship.score);
		al_draw_text(font[3], al_map_rgb(255, 255, 255), (mode.width / 2 - al_get_text_width(font[3], al_cstr(name)) / 2), mode.height * 0.55 - al_get_font_line_height(font[3]), 20, al_cstr(name));
		al_draw_line(mode.width * 0.3, mode.height * 0.55, mode.width - mode.width * 0.3, mode.height * 0.55, al_map_rgb(255, 255, 255), 4);
		al_draw_bitmap(save.bitmap, save.x, save.y , 0);
	}
}

void initSave(Button& button) {
	button.bitmap = al_load_bitmap("assets/sprites/restart.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height * 0.7;
}

void hasClickedOnSave()
{
	char score[12];
	sprintf_s(score, "%d", ship.score);
	setKey(score, al_cstr(name), "Players", "./highscore.ini");
	setGameState(HIGHSCORE);
}
