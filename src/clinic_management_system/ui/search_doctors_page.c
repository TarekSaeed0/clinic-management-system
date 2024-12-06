#include <clinic_management_system/ui/search_doctors_page.h>

#include <clinic_management_system/application.h>
#include <clinic_management_system/doctors.h>
#include <clinic_management_system/ui/doctors_list.h>

struct _SearchDoctorsPage {
	GtkBox parent;
	GtkSearchEntry *search_entry;
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

	// get doctors data
	ClinicManagementSystemApplication *application =
		CLINIC_MANAGEMENT_SYSTEM_APPLICATION(g_application_get_default());
	struct Doctor *doctors = clinic_management_system_application_get_doctors(application);

	// initialize doctors list
	struct TempStruct1 results = searchBySpeciality("", doctors);
	doctors_list_set_doctors(self->doctors_list, results.arrayToReturn, (size_t)results.count);
}

G_MODULE_EXPORT void search_doctors_search_entry_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	SearchDoctorsPage *page =
		SEARCH_DOCTORS_PAGE(gtk_widget_get_ancestor(widget, SEARCH_DOCTORS_PAGE_TYPE));

	// get the text in the search entry
	const char *speciality = gtk_editable_get_text(GTK_EDITABLE(page->search_entry));

	// get doctors data
	ClinicManagementSystemApplication *application =
		CLINIC_MANAGEMENT_SYSTEM_APPLICATION(g_application_get_default());
	struct Doctor *doctors = clinic_management_system_application_get_doctors(application);

	// get search results and update doctors list
	struct TempStruct1 results = searchBySpeciality(speciality, doctors);
	doctors_list_set_doctors(page->doctors_list, results.arrayToReturn, (size_t)results.count);
}

G_MODULE_EXPORT void search_doctors_back_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	// go back to the main page
	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));
	gtk_stack_set_visible_child_name(stack, "main-page");
}
