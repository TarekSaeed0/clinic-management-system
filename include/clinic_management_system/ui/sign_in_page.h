#ifndef CLINIC_MANAGEMENT_SYSTEM_SIGN_IN_PAGE_H
#define CLINIC_MANAGEMENT_SYSTEM_SIGN_IN_PAGE_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SIGN_IN_PAGE_TYPE sign_in_page_get_type()
G_DECLARE_FINAL_TYPE(SignInPage, sign_in_page, SIGN_IN, PAGE, GtkBox)

G_END_DECLS

#endif
