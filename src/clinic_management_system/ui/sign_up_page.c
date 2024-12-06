#include <clinic_management_system/ui/sign_up_page.h>

#include <clinic_management_system/application.h>
#include <gtk/gtk.h>

struct _SignUpPage {
	GtkBox parent;
	GtkEntry *username_entry;
	GtkEntry *name_entry;
	GtkPasswordEntry *password_entry;
	GtkPasswordEntry *confirm_password_entry;
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
	gtk_widget_class_bind_template_child(widget_class, SignUpPage, confirm_password_entry);
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

	// clear the sign up entries
	gtk_editable_delete_text(GTK_EDITABLE(page->username_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->name_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->password_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->confirm_password_entry), 0, -1);

	// HACK: rehide password entry
	gtk_password_entry_set_show_peek_icon(page->password_entry, false);
	gtk_password_entry_set_show_peek_icon(page->password_entry, true);
	gtk_password_entry_set_show_peek_icon(page->confirm_password_entry, false);
	gtk_password_entry_set_show_peek_icon(page->confirm_password_entry, true);

	// hide any success or error messages
	gtk_widget_set_visible(GTK_WIDGET(page->success_label), false);
	gtk_widget_set_visible(GTK_WIDGET(page->error_label), false);

	// remove the timeout, so the function is only called once
	return G_SOURCE_REMOVE;
}

G_MODULE_EXPORT void sign_up_confirm_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	SignUpPage *page = SIGN_UP_PAGE(gtk_widget_get_ancestor(widget, SIGN_UP_PAGE_TYPE));

	// get the text in the entries
	const char *username = gtk_editable_get_text(GTK_EDITABLE(page->username_entry));
	const char *name = gtk_editable_get_text(GTK_EDITABLE(page->name_entry));
	const char *password = gtk_editable_get_text(GTK_EDITABLE(page->password_entry));
	const char *confirm_password =
		gtk_editable_get_text(GTK_EDITABLE(page->confirm_password_entry));

	// get patients data
	ClinicManagementSystemApplication *application =
		CLINIC_MANAGEMENT_SYSTEM_APPLICATION(g_application_get_default());
	struct PatientsData *patients = clinic_management_system_application_get_patients(application);

	// try to sign up and handle any errors
	switch (sign_up(patients, name, username, password, confirm_password)) {
		case signed_up_successfuly: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), false);
			gtk_widget_set_visible(GTK_WIDGET(page->success_label), true);

			// show a success message and go back to the start page after a bit of time, so the user
			// can see the message
			g_timeout_add(1500, (GSourceFunc)sign_up_success_callback, page);
		} break;
		// show error message according to the return value
		case short_username: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"Username is too short, username must be at least four characters long."
			);
		} break;
		case invalid_username: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"Username is invalid, username must not contain whitespace."
			);
		} break;
		case existed_username: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"There already exists a patient with that username."
			);
		} break;
		case short_name: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"Name is too short, name must be at least four characters long."
			);
		} break;
		case invalid_name: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"Name is invalid, name must only contain letters and spaces and must contain at "
				"least four letters"
			);
		} break;
		case invalid_password: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"Password is invalid, password must be at least four characters long."
			);
		} break;
		case Passwords_didnot_match: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"Password and confirm password do not match."
			);
		} break;
		case max_Patients: {
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

	// go back to the start page
	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));
	gtk_stack_set_visible_child_name(stack, "start-page");

	SignUpPage *page = SIGN_UP_PAGE(gtk_widget_get_ancestor(widget, SIGN_UP_PAGE_TYPE));

	// clear the sign up entries
	gtk_editable_delete_text(GTK_EDITABLE(page->username_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->name_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->password_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->confirm_password_entry), 0, -1);

	// HACK: rehide password entry
	gtk_password_entry_set_show_peek_icon(page->password_entry, false);
	gtk_password_entry_set_show_peek_icon(page->password_entry, true);
	gtk_password_entry_set_show_peek_icon(page->confirm_password_entry, false);
	gtk_password_entry_set_show_peek_icon(page->confirm_password_entry, true);

	// hide any success or error messages
	gtk_widget_set_visible(GTK_WIDGET(page->success_label), false);
	gtk_widget_set_visible(GTK_WIDGET(page->error_label), false);
}
