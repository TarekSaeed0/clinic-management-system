#ifndef DOCTORS_H
#define DOCTORS_H

// The struct of doctors
struct Doctor {
	char name[100];
	char speciality[100];
	char address[100];
	char visita[100];
};

// this struct is very useful for getting an array as an output
struct Doctors {
	struct Doctor arrayToReturn[10];
};

// the required functions
int checkFileExistence(const char filePath[]);

struct Doctors getDoctors(const char filePath[]);

struct SearchResults {
	struct Doctor arrayToReturn[10];
	int count;
};

struct SearchResults searchBySpeciality(
	const char specialityOfDoctor[],
	struct Doctor arrayOfDoctors[10]
);

#endif
