#include <ui.h>

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

// an implementation of getch() for POSIX
static int getch(void) {
	struct termios old;
	// save old terminal settings
	tcgetattr(STDIN_FILENO, &old);
	struct termios current = old;
	// disable buffered i/o and echo
	current.c_lflag &= ~((tcflag_t)ICANON | (tcflag_t)ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &current);
	int character = getchar();
	// restore old terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	return character;
}
// an implementation of kbhit() for POSIX
static int kbhit(void) {
	struct termios old;
	// save old terminal settings
	tcgetattr(STDIN_FILENO, &old);
	struct termios current = old;
	// disable buffered i/o and echo
	current.c_lflag &= ~((tcflag_t)ICANON | (tcflag_t)ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &current);
	// set non-blocking mode
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, (unsigned)flags | O_NONBLOCK);
	int character = getchar();
	// restore old terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	fcntl(STDIN_FILENO, F_SETFL, flags);
	if (character == EOF) {
		return 0;
	}
	(void)ungetc(character, stdin);
	return 1;
}
#endif

#define MINIMUM_TEXT_FIELD_WIDTH 16

char *text_field(const char *prompt, allow_character_t allow_character, char mask) {
	char *string = malloc(1);
	if (string == NULL) {
		return NULL;
	}
	*string = '\0';
	size_t length = 0;
	size_t capacity = 1;
	size_t index = 0;

	size_t width = MINIMUM_TEXT_FIELD_WIDTH;

	printf("╭");
	for (size_t i = 0; i < strlen(prompt) + width + 5; i++) {
		printf("─");
	}
	printf("╮\n");

	printf("│ %s │ %-*s │\n", prompt, (int)width, "");

	printf("╰");
	for (size_t i = 0; i < strlen(prompt) + width + 5; i++) {
		printf("─");
	}
	printf("╯\n\x1b[2A\x1b[%zuC", strlen(prompt) + 5);

	int character = 0;
	while ((character = getch()) != '\n' && character != '\r' && character != EOF) {
		// handle backspace and delete
		if (character == 8 || character == 127) {
			// nothing to delete
			if (index == 0) {
				continue;
			}

			// check if it's allowed for the character after the deleted one to be there.
			if (allow_character != NULL && index != length &&
				!allow_character(string, index - 1, string + index + 1, length - index - 1, string[index])) {
				continue;
			}

			memmove(&string[index - 1], &string[index], length - index + 1);
			--index;
			--length;

			putchar('\b');
			if (isprint((unsigned char)mask)) {
				if (index != length) {
					printf("\x1b[%zuC", length - index);
				}
			} else {
				printf("%s", string + index);
			}
			printf(" \b");
			if (index != length) {
				printf("\x1b[%zuD", length - index);
			}

			continue;
		}

#ifdef _WIN32
		if (character == 224) {
			if (!kbhit()) {
				continue;
			}

			switch (character = getch()) {
				// handle right and left arrow keys
				case 'M': {
					if (index < length) {
						++index;
						printf("\x1b[1C");
					}
					continue;
				} break;
				case 'K': {
					if (index > 0) {
						--index;
						printf("\x1b[1D");
					}
					continue;
				} break;
			}

			if (character == '\n' || character == '\r' || character == EOF) {
				break;
			}

			continue;
		}
#else
		if (character == 27) {
			// the escape button was pressed
			if (!kbhit()) {
				continue;
			}

			// ansi escape code
			switch (character = getch()) {
				case '[': {
					switch (character = getch()) {
						// handle right and left arrow keys
						case 'C': {
							if (index < length) {
								++index;
								printf("\x1b[1C");
							}
							continue;
						} break;
						case 'D': {
							if (index > 0) {
								--index;
								printf("\x1b[1D");
							}
							continue;
						} break;
					}
				} break;
			}

			// ignore ansi escape codes
			while (character != '\n' && character != '\r' && character != EOF) {
				if (isalpha((unsigned char)character) || character == '~') {
					break;
				}
				character = getch();
			}

			continue;
		}
#endif

		// ignore disallowed characters
		if (allow_character != NULL &&
			!allow_character(string, index, string + index, length - index, (char)character)) {
			continue;
		}

		if (isprint((unsigned char)mask)) {
			putchar(mask);
			if (index != length) {
				for (size_t i = index; i < length; i++) {
					putchar(mask);
				}
				printf("\x1b[%zuD", length - index);
			}
		} else {
			putchar(character);
			if (index != length) {
				printf("%s\x1b[%zuD", string + index, length - index);
			}
		}

		if (length + 1 >= capacity) {
			// capacity overflow
			if (capacity == SIZE_MAX) {
				free(string);
				string = NULL;
				break;
			}

			// capacity doubles each time
			if (capacity > SIZE_MAX / 2) {
				capacity = SIZE_MAX;
			} else {
				capacity *= 2;
			}

			char *new_string = realloc(string, capacity);
			// allocation failure
			if (new_string == NULL) {
				free(string);
				string = NULL;
				break;
			}
			string = new_string;
		}

		memmove(&string[index + 1], &string[index], length - index + 1);
		string[index++] = (char)character;
		++length;
	}

	printf("\x1b[2B\x1b[1G");

	return string;
}

