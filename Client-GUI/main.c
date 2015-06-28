#include <gtk/gtk.h>

static void on_activate(GtkApplication* app, gpointer user_data);

int main(int argc, char* argv[]){

    GtkApplication *app;
    int status;
    app = gtk_application_new("cs513.zhshr.ui", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;

/*

    GtkBuilder * builder;
    GObject *window;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("form.xml");
    window = gtk_builder_get_object(builder, "window1");
    gtk_widget_show_all(window);
    gtk_main();
    return 0;

*/
}

static void on_activate(GtkApplication* app, gpointer user_data){

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;

/*

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "CS513 PROJECT CLIENT");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280,720);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    gtk_container_set_border_width(GTK_WINDOW(window),20);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

*/
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "form.xml", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    gtk_application_add_window(app, window);
   // gtk_window_set_default_size(GTK_WINDOW(window), 1280,720);


    gtk_widget_show_all(window);
}
