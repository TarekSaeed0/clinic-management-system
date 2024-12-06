#include <clinic_management_system/ui/sign_in_page.h>

#include <clinic_management_system/application.h>

struct _SignInPage {
	GtkBox parent;
	GtkEntry *username_entry;
	GtkPasswordEntry *password_entry;
	GtkLabel *success_label;
	GtkLabel *error_label;
};

G_DEFINE_TYPE(SignInPage, sign_in_page, GTK_TYPE_BOX)

static void sign_in_page_dispose(GObject *gobject) {
	gtk_widget_dispose_template(GTK_WIDGET(gobject), SIGN_IN_PAGE_TYPE);

	G_OBJECT_CLASS(sign_in_page_parent_class)->dispose(gobject);
}

static void sign_in_page_class_init(SignInPageClass *class) {
	G_OBJECT_CLASS(class)->dispose = sign_in_page_dispose;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

	gtk_widget_class_set_template_from_resource(
		widget_class,
		"/com/github/TarekSaeed0/clinic_management_system/ui/sign_in_page.ui"
	);

	gtk_widget_class_bind_template_child(widget_class, SignInPage, username_entry);
	gtk_widget_class_bind_template_child(widget_class, SignInPage, password_entry);
	gtk_widget_class_bind_template_child(widget_class, SignInPage, success_label);
	gtk_widget_class_bind_template_child(widget_class, SignInPage, error_label);
}

static void sign_in_page_init(SignInPage *self) {
	gtk_widget_init_template(GTK_WIDGET(self));
}

static gboolean sign_in_success_callback(gpointer data) {
	SignInPage *page = SIGN_IN_PAGE(data);
	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(data, GTK_TYPE_STACK));

	gtk_stack_set_visible_child_name(GTK_STACK(stack), "main-page");

	gtk_editable_delete_text(GTK_EDITABLE(page->username_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->password_entry), 0, -1);

	gtk_widget_set_visible(GTK_WIDGET(page->success_label), false);
	gtk_widget_set_visible(GTK_WIDGET(page->error_label), false);

	return G_SOURCE_REMOVE;
}

static gboolean sign_in_failure_user_does_not_exit_callback(gpointer data) {
	SignInPage *page = SIGN_IN_PAGE(data);
	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(data, GTK_TYPE_STACK));

	gtk_stack_set_visible_child_name(GTK_STACK(stack), "sign-up-page");

	gtk_editable_delete_text(GTK_EDITABLE(page->username_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->password_entry), 0, -1);

	gtk_widget_set_visible(GTK_WIDGET(page->success_label), false);
	gtk_widget_set_visible(GTK_WIDGET(page->error_label), false);

	return G_SOURCE_REMOVE;
}

G_MODULE_EXPORT void sign_in_confirm_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	SignInPage *page = SIGN_IN_PAGE(gtk_widget_get_ancestor(widget, SIGN_IN_PAGE_TYPE));

	const char *username = gtk_editable_get_text(GTK_EDITABLE(page->username_entry));
	const char *password = gtk_editable_get_text(GTK_EDITABLE(page->password_entry));

	ClinicManagementSystemApplication *application =
		CLINIC_MANAGEMENT_SYSTEM_APPLICATION(g_application_get_default());

	struct PatientsData *patients = clinic_management_system_application_get_patients(application);
	switch (logIn(*patients, username, password)) {
		case foundAccount: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), false);
			gtk_widget_set_visible(GTK_WIDGET(page->success_label), true);

			g_timeout_add(1500, (GSourceFunc)sign_in_success_callback, page);
		} break;
		case noAccount: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(
				GTK_LABEL(page->error_label),
				"There isn't a patient with that username, you should sign up first."
			);

			g_timeout_add(3000, (GSourceFunc)sign_in_failure_user_does_not_exit_callback, page);
		} break;
		case noPassword: {
			gtk_widget_set_visible(GTK_WIDGET(page->error_label), true);
			gtk_label_set_text(GTK_LABEL(page->error_label), "The password is incorrect.");
		} break;
	}
}

G_MODULE_EXPORT void sign_in_back_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));
	SignInPage *page = SIGN_IN_PAGE(gtk_widget_get_ancestor(widget, SIGN_IN_PAGE_TYPE));

	gtk_stack_set_visible_child_name(stack, "start-page");

	gtk_editable_delete_text(GTK_EDITABLE(page->username_entry), 0, -1);
	gtk_editable_delete_text(GTK_EDITABLE(page->password_entry), 0, -1);

	gtk_widget_set_visible(GTK_WIDGET(page->success_label), false);
	gtk_widget_set_visible(GTK_WIDGET(page->error_label), false);
}
