#include <clinic_management_system/ui/search_doctors_page.h>

#include <clinic_management_system/application.h>
#include <clinic_management_system/doctors.h>
#include <clinic_management_system/ui/doctors_list.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

struct _SearchDoctorsPage {
	GtkBox parent;
	GtkEntry *search_entry;
	DoctorsList *doctors_list;
};

G_DEFINE_TYPE(SearchDoctorsPage, search_doctors_page, GTK_TYPE_BOX)

static void search_doctors_page_dispose(GObject *gobject) {
	gtk_widget_dispose_template(GTK_WIDGET(gobject), SEARCH_DOCTORS_PAGE_TYPE);

	G_OBJECT_CLASS(search_doctors_page_parent_class)->dispose(gobject);
}

static void search_doctors_page_class_init(SearchDoctorsPageClass *class) {
	G_OBJECT_CLASS(class)->dispose = search_doctors_page_dispose;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

	gtk_widget_class_set_template_from_resource(
		widget_class,
		"/com/github/TarekSaeed0/clinic_management_system/ui/search_doctors_page.ui"
	);

	gtk_widget_class_bind_template_child(widget_class, SearchDoctorsPage, search_entry);
	gtk_widget_class_bind_template_child(widget_class, SearchDoctorsPage, doctors_list);
}

static void search_doctors_page_init(SearchDoctorsPage *self) {
	gtk_widget_init_template(GTK_WIDGET(self));

	struct doctors doctors;
	if (doctors_read(&doctors, DOCTORS_FILE_NAME) == doctors_read_failure) {
		g_warning("Failed to read doctors file\n");
	}

	doctors_list_set_doctors(self->doctors_list, &doctors);
}

G_MODULE_EXPORT void search_doctors_search_entry_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	SearchDoctorsPage *page =
		SEARCH_DOCTORS_PAGE(gtk_widget_get_ancestor(widget, SEARCH_DOCTORS_PAGE_TYPE));

	const char *speciality = gtk_editable_get_text(GTK_EDITABLE(page->search_entry));

	ClinicManagementSystemApplication *application =
		CLINIC_MANAGEMENT_SYSTEM_APPLICATION(g_application_get_default());

	struct doctors *doctors = clinic_management_system_application_get_doctors(application);

	struct doctors results = doctors_search(doctors, speciality);
	doctors_list_set_doctors(page->doctors_list, &results);
}

G_MODULE_EXPORT void search_doctors_back_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));

	gtk_stack_set_visible_child_name(stack, "main-page");
}
