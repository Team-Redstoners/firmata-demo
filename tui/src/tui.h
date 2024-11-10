#ifndef TUI_H
#define TUI_H

#include <stdbool.h>
#include <stddef.h>

typedef struct tui_label_t {
	int x;
	int y;
	char *text;
} tui_label_t;

typedef struct tui_button_t {
	int x;
	int y;
	char *text;
	bool active;
	void (*no_func)(); /* Executes when the button is active. */
	void (*nc_func)(); /* Executes when the button is not active. */
} tui_button_t;

typedef struct tui_lamp_t {
	int x;
	int y;
	char *text;
	bool active;
} tui_lamp_t;

typedef struct tui_t {
	bool running;
	int selection_id;
	int n_labels;
	int n_buttons;
	int n_lamps;
	tui_label_t *labels;
	tui_button_t *buttons;
	tui_lamp_t *lamps;
} tui_t;

void init_tui(tui_t *tui);
void exit_tui(void);
void add_tui_labels(tui_t *tui, tui_label_t *tui_labels, size_t array_size);
void add_tui_buttons(tui_t *tui, tui_button_t *tui_buttons, size_t array_size);
void add_tui_lamps(tui_t *tui, tui_lamp_t *tui_lamps, size_t array_size);
void draw_tui(tui_t *tui);
void move_tui_selection(tui_t *tui, int distance);
void activate_tui_selection(tui_t *tui);
void execute_tui_outputs(tui_t *tui);
void handle_tui_controls(tui_t *tui);

#endif /* TUI_H */
