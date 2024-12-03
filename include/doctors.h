/**
 * @file doctors.h
 * @brief Header file for managing doctors.
 */

#ifndef DOCTORS_H
#define DOCTORS_H

#include <stddef.h>

/** @brief Maximum size for a doctor's name. */
#define MAXIMUM_DOCTOR_NAME_SIZE (128)

/** @brief Maximum size for a doctor's speciality. */
#define MAXIMUM_DOCTOR_SPECIALITY_SIZE (64)

/** @brief Maximum size for a doctor's clinic address. */
#define MAXIMUM_DOCTOR_CLINIC_ADDRESS_SIZE (128)

/** @brief Maximum size for a doctor's visita. */
#define MAXIMUM_DOCTOR_VISITA_SIZE (128)

/** @brief Maximum number of doctors that can be stored. */
#define MAXIMUM_DOCTORS_COUNT (10)

/** @brief Name of file where doctors data are stored. */
#define DOCTORS_FILE_NAME ("doctors.csv")

/**
 * @struct doctor
 * @brief Structure representing the data of a doctor.
 */
struct doctor {
	char name[MAXIMUM_DOCTOR_NAME_SIZE];					 /**< The name of the doctor. */
	char speciality[MAXIMUM_DOCTOR_SPECIALITY_SIZE];		 /**< The speciality of the doctor. */
	char clinic_address[MAXIMUM_DOCTOR_CLINIC_ADDRESS_SIZE]; /**< The clinic address of the doctor. */
	char visita[MAXIMUM_DOCTOR_VISITA_SIZE];				 /**< The visita of the doctor. */
};

/**
 * @struct doctors
 * @brief Structure representing the data of multiple doctors.
 */
struct doctors {
	struct doctor data[MAXIMUM_DOCTORS_COUNT]; /**< Array of doctors. */
	size_t count;							   /**< Number of doctors currently stored. */
};

/**
 * @enum doctors_read_status
 * @brief Status of reading doctors from a file.
 */
enum doctors_read_status {
	doctors_read_success, /**< Reading doctors was successful. */
	doctors_read_failure  /**< Reading doctors failed. */
};

/**
 * @brief Reads doctor data from a file.
 *
 * @param[out] doctors Pointer to the doctors structure to populate.
 * @param[in] file_name Name of the file to read from.
 *
 * @return The status of the read operation.
 */
enum doctors_read_status doctors_read(struct doctors *doctors, const char *file_name);

/**
 * @brief Searches for doctors by speciality.
 *
 * @param[in] doctors Pointer to the doctors structure to search within.
 * @param[in] speciality The speciality to search for.
 *
 * @return The search results.
 */
struct doctors doctors_search(const struct doctors *doctors, const char *speciality);

#endif
