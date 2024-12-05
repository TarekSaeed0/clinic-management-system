#include <gtk/gtk.h>

#include <clinic_management_system/application.h>

int main(int argc, char *argv[]) {
	// Disable client-side decorations
	putenv((char[]){ "GTK_CSD=0" });

	return g_application_run(G_APPLICATION(clinic_management_system_application_new()), argc, argv);
}
