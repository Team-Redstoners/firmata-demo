#include "tui.h"
#include "tui_config.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stddef.h>

void init_tui(tui_t *tui)
{
	/* Init cursor mode. */
	initscr();

	/*
	 * Disable control signals... we probably don't want this as it will limit
	 * Ctrl-C exit, etc.
	 */
	//cbreak();

	/* Enable key-reading. */
	keypad(stdscr, true);

	/* Disable echoing of characters typed in the terminal. */
	noecho();

	/* Set other initial TUI parameters */
	tui->running = true;
	tui->selection_id = 0;
}

void exit_tui(void)
{
	/* Exit cursor mode. */
	endwin();
}

void add_tui_labels(tui_t *tui, tui_label_t *tui_labels, size_t array_size)
{
	tui->labels = tui_labels;
	tui->n_labels = array_size / sizeof(tui_label_t);
}

void add_tui_buttons(tui_t *tui, tui_button_t *tui_buttons, size_t array_size)
{
	tui->buttons = tui_buttons;
	tui->n_buttons = array_size / sizeof(tui_button_t);
}

void add_tui_lamps(tui_t *tui, tui_lamp_t *tui_lamps, size_t array_size)
{
	tui->lamps = tui_lamps;
	tui->n_lamps = array_size / sizeof(tui_lamp_t);
}

void draw_tui(tui_t *tui)
{
	/* Clear the screen. */
	clear();

	/* Draw labels. */
	for(int i = 0; i < tui->n_labels; i++)
	{
		move(tui->labels[i].y, tui->labels[i].x);
		addstr(tui->labels[i].text);
	}

	/* Draw buttons. */
	for(int i = 0; i < tui->n_buttons; i++)
	{
		if(tui->buttons[i].active == true)
		{
			attron(A_STANDOUT);
		}

		move(tui->buttons[i].y, tui->buttons[i].x);
		addstr(tui->buttons[i].text);

		attroff(A_STANDOUT);
	}

	/* Draw lamps. */
	for(int i = 0; i < tui->n_lamps; i++)
	{
		if(tui->lamps[i].active == true)
		{
			attron(A_STANDOUT);
		}

		move(tui->lamps[i].y, tui->lamps[i].x);
		addstr(tui->lamps[i].text);

		attroff(A_STANDOUT);
	}

	/* Draw the cursor selection. */
	move
	(
		tui->buttons[tui->selection_id].y,
		tui->buttons[tui->selection_id].x - 1
	);

	addch('>');

	move
	(
		tui->buttons[tui->selection_id].y,
		tui->buttons[tui->selection_id].x - 1
	);

	/* Draw all changes to the screen. */
	refresh();
}

void move_tui_selection(tui_t *tui, int distance)
{
	/* Increment the selection distance. */
	tui->selection_id += distance;

	/* Prevent the selection from leaving the range of the buttons available. */
	if(tui->selection_id > tui->n_buttons - 1)
	{
		tui->selection_id = 0;
	}

	if(tui->selection_id < 0)
	{
		tui->selection_id = tui->n_buttons - 1;
	}
}

void activate_tui_selection(tui_t *tui)
{
	/* Set buttons to active/inactive upon selection and usage. */

	if(tui->buttons[tui->selection_id].active == false)
	{
		tui->buttons[tui->selection_id].active = true;
	}
	else
	{
		tui->buttons[tui->selection_id].active = false;
	}
}

void execute_tui_outputs(tui_t *tui)
{
	/* Active the NO/NC functions for active/inactive buttons. */

	for(int i = 0; i < tui->n_buttons; i++)
	{
		if(tui->buttons[i].active && tui->buttons[i].no_func)
		{
			tui->buttons[i].no_func();
		}
		else if(tui->buttons[i].nc_func)
		{
			tui->buttons[i].nc_func();
		}
	}
}

void handle_tui_controls(tui_t *tui)
{
	/* Get the keyboard character input. */
	static int ch;
	ch = getch();

	/* Match the keyboard input to a control. */
	switch(ch)
	{
		case TUI_KEYBIND_EXIT:
			tui->running = false;
			break;

		case TUI_KEYBIND_UP:
			move_tui_selection(tui, -1);
			break;

		case TUI_KEYBIND_LEFT:

			break;

		case TUI_KEYBIND_DOWN:
			move_tui_selection(tui, 1);
			break;

		case TUI_KEYBIND_RIGHT:

			break;

		case TUI_KEYBIND_ACTIVATE:
			activate_tui_selection(tui);
			break;
	}
}
