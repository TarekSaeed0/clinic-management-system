/**
 * @file ui.h
 * @brief Header file for user interface utilities.
 */

#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @typedef allow_character_t
 * @brief Function pointer type for character validation.
 *
 * This function determines whether a given character is allowed based on its context
 * in the input string.
 *
 * @param[in] prefix Pointer to the string portion before the character being validated.
 * @param[in] prefix_length Length of the prefix string.
 * @param[in] suffix Pointer to the string portion after the character being validated.
 * @param[in] suffix_length Length of the suffix string.
 * @param[in] character The character to validate.
 *
 * @return `true` if the character is allowed, `false` otherwise.
 */
typedef bool (*allow_character_t)(
	const char *prefix,
	size_t prefix_length,
	const char *suffix,
	size_t suffix_length,
	char character
);

/**
 * @brief Prompts the user to input a string from stdin.
 *
 * Displays a prompt and reads input from stdin while applying character validation and optional masking.
 * Returns a null-terminated, dynamically allocated string that must be freed by the caller.
 * Returns `NULL` if an error occurs.
 *
 * @param[in] prompt A string displayed as a prompt before the input field.
 * @param[in] allow_character A callback function for validating characters during input.
 *                             If `NULL`, all characters are accepted.
 * @param[in] mask A character used to mask the input (e.g., for passwords).
 *                 If not printable, masking is disabled.
 *
 * @return A dynamically allocated string containing the user's input, or `NULL` on error.
 */
char *text_field(const char *prompt, allow_character_t allow_character, char mask);

/**
 * @brief Displays a menu and allows the user to select an option.
 *
 * Renders a list of menu items and prompts the user to select one by its index.
 *
 * @param[in] title The title displayed above the menu options.
 * @param[in] items Array of null-terminated strings representing the menu items.
 * @param[in] length Number of items in the menu.
 *
 * @return The index of the selected item (starting from 0).
 *         If no valid selection is made, the behavior is undefined.
 */
size_t menu(const char *title, const char *const *items, size_t length);

#endif
