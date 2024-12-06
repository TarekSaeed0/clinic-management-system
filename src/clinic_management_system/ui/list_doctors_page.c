#include <clinic_management_system/ui/list_doctors_page.h>

#include <clinic_management_system/application.h>
#include <clinic_management_system/doctors.h>
#include <clinic_management_system/ui/doctors_list.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

struct _ListDoctorsPage {
	GtkBox parent;
	DoctorsList *doctors_list;
};

G_DEFINE_TYPE(ListDoctorsPage, list_doctors_page, GTK_TYPE_BOX)

static void list_doctors_page_dispose(GObject *gobject) {
	gtk_widget_dispose_template(GTK_WIDGET(gobject), LIST_DOCTORS_PAGE_TYPE);

	G_OBJECT_CLASS(list_doctors_page_parent_class)->dispose(gobject);
}

static void list_doctors_page_class_init(ListDoctorsPageClass *class) {
	G_OBJECT_CLASS(class)->dispose = list_doctors_page_dispose;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

	gtk_widget_class_set_template_from_resource(
		widget_class,
		"/com/github/TarekSaeed0/clinic_management_system/ui/list_doctors_page.ui"
	);

	gtk_widget_class_bind_template_child(widget_class, ListDoctorsPage, doctors_list);
}

static void list_doctors_page_init(ListDoctorsPage *self) {
	gtk_widget_init_template(GTK_WIDGET(self));

	ClinicManagementSystemApplication *application =
		CLINIC_MANAGEMENT_SYSTEM_APPLICATION(g_application_get_default());

	struct Doctor *doctors = clinic_management_system_application_get_doctors(application);

	doctors_list_set_doctors(self->doctors_list, doctors, 10);
}
G_MODULE_EXPORT void list_doctors_back_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));

	gtk_stack_set_visible_child_name(stack, "main-page");
}
