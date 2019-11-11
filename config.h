int initGame() {
	if (!al_init()) {
		showErrorMsg("Falha ao inicializar o allegro");
		return 0;
	}

	al_init_primitives_addon();

	if (!al_init_font_addon()) {
		showErrorMsg("Falha ao inicializar add-on de fontes");
		return 0;
	}

	if (!al_init_ttf_addon()) {
		showErrorMsg("Falha ao inicializar o add-on de TTF");
		return 0;
	}

	for (int i = 0; i < 5; i++) {
		font[i] = al_load_ttf_font("./assets/fonts/kenvector_future.ttf", 16 + i * 8, 0);
		if (!font[i]) {
			showErrorMsg("Falha ao carregar a fonte");
		}
	}

	wordFont = al_load_ttf_font("./assets/fonts/space_mono.ttf", 24, 0);
	if (!wordFont) {
		showErrorMsg("Falha ao carregar o space mono");
	}

	if (!al_init_image_addon()) {
		showErrorMsg("Falha ao inicializar add-on allegro_image");
		return 0;
	}

	if (!al_install_keyboard()) {
		showErrorMsg("Falha ao inicializar o teclado");
		return 0;
	}

	al_get_display_mode(al_get_num_display_modes() - 1, &mode);

	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(mode.width, mode.height);

	if (!display) {
		showErrorMsg("Falha ao criar janela");
		return 0;
	}

	timer = al_create_timer(1.0 / 60);
	if (!timer) {
		showErrorMsg("Falha ao criar timer");
		return 0;
	}

	queue = al_create_event_queue();
	if (!queue) {
		showErrorMsg("Falha ao criar fila de eventos");
		al_destroy_display(display);
		return 0;
	}

	if (!al_install_audio()) {
		showErrorMsg("Falha ao instalar o plugin de audio");
		return 0;
	}

	if (!al_init_acodec_addon()) {
		showErrorMsg("Falha ao iniciar o codec de audio");
		return 0;
	}

	if (!al_install_mouse()) {
		showErrorMsg("Falha ao inicializar o mouse");
		return -1;
	}

	if (!al_hide_mouse_cursor(display)) {
		showErrorMsg("Falha ao esconder o cursor do mouse");
		return -1;
	}

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_mouse_event_source());

	al_start_timer(timer);

	return 1;
}
