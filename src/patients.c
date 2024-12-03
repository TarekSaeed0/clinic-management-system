#include <patients.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_ x

enum patients_read_status patients_read(struct patients *patients, const char *file_name) {
	assert(patients != NULL && file_name != NULL);

	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		*patients = (struct patients){
			.data = { 0 },
			.count = 0,
		};
		return patients_read_success;
	}

	patients->count = 0;
	while (fscanf(
			   file,
			   "%" STRINGIFY(MAXIMUM_PATIENT_USERNAME_SIZE) "[^,],%" STRINGIFY(MAXIMUM_PATIENT_PASSWORD_SIZE
			   ) "[^,],%" STRINGIFY(MAXIMUM_PATIENT_NAME_SIZE) "[^,\n] ",
			   patients->data[patients->count].username,
			   patients->data[patients->count].password,
			   patients->data[patients->count].name
		   ) == 3) {
		if (++patients->count == MAXIMUM_PATIENTS_COUNT) {
			break;
		}
	}

	(void)fclose(file);

	return patients_read_success;
}

enum patients_write_status patients_write(const struct patients *patients, const char *file_name) {
	assert(patients != NULL && file_name != NULL);

	FILE *file = fopen(file_name, "w");
	if (file == NULL) {
		return patients_write_failure;
	}

	for (size_t i = 0; i < patients->count; i++) {
		if (fprintf(
				file,
				"%s,%s,%s\n",
				patients->data[i].username,
				patients->data[i].password,
				patients->data[i].name
			) < 0) {
			return patients_write_failure;
		}
	}

	if (fclose(file) == EOF) {
		return patients_write_failure;
	}

	return patients_write_success;
}

enum patients_sign_up_status patients_sign_up(
	struct patients *patients,
	const char *username,
	const char *password,
	const char *name
) {
	assert(patients != NULL && username != NULL && password != NULL && name != NULL);

	size_t i = 0;
	while (i < patients->count) {
		if (strcmp(patients->data[i].username, username) == 0) {
			break;
		}
		i++;
	}

	if (i < patients->count) {
		return patients_sign_up_failure_user_already_exists;
	}

	if (patients->count == MAXIMUM_PATIENTS_COUNT) {
		return patients_sign_up_failure_maximum_count_reached;
	}

	struct patient patient;
	assert(strlen(username) < MAXIMUM_PATIENT_USERNAME_SIZE);
	strcpy(patient.username, username);
	assert(strlen(password) < MAXIMUM_PATIENT_PASSWORD_SIZE);
	strcpy(patient.password, password);
	assert(strlen(name) < MAXIMUM_PATIENT_NAME_SIZE);
	strcpy(patient.name, name);

	patients->data[patients->count++] = patient;
	return patients_sign_up_success;
}

enum patients_sign_in_status patients_sign_in(
	const struct patients *patients,
	const char *username,
	const char *password
) {
	assert(patients != NULL && username != NULL && password != NULL);

	size_t i = 0;
	while (i < patients->count) {
		if (strcmp(patients->data[i].username, username) == 0) {
			break;
		}
		i++;
	}

	if (i >= patients->count) {
		return patients_sign_in_failure_user_does_not_exist;
	}

	if (strcmp(patients->data[i].password, password) != 0) {
		return patients_sign_in_failure_password_is_incorrect;
	}

	return patients_sign_in_success;
}
