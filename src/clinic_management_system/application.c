#include <clinic_management_system/application.h>

#include <clinic_management_system/ui/doctors_list.h>
#include <clinic_management_system/ui/list_doctors_page.h>
#include <clinic_management_system/ui/main_page.h>
#include <clinic_management_system/ui/search_doctors_page.h>
#include <clinic_management_system/ui/sign_in_page.h>
#include <clinic_management_system/ui/sign_up_page.h>
#include <clinic_management_system/ui/start_page.h>
#include <glib.h>
#include <gtk/gtk.h>

struct _ClinicManagementSystemApplication {
	GtkApplication parent;
	struct patients patients;
	struct doctors doctors;
};

G_DEFINE_TYPE(
	ClinicManagementSystemApplication,
	clinic_management_system_application,
	GTK_TYPE_APPLICATION
)

static void clinic_management_system_application_init(ClinicManagementSystemApplication *self) {
	if (patients_read(&self->patients, PATIENTS_FILE_NAME) == patients_read_failure) {
		g_warning("Failed to read patients file\n");
	}

	if (doctors_read(&self->doctors, DOCTORS_FILE_NAME) == doctors_read_failure) {
		g_warning("Failed to read doctors file\n");
	}
}

static void remove_style_provider(gpointer data) {
	GtkStyleProvider *provider = GTK_STYLE_PROVIDER(data);
	gtk_style_context_remove_provider_for_display(gdk_display_get_default(), provider);
}

static void clinic_management_system_application_activate(GApplication *application) {
	g_type_ensure(START_PAGE_TYPE);
	g_type_ensure(SIGN_UP_PAGE_TYPE);
	g_type_ensure(SIGN_IN_PAGE_TYPE);
	g_type_ensure(MAIN_PAGE_TYPE);
	g_type_ensure(DOCTORS_LIST_ITEM_TYPE);
	g_type_ensure(DOCTORS_LIST_TYPE);
	g_type_ensure(LIST_DOCTORS_PAGE_TYPE);
	g_type_ensure(SEARCH_DOCTORS_PAGE_TYPE);

	GtkBuilder *builder = gtk_builder_new_from_resource(
		"/com/github/TarekSaeed0/clinic_management_system/ui/window.ui"
	);

	GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
	gtk_window_set_application(window, GTK_APPLICATION(application));

	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_resource(
		provider,
		"/com/github/TarekSaeed0/clinic_management_system/style.css"
	);
	gtk_style_context_add_provider_for_display(
		gdk_display_get_default(),
		GTK_STYLE_PROVIDER(provider),
		G_MAXUINT
	);
	g_object_set_data_full(
		G_OBJECT(window),
		"provider",
		GTK_STYLE_PROVIDER(provider),
		remove_style_provider
	);

	GtkIconTheme *icon_theme = gtk_icon_theme_get_for_display(gdk_display_get_default());
	gtk_icon_theme_add_resource_path(
		icon_theme,
		"/com/github/TarekSaeed0/clinic_management_system/icons/hicolor"
	);

	gtk_window_present(window);

	g_object_unref(builder);
}

static void clinic_management_system_application_shutdown(GApplication *self) {
	ClinicManagementSystemApplication *application = CLINIC_MANAGEMENT_SYSTEM_APPLICATION(self);

	if (patients_write(&application->patients, PATIENTS_FILE_NAME) == patients_write_failure) {
		g_warning("Failed to write patients file\n");
	}

	G_APPLICATION_CLASS(clinic_management_system_application_parent_class)->shutdown(self);
}

static void clinic_management_system_application_class_init(ClinicManagementSystemApplicationClass
																*class) {
	G_APPLICATION_CLASS(class)->activate = clinic_management_system_application_activate;
	G_APPLICATION_CLASS(class)->shutdown = clinic_management_system_application_shutdown;
}

ClinicManagementSystemApplication *clinic_management_system_application_new(void) {
	return g_object_new(
		CLINIC_MANAGEMENT_SYSTEM_APPLICATION_TYPE,
		"application-id",
		"com.github.TarekSaeed0.clinic_management_system",
		"flags",
		G_APPLICATION_DEFAULT_FLAGS,
		NULL
	);
}

struct patients *clinic_management_system_application_get_patients(
	ClinicManagementSystemApplication *self
) {
	return &self->patients;
}

struct doctors *clinic_management_system_application_get_doctors(
	ClinicManagementSystemApplication *self
) {
	return &self->doctors;
}
