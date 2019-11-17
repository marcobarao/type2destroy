void highscore(ALLEGRO_EVENT& ev, bool& redraw) {
	if (getInit()) {
		setInit(false);
		initCancel(cancel);
	}

	switch (ev.type) {
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			clickOnButton(cancel, cursor, hasClickedOnBack);
			break;
	}

	if (redraw && al_is_event_queue_empty(queue)) {
		char string[] = "Highscore";
		al_draw_text(font[4], al_map_rgb(255, 255, 255), (mode.width / 2 - al_get_text_width(font[4], string) / 2), mode.height * 0.1, 20, string);
		for (int i = 0; i < 10; i++) {
			ALLEGRO_USTR* line = al_ustr_new(highScoreNames[i]);
			char score[6];
			sprintf_s(score, "%d", highScoreScores[i]);
			int padding = 100 - strlen(highScoreNames[i]) - strlen(score);
			while (al_ustr_length(line) < padding) {
				al_ustr_append_chr(line, '.');
			}
			al_ustr_append_cstr(line, score);
			al_draw_text(font[2], al_map_rgb(255, 255, 255), (mode.width / 2 - al_get_text_width(font[2], al_cstr(line)) / 2), mode.height * (0.05 * i) + mode.height * 0.2, 20, al_cstr(line));
		}
		al_draw_bitmap(cancel.bitmap, cancel.x, cancel.y, 0);
	}
}

void initCancel(Button& button)
{
	button.bitmap = al_load_bitmap("assets/sprites/mainmenu.png");
	button.x = centerBitmapX(button.bitmap, mode.width);
	button.y = mode.height * 0.8;
}

void hasClickedOnBack()
{
	setGameState(MENU);
}

void getHighscore()
{
	char** names = (char**)malloc(10);
	int scores[10] = { INT_MIN };
	ALLEGRO_USTR** list = (ALLEGRO_USTR**)malloc(200);
	int size = getSectionData("Players", "./highscore.ini", list);
	for (int i = 0; i < size; i++) {
		char* str = (char*)al_cstr(list[i]);
		char* context = NULL;
		char* end;
		char* splited = strtok_s(str, "=", &context);
		char* name = splited;
		splited = strtok_s(NULL, "=", &context);
		int score = atol(splited);

		for (int j = 0; j < 10; j++) {
			if (score > scores[j]) {
				for (int k = 9; k > j; k--) {
					names[k] = names[k - 1];
					scores[k] = scores[k - 1];
				}

				names[j] = name;
				scores[j] = score;

				break;
			}
		}
	}

	highScoreNames = names;
	for (int i = 0; i < 10; i++) {
		highScoreScores[i] = scores[i];
	}
}