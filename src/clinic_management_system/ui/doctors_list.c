#include <clinic_management_system/ui/doctors_list.h>

#include <gio/gio.h>
#include <gtk/gtk.h>

struct _DoctorsListItem {
	GtkBox parent;
	GtkLabel *name_label;
	GtkLabel *speciality_label;
	GtkLabel *clinic_address_label;
	GtkLabel *visita_label;
};

G_DEFINE_TYPE(DoctorsListItem, doctors_list_item, GTK_TYPE_BOX)

static void doctors_list_item_dispose(GObject *gobject) {
	gtk_widget_dispose_template(GTK_WIDGET(gobject), DOCTORS_LIST_ITEM_TYPE);

	G_OBJECT_CLASS(doctors_list_item_parent_class)->dispose(gobject);
}

static void doctors_list_item_class_init(DoctorsListItemClass *class) {
	G_OBJECT_CLASS(class)->dispose = doctors_list_item_dispose;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

	gtk_widget_class_set_template_from_resource(
		widget_class,
		"/com/github/TarekSaeed0/clinic_management_system/ui/doctors_list_item.ui"
	);

	gtk_widget_class_bind_template_child(widget_class, DoctorsListItem, name_label);
	gtk_widget_class_bind_template_child(widget_class, DoctorsListItem, speciality_label);
	gtk_widget_class_bind_template_child(widget_class, DoctorsListItem, clinic_address_label);
	gtk_widget_class_bind_template_child(widget_class, DoctorsListItem, visita_label);
}

static void doctors_list_item_init(DoctorsListItem *self) {
	gtk_widget_init_template(GTK_WIDGET(self));
}

DoctorsListItem *doctors_list_item_new(void) {
	return g_object_new(doctors_list_item_get_type(), NULL);
}

void doctors_list_item_set_data(DoctorsListItem *self, struct Doctor doctor) {
	gtk_label_set_text(self->name_label, doctor.name);
	gtk_label_set_text(self->speciality_label, doctor.speciality);
	gtk_label_set_text(self->clinic_address_label, doctor.address);
	gtk_label_set_text(self->visita_label, doctor.visita);
}

typedef struct _Doctor {
	GObject parent_instance;
	struct Doctor doctor;
} Doctor;

typedef struct _DoctorClass {
	GObjectClass parent_class;
} DoctorClass;

G_DEFINE_TYPE(Doctor, doctor, G_TYPE_OBJECT)
#define DOCTOR_TYPE doctor_get_type()

static void doctor_init(Doctor *self) {
	(void)self;
}

static void doctor_finalize(GObject *object) {
	G_OBJECT_CLASS(doctor_parent_class)->finalize(object);
}

static void doctor_class_init(DoctorClass *class) {
	G_OBJECT_CLASS(class)->finalize = doctor_finalize;
}

static Doctor *doctor_new(struct Doctor doctor) {
	Doctor *self = g_object_new(doctor_get_type(), NULL);
	self->doctor = doctor;
	return self;
}

struct _DoctorsList {
	GtkBox parent;
	GtkListView *list_view;
};

G_DEFINE_TYPE(DoctorsList, doctors_list, GTK_TYPE_BOX)

static void doctors_list_dispose(GObject *gobject) {
	gtk_widget_dispose_template(GTK_WIDGET(gobject), DOCTORS_LIST_TYPE);

	G_OBJECT_CLASS(doctors_list_parent_class)->dispose(gobject);
}

static void doctors_list_class_init(DoctorsListClass *class) {
	G_OBJECT_CLASS(class)->dispose = doctors_list_dispose;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

	gtk_widget_class_set_template_from_resource(
		widget_class,
		"/com/github/TarekSaeed0/clinic_management_system/ui/doctors_list.ui"
	);

	gtk_widget_class_bind_template_child(widget_class, DoctorsList, list_view);
}

static void doctors_list_item_setup(
	GtkSignalListItemFactory *factory,
	GtkListItem *list_item,
	gpointer data
) {
	(void)factory, (void)data;

	DoctorsListItem *item = doctors_list_item_new();

	gtk_list_item_set_child(list_item, GTK_WIDGET(item));
}

static void doctors_list_item_bind(
	GtkSignalListItemFactory *factory,
	GtkListItem *list_item,
	gpointer data
) {
	(void)factory, (void)data;

	DoctorsListItem *item = DOCTORS_LIST_ITEM(gtk_list_item_get_child(list_item));
	Doctor *doctor = gtk_list_item_get_item(list_item);

	doctors_list_item_set_data(item, doctor->doctor);
}

static void doctors_list_init(DoctorsList *self) {
	gtk_widget_init_template(GTK_WIDGET(self));

	GtkSignalListItemFactory *factory =
		GTK_SIGNAL_LIST_ITEM_FACTORY(gtk_signal_list_item_factory_new());

	g_signal_connect(factory, "setup", G_CALLBACK(doctors_list_item_setup), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(doctors_list_item_bind), NULL);

	gtk_list_view_set_factory(self->list_view, GTK_LIST_ITEM_FACTORY(factory));
}

void doctors_list_set_doctors(
	DoctorsList *self,
	const struct Doctor doctors[static 10],
	size_t count
) {
	GListStore *list_store = g_list_store_new(DOCTOR_TYPE);

	for (size_t i = 0; i < count; i++) {
		g_list_store_append(list_store, doctor_new(doctors[i]));
	}

	gtk_list_view_set_model(
		self->list_view,
		GTK_SELECTION_MODEL(gtk_no_selection_new(G_LIST_MODEL(list_store)))
	);
}
