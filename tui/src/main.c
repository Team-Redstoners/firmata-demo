#include "tui.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
 * TODO: This is just a template for a possible TUI setup for the demo.  It'll
 * probably need to change between now and when we implement Firmata...
 */

static tui_t tui;

static void no_func(void)
{
	tui.lamps[0].active = true;
}

static void nc_func(void)
{
	tui.lamps[0].active = false;
}

static tui_label_t tui_labels[] =
{
	{1, 1, "Physical Qutputs"},
	{1, 2, "(Write Only)"},
	{20, 1, "Physical Inputs"},
	{20, 2, "(Read Only)"},
};

static tui_button_t tui_buttons[] =
{
	{1, 4, "[GPIO1 ]", false, &no_func, &nc_func},
	{1, 5, "[GPIO2 ]", false, NULL, NULL},
	{1, 6, "[GPIO3 ]", false, NULL, NULL},
	{1, 7, "[GPIO4 ]", false, NULL, NULL},
	{1, 8, "[GPIO5 ]", false, NULL, NULL},
};

static tui_lamp_t tui_lamps[] =
{
	{20, 4, "[GPIO6 ]", false},
	{20, 5, "[GPIO7 ]", false},
	{20, 6, "[GPIO8 ]", false},
	{20, 7, "[GPIO9 ]", false},
	{20, 8, "[GPIO10]", false},
};

int main(void)
{
	add_tui_labels(&tui, tui_labels, sizeof(tui_labels));
	add_tui_buttons(&tui, tui_buttons, sizeof(tui_buttons));
	add_tui_lamps(&tui, tui_lamps, sizeof(tui_lamps));

	init_tui(&tui);

	while(tui.running)
	{
		draw_tui(&tui);
		handle_tui_controls(&tui);
		execute_tui_outputs(&tui);
	}

	exit_tui();
}
