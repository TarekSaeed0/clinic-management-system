#ifndef CLINIC_MANAGEMENT_SYSTEM_SEARCH_DOCTORS_PAGE_H
#define CLINIC_MANAGEMENT_SYSTEM_SEARCH_DOCTORS_PAGE_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SEARCH_DOCTORS_PAGE_TYPE search_doctors_page_get_type()
G_DECLARE_FINAL_TYPE(SearchDoctorsPage, search_doctors_page, SEARCH_DOCTORS, PAGE, GtkBox)

G_END_DECLS

#endif
