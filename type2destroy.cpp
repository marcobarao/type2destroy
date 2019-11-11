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

int main()
{
	setGameState(HIGHSCORE);
	bool close = false;
	bool redraw = true;
	char input[1] = "";

	if (!initGame()) {
		return -1;
	}

	initCursor(cursor);
	initRestart(restart);

	while (!close) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);

		switch (ev.type) {
		case ALLEGRO_EVENT_TIMER:
			redraw = true;
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			close = true;
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			cursor.x = ev.mouse.x;
			cursor.y = ev.mouse.y;
			break;
		}

		switch(getGameState()) {
			case PLAYING:
				playingState(ev, redraw);
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
