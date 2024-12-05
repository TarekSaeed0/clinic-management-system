#include <clinic_management_system/ui/sign_up_page.h>

#include <clinic_management_system/application.h>

struct _SignUpPage {
	GtkBox parent;
	GtkEntry *username_entry;
	GtkEntry *name_entry;
	GtkPasswordEntry *password_entry;
	GtkLabel *success_label;
	GtkLabel *error_label;
};

G_DEFINE_TYPE(SignUpPage, sign_up_page, GTK_TYPE_BOX)

static void sign_up_page_dispose(GObject *gobject) {
	gtk_widget_dispose_template(GTK_WIDGET(gobject), SIGN_UP_PAGE_TYPE);

	G_OBJECT_CLASS(sign_up_page_parent_class)->dispose(gobject);
}

static void sign_up_page_class_init(SignUpPageClass *class) {
	G_OBJECT_CLASS(class)->dispose = sign_up_page_dispose;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

	gtk_widget_class_set_template_from_resource(
		widget_class,
		"/com/github/TarekSaeed0/clinic_management_system/ui/sign_up_page.ui"
	);

	gtk_widget_class_bind_template_child(widget_class, SignUpPage, username_entry);
	gtk_widget_class_bind_template_child(widget_class, SignUpPage, name_entry);
	gtk_widget_class_bind_template_child(widget_class, SignUpPage, password_entry);
	gtk_widget_class_bind_template_child(widget_class, SignUpPage, success_label);
	gtk_widget_class_bind_template_child(widget_class, SignUpPage, error_label);
}

static void sign_up_page_init(SignUpPage *self) {
	gtk_widget_init_template(GTK_WIDGET(self));
}

static gboolean sign_up_success_callback(gpointer data) {
	SignUpPage *page = SIGN_UP_PAGE(data);
	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(data, GTK_TYPE_STACK));

	gtk_stack_set_visible_child_name(GTK_STACK(stack), "start-page");

	gtk_editable_delete_text(GTK_EDITABLE(page->username_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->name_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->password_entry), 0, -1);

	gtk_widget_set_visible(GTK_WIDGET(page->success_label), false);
	gtk_widget_set_visible(GTK_WIDGET(page->error_label), false);

	return G_SOURCE_REMOVE;
}

G_MODULE_EXPORT void sign_up_confirm_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	SignUpPage *page = SIGN_UP_PAGE(gtk_widget_get_ancestor(widget, SIGN_UP_PAGE_TYPE));

	const char *username = gtk_editable_get_text(GTK_EDITABLE(page->username_entry));
	const char *name = gtk_editable_get_text(GTK_EDITABLE(page->name_entry));
	const char *password = gtk_editable_get_text(GTK_EDITABLE(page->password_entry));

	ClinicManagementSystemApplication *application =
		CLINIC_MANAGEMENT_SYSTEM_APPLICATION(g_application_get_default());

	struct patients *patients = clinic_management_system_application_get_patients(application);
	switch (patients_sign_up(patients, username, password, name)) {
		case patients_sign_up_success: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), false);
			gtk_widget_set_visible(GTK_WIDGET(page->success_label), true);

			g_timeout_add(1500, (GSourceFunc)sign_up_success_callback, page);
		} break;
		case patients_sign_up_failure_username_is_invalid: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"Username is invalid, username must be a non-empty string containing only letters, "
				"digits, underscores, dashes and dots."
			);
		} break;
		case patients_sign_up_failure_user_already_exists: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"There already exists a patient with that username."
			);
		} break;
		case patients_sign_up_failure_maximum_count_reached: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"Maximum number of simultaneous patients in the system reached."
			);
		} break;
	}
}

G_MODULE_EXPORT void sign_up_back_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));
	SignUpPage *page = SIGN_UP_PAGE(gtk_widget_get_ancestor(widget, SIGN_UP_PAGE_TYPE));

	gtk_stack_set_visible_child_name(stack, "start-page");

	gtk_editable_delete_text(GTK_EDITABLE(page->username_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->name_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->password_entry), 0, -1);

	gtk_widget_set_visible(GTK_WIDGET(page->success_label), false);
	gtk_widget_set_visible(GTK_WIDGET(page->error_label), false);
}
