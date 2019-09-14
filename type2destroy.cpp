#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int main()
{
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_DISPLAY_MODE mode;

	al_init();
	al_init_font_addon();
	al_init_ttf_addon();

	al_get_display_mode(al_get_num_display_modes() - 1, &mode);

	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(mode.width, mode.height);

	ALLEGRO_FONT* font = al_load_ttf_font("./assets/fonts/SpaceMono-Regular.ttf", 64, 0);

	while (true) {
		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_draw_text(font, al_map_rgb(0, 0, 0), 0, 0, 0, "Type2Destroy");

		al_flip_display();
	}

	al_destroy_display(display);
	al_destroy_font(font);

	return 0;
}
