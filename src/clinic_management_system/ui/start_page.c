#include <clinic_management_system/ui/start_page.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

struct _StartPage {
	GtkBox parent;
};

G_DEFINE_TYPE(StartPage, start_page, GTK_TYPE_BOX)

static void start_page_dispose(GObject *gobject) {
	gtk_widget_dispose_template(GTK_WIDGET(gobject), START_PAGE_TYPE);

	G_OBJECT_CLASS(start_page_parent_class)->dispose(gobject);
}

static void start_page_class_init(StartPageClass *class) {
	G_OBJECT_CLASS(class)->dispose = start_page_dispose;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

	gtk_widget_class_set_template_from_resource(
		widget_class,
		"/com/github/TarekSaeed0/clinic_management_system/ui/start_page.ui"
	);
}

static void start_page_init(StartPage *self) {
	gtk_widget_init_template(GTK_WIDGET(self));
}

G_MODULE_EXPORT void sign_up_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));

	gtk_stack_set_visible_child_name(stack, "sign-up-page");
}

G_MODULE_EXPORT void sign_in_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));

	gtk_stack_set_visible_child_name(stack, "sign-in-page");
}

G_MODULE_EXPORT void quit_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	GApplication *application = g_application_get_default();

	g_application_quit(application);
}
