#include <channel-usbredir.h>
#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <spice-client-gtk.h>
#include <spice-types.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
    SpiceSession *session;
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *spice_usb_device_widget;
} App;

void create_spice_usb_device_widget(App *app) {
    if (app->spice_usb_device_widget) {
        gtk_widget_destroy(app->spice_usb_device_widget);
    }

    app->spice_usb_device_widget = spice_usb_device_widget_new(app->session, "%s %s");
    gtk_box_pack_start(GTK_BOX(app->box), app->spice_usb_device_widget, TRUE, TRUE, 0);
    gtk_widget_show_all(app->spice_usb_device_widget);
}

void on_channel_new(SpiceSession *session, SpiceChannel *channel, gpointer data) {
    if (SPICE_IS_USBREDIR_CHANNEL(channel)) {
        create_spice_usb_device_widget(data);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    if (argc != 2) {
        fprintf(stderr, "%s <unix-socket|ip:port>\n", argv[0]);
        return 1;
    }

    App app = {0};
    app.session = spice_session_new();
    g_object_set(G_OBJECT(app.session), "enable-usbredir", TRUE, NULL);

    if (strchr(argv[1], ':') != NULL) {
        gchar **split = g_strsplit(argv[1], ":", 2);
        g_object_set(app.session, "host", split[0], "port", split[1], NULL);
        g_strfreev(split);
    } else {
        const char *spice_socket = argv[1];
        struct stat spice_socket_stat;

        if (stat(spice_socket, &spice_socket_stat) != 0) {
            fprintf(stderr, "error: '%s' does not exist\n", spice_socket);
            return 1;
        }

        if (!S_ISSOCK(spice_socket_stat.st_mode)) {
            fprintf(stderr, "error: '%s' is not a socket\n", spice_socket);
            return 1;
        }

        g_object_set(app.session, "unix-path", spice_socket, NULL);
    }

    spice_session_connect(app.session);

    app.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app.window), "Spice USB Redirect");
    gtk_window_set_resizable(GTK_WINDOW(app.window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(app.window), 20);

    app.box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_container_add(GTK_CONTAINER(app.window), app.box);

    create_spice_usb_device_widget(&app);

    g_signal_connect(app.session, "channel-new", G_CALLBACK(on_channel_new), &app);
    g_signal_connect(app.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(app.window);
    gtk_main();

    return 0;
}
