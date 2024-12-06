#ifndef CLINIC_MANAGEMENT_SYSTEM_PATIENTS_H
#define CLINIC_MANAGEMENT_SYSTEM_PATIENTS_H

// Log in Check enum
enum checkAccount {
	noAccount,
	noPassword,
	foundAccount
};

struct Patient {
	char name[100];
	char username[100];
	char password[100];
};

struct PatientsData {
	struct Patient Patients[10];
	int numPatients;
};

struct PatientsData getPatients(void);

enum checkAccount logIn(
	struct PatientsData Patients,
	const char takenUsername[],
	const char takenPassword[]
);

enum check_Account {
	existed_username = 1,
	Passwords_didnot_match = 3,
	max_Patients = 4,
	invalid_password = 5,
	invalid_name = 6,
	invalid_username = 7,
	short_name = 8,
	short_username = 9,
	signed_up_successfuly = 10
};

enum check_Account sign_up(
	struct PatientsData *patients,
	const char *taken_name,
	const char *taken_username,
	const char *taken_password,
	const char *taken_confirm_password
);

#endif
