#ifndef CLINIC_MANAGEMENT_SYSTEM_SIGN_UP_PAGE_H
#define CLINIC_MANAGEMENT_SYSTEM_SIGN_UP_PAGE_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SIGN_UP_PAGE_TYPE sign_up_page_get_type()
G_DECLARE_FINAL_TYPE(SignUpPage, sign_up_page, SIGN_UP, PAGE, GtkBox)

G_END_DECLS

#endif
