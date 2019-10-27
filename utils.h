void showErrorMsg(const char* text) {
	al_show_native_message_box(display, "ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

int random(int lower, int upper) {
	return (rand() %
		(upper - lower + 1)) + lower;
}

void clickOnButton(Button button, Cursor cursor, void (*callback)()) {
	int endXButton = button.x + al_get_bitmap_width(button.bitmap);
	int endYButton = button.y + al_get_bitmap_height(button.bitmap);

	bool onXRange = cursor.x > button.x && cursor.x < endXButton;
	bool onYRange = cursor.y > button.y && cursor.y < endYButton;

	if (onXRange && onYRange) {
		(*callback)();
	}
}

int centerBitmapX(ALLEGRO_BITMAP* bitmap, int width) {
	return (width / 2) - (al_get_bitmap_width(bitmap) / 2);
}

int centerBitmapY(ALLEGRO_BITMAP* bitmap, int height) {
	return (height / 2) - (al_get_bitmap_width(bitmap) / 2);
}