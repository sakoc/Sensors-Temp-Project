#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Sıcaklık verilerini okuyan fonksiyon
void read_temperature(GtkTextBuffer *buffer) {
    FILE *fp;
    char *command = "sensors"; // sensors komutuyla sıcaklık verilerini alınabilir

    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Error opening pipe!\n");
        return;
    }

    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(buffer, &iter); // Metin kutusunun sonuna git
    char sensor_data[256];
    while (fgets(sensor_data, 256, fp) != NULL) {
        gtk_text_buffer_insert(buffer, &iter, sensor_data, -1); // Metin kutusuna veriyi ekle
    }

    pclose(fp);
}

// Pencereyi güncelleyen fonksiyon
gboolean update_window(GtkWidget *widget) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));

    // Metin kutusunu temizle
    gtk_text_buffer_set_text(buffer, "", -1);

    // Sıcaklık verilerini oku ve metin kutusuna ekle
    read_temperature(buffer);

    // Timer'ı tekrar başlat
    return TRUE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *text_view;
    guint timer_id;

    gtk_init(&argc, &argv);

    // Pencere oluştur
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sensors Temp");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Metin kutusu oluştur
    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_container_add(GTK_CONTAINER(window), text_view);

    // Timer'ı başlat (her 2 saniyede bir güncelle)
    timer_id = g_timeout_add_seconds(1, (GSourceFunc)update_window, (gpointer)text_view);

    // Pencereyi göster
    gtk_widget_show_all(window);

    // GTK main döngüsünü başlat
    gtk_main();

    return 0;
}