size_t menu(const char *title, const char *const *items, size_t length) {
	size_t selected = 0;

	size_t maximum_width = title != NULL ? strlen(title) : 0;
	for (size_t i = 0; i < length; i++) {
		size_t width = strlen(items[i]);
		if (width > maximum_width) {
			maximum_width = width;
		}
	}

	printf("╭");
	for (size_t i = 0; i < maximum_width + 6; i++) {
		printf("─");
	}
	printf("╮\n");

	if (title != NULL) {
		printf("│   %-*s   │\n", (int)maximum_width, title);

		printf("│");
		for (size_t i = 0; i < maximum_width + 6; i++) {
			printf("─");
		}
		printf("│\n");
	}

	printf("\x1b[?25l");
	for (size_t i = 0; i < length; i++) {
		printf("│   %-*s   │\n", (int)maximum_width, items[i]);
	}

	printf("╰");
	for (size_t i = 0; i < maximum_width + 6; i++) {
		printf("─");
	}
	printf("╯\n");

	printf("\x1b[%zuA│ >\x1b[1G", length + 1);

	int character = 0;
	while ((character = getch()) != '\n' && character != '\r' && character != EOF) {
#ifdef _WIN32
		if (character == 224) {
			if (!kbhit()) {
				continue;
			}

			switch (character = getch()) {
				// handle up and down arrow keys
				case 'H': {
					printf("\x1b[1A");
					continue;
				} break;
				case 'P': {
					printf("\x1b[1B");
					continue;
				} break;
			}

			if (character == '\n' || character == '\r' || character == EOF) {
				break;
			}

			continue;
		}
#else
		if (character == 27) {
			// the escape button was pressed
			if (!kbhit()) {
				continue;
			}

			// ansi escape code
			switch (character = getch()) {
				case '[': {
					switch (character = getch()) {
						// handle up and down arrow keys
						case 'A': {
							printf("\x1b[2K│   %-*s   │\x1b[1G\x1b[1A", (int)maximum_width, items[selected]);
							if (selected > 0) {
								selected--;
							} else {
								selected = length - 1;
								printf("\x1b[%zuB", length);
							}
							printf("\x1b[2K│ > %-*s   │\x1b[1G", (int)maximum_width, items[selected]);
							continue;
						} break;
						case 'B': {
							printf("\x1b[2K│   %-*s   │\x1b[1G\x1b[1B", (int)maximum_width, items[selected]);
							if (selected < length - 1) {
								selected++;
							} else {
								selected = 0;
								printf("\x1b[%zuA", length);
							}
							printf("\x1b[2K│ > %-*s   │\x1b[1G", (int)maximum_width, items[selected]);
							continue;
						} break;
					}
				} break;
			}

			// ignore ansi escape codes
			while (character != '\n' && character != '\r' && character != EOF) {
				if (isalpha((unsigned char)character) || character == '~') {
					break;
				}
				character = getch();
			}

			continue;
		}
#endif
	}

	printf("\x1b[%zuB\x1b[1G\x1b[?25h", length - selected + 1);

	return selected;
}
