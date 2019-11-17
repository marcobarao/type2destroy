int gameState = MENU;
bool init = true;

void setGameState(GAME_STATE newState) {
	init = true;
	if (newState == PLAYING) {
		al_stop_timer(otherTimer);
		al_start_timer(timer);
	}
	else {
		al_stop_timer(timer);
		al_start_timer(otherTimer);
	}
	switch(newState) {
		case PLAYING:
			init = gameState != PAUSE;
			break;
	}
	gameState = newState;
}

int getGameState() {
	return gameState;
}

void setInit(bool newInit) {
	init = newInit;
}

bool getInit() {
	return init;
}

