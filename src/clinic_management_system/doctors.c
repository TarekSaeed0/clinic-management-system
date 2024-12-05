#include <clinic_management_system/doctors.h>

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_ x

enum doctors_read_status doctors_read(struct doctors *doctors, const char *file_name) {
	assert(doctors != NULL && file_name != NULL);

	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		doctors->count = 0;
		return doctors_read_success;
	}

	doctors->count = 0;
	while (fscanf(
			   file,
			   "%" STRINGIFY(MAXIMUM_DOCTOR_NAME_SIZE
			   ) "[^,],%" STRINGIFY(MAXIMUM_DOCTOR_SPECIALITY_SIZE
			   ) "[^,],%" STRINGIFY(MAXIMUM_DOCTOR_CLINIC_ADDRESS_SIZE
			   ) "[^,],%" STRINGIFY(MAXIMUM_DOCTOR_VISITA_SIZE) "[^,\n] ",
			   doctors->data[doctors->count].name,
			   doctors->data[doctors->count].speciality,
			   doctors->data[doctors->count].clinic_address,
			   doctors->data[doctors->count].visita
		   ) == 4) {
		if (++doctors->count == MAXIMUM_DOCTORS_COUNT) {
			break;
		}
	}

	(void)fclose(file);

	return doctors_read_success;
}
struct doctors doctors_search(const struct doctors *doctors, const char *speciality) {
	assert(doctors != NULL && speciality != NULL);

	struct doctors results;
	results.count = 0;

	for (size_t i = 0; i < doctors->count; i++) {
		bool matches = true;
		for (size_t j = 0; speciality[j] != '\0'; j++) {
			if (tolower(speciality[j]) != tolower(doctors->data[i].speciality[j])) {
				matches = false;
				break;
			}
		}

		if (matches) {
			results.data[results.count++] = doctors->data[i];
		}
	}

	return results;
}
