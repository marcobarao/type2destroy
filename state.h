int gameState = ADD_HIGHSCORE;
bool init = true;

void setGameState(GAME_STATE newState) {
	switch(newState) {
		case PLAYING:
			init = true;
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

