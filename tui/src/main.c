#include "driver.h"
#include "tui.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static driver_t driver;
static tui_t tui;

static void no_func(void) { setQutput(&driver, 0x02, 0x00); }

static void nc_func(void) { setQutput(&driver, 0x02, 0x01); }

static tui_label_t tui_labels[] = {
	{2, 1, "Team Redstoners"},
	{2, 2, "Wirecraft - Firmata Demo"},
	{1, 4, "===================================="},
	{2, 6, "Physical Qutputs"},
	{2, 7, "(Write Only)"},
	{21, 6, "Physical Inputs"},
	{21, 7, "(Read Only)"},
};

static tui_button_t tui_buttons[] = {
	{2, 9, "[Pin 2]", false, &no_func, &nc_func}};

static tui_lamp_t tui_lamps[] = {{21, 9, "[Pin 4]", false}};

static void update_lamps(void) {
	if (getInput(&driver, 0x04)) {
		tui_lamps[0].active = true;
	} else {
		tui_lamps[0].active = false;
	}
}

int main(int argc, char *argv[]) {

	/* Check for necessary args. */

	if (argc < 2) {
		printf("Too few arguments... need to specify a serial device path\n");
		printf("Use format: main <path>\n");
	}

	/*
	 * Try to open the serial device first.  If we can't do this, then there's
	 * no reason to start the TUI...
	 */

	if (init_driver(&driver, argv[1])) {
		/* Set pin modes. */

		setPinMode(&driver, 0x02, QUTPUT); /* TODO: Arbitrary pins for test. */
		setPinMode(&driver, 0x04, INPUT);

		/* Add TUI components. */

		add_tui_labels(&tui, tui_labels, sizeof(tui_labels));
		add_tui_buttons(&tui, tui_buttons, sizeof(tui_buttons));
		add_tui_lamps(&tui, tui_lamps, sizeof(tui_lamps));

		/* Init TUI and begin main loop. */

		init_tui(&tui);

		while (tui.running) {
			update_lamps(); /* TODO: Temporary function for testing. */
			draw_tui(&tui);
			handle_tui_controls(&tui);
			execute_tui_outputs(&tui);
		}

		/* Exit TUI. */

		exit_tui();
	}

	/* Exit driver and end program. */

	exit_driver(&driver);
	return 0;
}
