/**
 * @file patients.h
 * @brief Header file for managing patients.
 */

#ifndef PATIENTS_H
#define PATIENTS_H

#include <stdbool.h>
#include <stddef.h>

/** @brief Maximum size for a patient's username. */
#define MAXIMUM_PATIENT_USERNAME_SIZE (64)

/** @brief Maximum size for a patient's password. */
#define MAXIMUM_PATIENT_PASSWORD_SIZE (64)

/** @brief Maximum size for a patient's name. */
#define MAXIMUM_PATIENT_NAME_SIZE (128)

/** @brief Maximum number of patients that can be stored. */
#define MAXIMUM_PATIENTS_COUNT (10)

/** @brief Name of file where patients data are stored. */
#define PATIENTS_FILE_NAME ("patients.csv")

/**
 * @struct patient
 * @brief Structure representing the data of a patient.
 */
struct patient {
	char username[MAXIMUM_PATIENT_USERNAME_SIZE]; /**< The username of the patient. */
	char password[MAXIMUM_PATIENT_PASSWORD_SIZE]; /**< The password of the patient. */
	char name[MAXIMUM_PATIENT_USERNAME_SIZE];	  /**< The name of the patient. */
};

/**
 * @struct patients
 * @brief Structure representing the data of multiple patients.
 */
struct patients {
	struct patient data[MAXIMUM_PATIENTS_COUNT]; /**< Array of patients. */
	size_t count;								 /**< Number of patients currently stored. */
};

/**
 * @enum patients_read_status
 * @brief Status of reading patients from a file.
 */
enum patients_read_status {
	patients_read_success, /**< Reading patients was successful. */
	patients_read_failure  /**< Reading patients failed. */
};

/**
 * @brief Reads patients data from a file.
 *
 * @param[out] patients Pointer to the patients structure to populate.
 * @param[in] file_name Name of the file to read from.
 *
 * @return The status of the read operation.
 */
enum patients_read_status patients_read(struct patients *patients, const char *file_name);

/**
 * @enum patients_write_status
 * @brief The status of writing patients to a file.
 */
enum patients_write_status {
	patients_write_success, /**< Writing patients was successful. */
	patients_write_failure	/**< Writing patients failed. */
};

/**
 * @brief Writes patients data to a file.
 *
 * @param[in] patients Pointer to the patients structure to write.
 * @param[in] file_name Name of the file to write to.
 *
 * @return The status of the write operation.
 */
enum patients_write_status patients_write(const struct patients *patients, const char *file_name);

/**
 * @enum patients_sign_up_status
 * @brief Status of signing up a new patient.
 */
enum patients_sign_up_status {
	patients_sign_up_success,					   /**< Sign up was successful. */
	patients_sign_up_failure_user_already_exists,  /**< User already exists. */
	patients_sign_up_failure_maximum_count_reached /**< Maximum patients count reached. */
};

/**
 * @brief Signs up a new patient.
 *
 * @param patients Pointer to the patients structure.
 * @param username The username of the new patient.
 * @param password The password of the new patient.
 * @param name The name of the new patient.
 *
 * @return The status of the sign up operation.
 */
enum patients_sign_up_status patients_sign_up(
	struct patients *patients,
	const char *username,
	const char *password,
	const char *name
);

/**
 * @enum patients_sign_in_status
 * @brief Status of signing in a patient.
 */
enum patients_sign_in_status {
	patients_sign_in_success,					   /**< Sign in was successful. */
	patients_sign_in_failure_user_does_not_exist,  /**< User does not exist. */
	patients_sign_in_failure_password_is_incorrect /**< Password is incorrect. */
};

/**
 * @brief Signs in a patient.
 *
 * @param[in] patients Pointer to the patients structure.
 * @param[in] username The username of the patient.
 * @param[in] password The password of the patient.
 *
 * @return The status of the sign in operation.
 */
enum patients_sign_in_status patients_sign_in(
	const struct patients *patients,
	const char *username,
	const char *password
);

#endif
