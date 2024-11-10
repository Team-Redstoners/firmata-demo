#ifndef TUI_CONFIG_H
#define TUI_CONFIG_H

#include <ncurses.h>

typedef enum tui_keybinds_t
{
	TUI_KEYBIND_EXIT		=	'q',
	TUI_KEYBIND_UP			=	KEY_UP,
	TUI_KEYBIND_LEFT		=	KEY_LEFT,
	TUI_KEYBIND_DOWN		=	KEY_DOWN,
	TUI_KEYBIND_RIGHT		=	KEY_RIGHT,
	TUI_KEYBIND_ACTIVATE	=	' ',
}
tui_keybinds_t;

#endif /* TUI_CONFIG_H */
