#include <stdlib.h>
#include <stdio.h>
#include <afx.h>
#include <Windows.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include <float.h>
#include "objects.h"
#include "utils.h"
#include "config.h"
#include "state.h"
#include "playing.h"
#include "gameOver.h"
#include "addHighScore.h"
#include "highscore.h"
#include "pause.h"
#include "menu.h"

int main()
{
	if (!initGame()) {
		return -1;
	}

	setGameState(MENU);
	bool redraw = true;
	char input[1] = "";

	initCursor(cursor);
	bg = al_load_bitmap("assets/sprites/bg.png");
	
	getHighscore();

	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);

		switch (ev.type) {
			case ALLEGRO_EVENT_TIMER:
				redraw = true;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				done = true;
				break;
			case ALLEGRO_EVENT_MOUSE_AXES:
				cursor.x = ev.mouse.x;
				cursor.y = ev.mouse.y;
				break;
		}

		if (redraw && al_is_event_queue_empty(queue)) {
			for (int i = 0; i < mapSize; i++)
				al_draw_bitmap(bg, tileSize * (i % mapColumns), tileSize * (i / mapColumns), 0);
		}

		switch(getGameState()) {
			case MENU:
				menuState(ev, redraw);
				break;

			case PLAYING:
				playingState(ev, redraw);
				break;

			case PAUSE:
				pauseState(ev, redraw);
				break;

			case GAME_OVER:
				gameOverState(ev, redraw);
				break;

			case ADD_HIGHSCORE:
				addHighScore(ev, redraw);
				break;

			case HIGHSCORE:
				highscore(ev, redraw);
				break;
		}

		if (redraw && al_is_event_queue_empty(queue)) {
			redraw = false;
			al_draw_bitmap(cursor.bitmap, cursor.x, cursor.y, 0);
			al_flip_display();
		}
	}

	al_destroy_display(display);
	for (int i = 0; i < 5; i++) {
		al_destroy_font(font[i]);
	}
	al_destroy_event_queue(queue);

	return 0;
}

void initCursor(Cursor& cursor)
{
	cursor.bitmap = al_load_bitmap("assets/sprites/cursor.png");
}
