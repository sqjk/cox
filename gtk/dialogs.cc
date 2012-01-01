#include <gtk/gtk.h>

#include <cox/dialogs.hh>

namespace cox {
  namespace {
    path select(GtkFileChooserAction action, GtkWindow *parent=nullptr)
    {
    path p;
    GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open File", parent, action,
                                                     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                     GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                                     NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        p = filename;
        g_free (filename);
      }
    gtk_widget_destroy (dialog);
    return p;
    }
  }

  path select_save_file()
  {
    return select(GTK_FILE_CHOOSER_ACTION_OPEN);
  }
  path select_save_file(const window& w)
  {
    return select(GTK_FILE_CHOOSER_ACTION_OPEN, native::get_object_of<GtkWindow>(w));
  }
  path select_save_folder()
  {
    return select(GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER);
  }
  path select_save_folder(const window& w)
  {
    return select(GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER, native::get_object_of<GtkWindow>(w));
  }
  path select_open_file()
  {
    return select(GTK_FILE_CHOOSER_ACTION_OPEN);
  }
  path select_open_file(const window& w)
  {
    return select(GTK_FILE_CHOOSER_ACTION_OPEN, native::get_object_of<GtkWindow>(w));
  }
  path select_open_folder()
  {
    return select(GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
  }
  path select_open_folder(const window& w)
  {
    return select(GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, native::get_object_of<GtkWindow>(w));
  }


  namespace {
  }

  void message(const char *text)
  {
    message(text, 0);
  }
  void message(const char *text, const window& parent)
  {
      GtkWidget *dialog;
      dialog = gtk_message_dialog_new (native::get_object_of<GtkWindow>(parent),
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       GTK_MESSAGE_INFO,
                                       GTK_BUTTONS_CLOSE,
                                       text);
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);
    }
}
