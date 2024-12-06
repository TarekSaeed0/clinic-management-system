#include <clinic_management_system/doctors.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

int checkFileExistence(const char filePath[]) {
	FILE *fptr = fopen(filePath, "r");

	if (fptr == NULL) {
		printf("The database doesn't exist");
		return 1;
	} else {
		printf("The database exists\n");
		fclose(fptr);
		return 0;
	}
}

struct TempStruct getDoctors(const char filePath[]) {

	FILE *fptr = fopen(filePath, "r");

	struct Doctor doctorArray[10];

	char *tempptr;
	char buffer[1024];
	int field;
	for (int row = 0; fgets(buffer, 1024, fptr); row++) {
		field = 0;
		tempptr = strtok(buffer, "|");
		while (tempptr != NULL) {
			switch (field) {
				case 0: strcpy(doctorArray[row].name, tempptr); break;
				case 1: strcpy(doctorArray[row].speciality, tempptr); break;
				case 2: strcpy(doctorArray[row].address, tempptr); break;
				case 3: strcpy(doctorArray[row].visita, tempptr); break;
			}
			tempptr = strtok(NULL, "|");
			field++;
		}
	}
	struct TempStruct temporary;
	memcpy(temporary.arrayToReturn, doctorArray, 10 * sizeof(doctorArray[0]));
	fclose(fptr);
	return temporary;
}

struct TempStruct1 searchBySpeciality(
	const char specialityOfDoctor[],
	struct Doctor arrayOfDoctors[10]
) {
	struct Doctor listingArray[10];
	int is_somehow_similar = 0;

	for (size_t k = 0; k < 10; k++) {
		for (size_t i = 0; arrayOfDoctors[k].speciality[i] != '\0'; i++) {

			for (size_t j = 0; j < strlen(specialityOfDoctor); j++) {
				if (tolower(arrayOfDoctors[k].speciality[i + j]) ==
					tolower(specialityOfDoctor[j])) {
					if (j == (strlen(specialityOfDoctor) - 1)) {
						listingArray[is_somehow_similar] = arrayOfDoctors[k];
						is_somehow_similar++;
						break;
					}
					continue;
				} else {
					break;
				}
			}
			break;
		}
	}

	struct TempStruct1 temporary;
	memcpy(temporary.arrayToReturn, listingArray, 10 * sizeof(listingArray[0]));
	temporary.count = is_somehow_similar;
	return temporary;
}
