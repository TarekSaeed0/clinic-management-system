#include <clinic_management_system/patients.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct PatientsData getPatients(void) {
	struct PatientsData Patients;
	FILE *PatientsFile = fopen("patients.csv", "r");

	if (PatientsFile == NULL) {
		Patients.numPatients = 0;
		return Patients;
	}

	int size = 0;
	char name[100], username[100], password[100];

	while (fscanf(PatientsFile, "%99[^,],%99[^,],%99[^\n] ", name, username, password) != EOF) {
		strcpy(Patients.Patients[size].name, name);
		strcpy(Patients.Patients[size].username, username);
		strcpy(Patients.Patients[size].password, password);
		size++;
	}

	Patients.numPatients = size;

	fclose(PatientsFile);
	return Patients;
}

enum checkAccount logIn(
	struct PatientsData Patients,
	const char takenUsername[],
	const char takenPassword[]
) {

	int size = Patients.numPatients;

	bool foundUsername = false;

	for (int i = 0; i < size; i++) {
		// found username
		if (strcmp(Patients.Patients[i].username, takenUsername) == 0) {
			foundUsername = true;
			// found password
			if (strcmp(Patients.Patients[i].password, takenPassword) == 0) {
				return foundAccount;
			} else {
				break;
			}
		}
	}

	if (foundUsername == false) {
		return noAccount;
	} else {
		return noPassword;
	}
}

void append_patients(
	const char *file_name,
	const char *taken_name,
	const char *taken_username,
	const char *taken_password
) {

	FILE *PatientsFile = fopen(file_name, "a");

	fprintf(PatientsFile, "%s,%s,%s\n", taken_name, taken_username, taken_password);

	fclose(PatientsFile);
}

enum check_Account sign_up(
	struct PatientsData *patients,
	const char *taken_name,
	const char *taken_username,
	const char *taken_password,
	const char *taken_confirm_password
) {

	if (strlen(taken_username) < 4) {
		return short_username;
	}

	for (int i = 0; i < (int)strlen(taken_username); i++) {
		if ((taken_username[i] == ' ' || taken_username[i] == '\t' || taken_username[i] == '\n' ||
			 taken_username[i] == ',')) {
			return invalid_username;
		}
	}

	for (int i = 0; i < patients->numPatients; i++) {
		if (strcmp(taken_username, patients->Patients[i].username) == 0) {
			return existed_username;
		}
	}

	if (strlen(taken_name) < 4) {
		return short_name;
	}

	int count = 0;
	for (int i = 0; i < (int)strlen(taken_name); i++) {
		if (((taken_name[i] >= 'A' && taken_name[i] <= 'Z') ||
			 (taken_name[i] >= 'a' && taken_name[i] <= 'z'))) {
			count++;
		} else if ((taken_name[i] != ' ' && taken_name[i] != '\t' && taken_name[i] != '\n')) {
			return invalid_name;
		}
	}
	if (count < 4) {
		return invalid_name;
	}

	if (strlen(taken_password) < 4) {
		return invalid_password;
	}

	for (int i = 0; i < (int)strlen(taken_password); i++) {
		if ((taken_password[i] == ',')) {
			return invalid_password;
		}
	}

	if (strcmp(taken_password, taken_confirm_password) != 0) {
		return Passwords_didnot_match;
	}

	if (patients->numPatients >= 10) {
		return max_Patients;
	} else {
		strcpy(patients->Patients[patients->numPatients].name, taken_name);
		strcpy(patients->Patients[patients->numPatients].username, taken_username);
		strcpy(patients->Patients[patients->numPatients].password, taken_password);
		patients->numPatients++;
		append_patients("patients.csv", taken_name, taken_username, taken_password);
		return signed_up_successfuly;
	}
}
