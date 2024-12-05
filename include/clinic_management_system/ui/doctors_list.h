#ifndef CLINIC_MANAGEMENT_SYSTEM_DOCTORS_LIST_H
#define CLINIC_MANAGEMENT_SYSTEM_DOCTORS_LIST_H

#include <clinic_management_system/doctors.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define DOCTORS_LIST_ITEM_TYPE doctors_list_item_get_type()
G_DECLARE_FINAL_TYPE(DoctorsListItem, doctors_list_item, DOCTORS, LIST_ITEM, GtkBox)

DoctorsListItem *doctors_list_item_new(void);
void doctors_list_item_set_data(DoctorsListItem *self, struct doctor doctor);

#define DOCTORS_LIST_TYPE doctors_list_get_type()
G_DECLARE_FINAL_TYPE(DoctorsList, doctors_list, DOCTORS, LIST, GtkBox)

void doctors_list_set_doctors(DoctorsList *self, const struct doctors *doctors);

G_END_DECLS

#endif
