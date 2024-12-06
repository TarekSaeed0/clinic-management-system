#ifndef CLINIC_MANAGEMENT_SYSTEM_APPLICATION_H
#define CLINIC_MANAGEMENT_SYSTEM_APPLICATION_H

#include <clinic_management_system/doctors.h>
#include <clinic_management_system/patients.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CLINIC_MANAGEMENT_SYSTEM_APPLICATION_TYPE clinic_management_system_application_get_type()
G_DECLARE_FINAL_TYPE(
	ClinicManagementSystemApplication,
	clinic_management_system_application,
	CLINIC_MANAGEMENT_SYSTEM,
	APPLICATION,
	GtkApplication
)

ClinicManagementSystemApplication *clinic_management_system_application_new(void);
struct PatientsData *clinic_management_system_application_get_patients(
	ClinicManagementSystemApplication *self
);
struct Doctor *clinic_management_system_application_get_doctors(
	ClinicManagementSystemApplication *self
);

G_END_DECLS

#endif
