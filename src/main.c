#include <doctors.h>
#include <patients.h>
#include <stdio.h>
#include <stdlib.h>
#include <ui.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

int main(void) {
	struct doctors doctors;
	doctors_read(&doctors, DOCTORS_FILE_NAME);
	struct patients patients;
	patients_read(&patients, PATIENTS_FILE_NAME);

	size_t page = 0;

	while (1) {
		switch (page) {
			case 0: {
				const char *items[] = {
					"Sign Up",
					"Sign In",
					"Quit",
				};
				size_t selected = menu(NULL, items, ARRAY_LENGTH(items));
				switch (selected) {
					case 0: {
						char *username = text_field("Username", NULL, 0);
						char *password = text_field("Password", NULL, '*');
						char *name = text_field("Name", NULL, 0);

						enum patients_sign_up_result result =
							patients_sign_up(&patients, username, password, name);
						switch (result) {
							case patients_sign_up_success: {
								printf("Successfully signed up.\n");
							} break;
							case patients_sign_up_failure_user_already_exists: {
								printf("There already exists a patient with that username.\n");
							} break;
							case patients_sign_up_failure_maximum_count_reached: {
								printf("Maximum number of simultaneous patients in the system "
									   "reached.\n");
							} break;
						}

						free(username);
						free(password);
						free(name);
					} break;
					case 1: {
						char *username = text_field("Username", NULL, 0);
						char *password = text_field("Password", NULL, '*');

						enum patients_sign_in_result result =
							patients_sign_in(&patients, username, password);
						switch (result) {
							case patients_sign_in_success: {
								printf("Successfully signed in.\n");
								page = 1;
							} break;
							case patients_sign_in_failure_user_does_not_exist: {
								printf("There isn't a patient with that username.\n");
							} break;
							case patients_sign_in_failure_password_is_incorrect: {
								printf("The password is incorrect.\n");
							} break;
						}

						free(password);
						free(username);
					} break;
					case 2: {
						patients_write(&patients, PATIENTS_FILE_NAME);

						return EXIT_SUCCESS;
					} break;
					default: __builtin_unreachable();
				}
			} break;
			case 1: {
				const char *items[] = {
					"List Doctors",
					"Search Doctors",
					"Sign Out",
				};
				size_t selected = menu(NULL, items, ARRAY_LENGTH(items));
				switch (selected) {
					case 0: {
						printf("There is %zu doctors\n", doctors.count);
						for (size_t i = 0; i < doctors.count; i++) {
							printf("%zu. %s\n", i, doctors.data[i].name);
							printf("\tspeciality: %s\n", doctors.data[i].speciality);
							printf("\tclinic address: %s\n", doctors.data[i].clinic_address);
							printf("\tvisita: %s\n", doctors.data[i].visita);
						}
					} break;
					case 1: {

						char *speciality = text_field("Speciality", NULL, 0);

						struct doctors results = doctors_search(&doctors, speciality);

						printf("Found %zu search results\n", results.count);
						for (size_t i = 0; i < results.count; i++) {
							printf("%s\n", results.data[i].name);
						}

						free(speciality);
					} break;
					default: {
						page = 0;
					} break;
				}
			} break;
			default: __builtin_unreachable();
		}
	}
}
