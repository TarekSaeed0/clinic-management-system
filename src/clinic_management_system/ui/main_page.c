#include <clinic_management_system/ui/main_page.h>

struct _MainPage {
	GtkBox parent;
};

G_DEFINE_TYPE(MainPage, main_page, GTK_TYPE_BOX)

static void main_page_dispose(GObject *gobject) {
	gtk_widget_dispose_template(GTK_WIDGET(gobject), MAIN_PAGE_TYPE);

	G_OBJECT_CLASS(main_page_parent_class)->dispose(gobject);
}

static void main_page_class_init(MainPageClass *class) {
	G_OBJECT_CLASS(class)->dispose = main_page_dispose;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

	gtk_widget_class_set_template_from_resource(
		widget_class,
		"/com/github/TarekSaeed0/clinic_management_system/ui/main_page.ui"
	);
}

static void main_page_init(MainPage *self) {
	gtk_widget_init_template(GTK_WIDGET(self));
}

G_MODULE_EXPORT void list_doctors_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	// go to the list doctors page
	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));
	gtk_stack_set_visible_child_name(stack, "list-doctors-page");
}

G_MODULE_EXPORT void search_doctors_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	// go to the search doctors page
	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));
	gtk_stack_set_visible_child_name(stack, "search-doctors-page");
}

G_MODULE_EXPORT void sign_out_button_callback(GtkWidget *widget, gpointer data) {
	(void)widget, (void)data;

	// sign out and go back to the start page
	GtkStack *stack = GTK_STACK(gtk_widget_get_ancestor(widget, GTK_TYPE_STACK));
	gtk_stack_set_visible_child_name(stack, "start-page");
}
