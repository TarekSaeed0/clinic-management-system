#ifndef CLINIC_MANAGEMENT_SYSTEM_LIST_DOCTORS_PAGE_H
#define CLINIC_MANAGEMENT_SYSTEM_LIST_DOCTORS_PAGE_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define LIST_DOCTORS_PAGE_TYPE list_doctors_page_get_type()
G_DECLARE_FINAL_TYPE(ListDoctorsPage, list_doctors_page, LIST_DOCTORS, PAGE, GtkBox)

G_END_DECLS

#endif
