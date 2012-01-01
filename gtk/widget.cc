#include <gtk/gtk.h>

#include <cox/widget.hh>

namespace cox {
  widget::~widget()
  {
  }

  bool widget::enabled() const
  {
    return gtk_widget_get_sensitive(native::get_object_of<GtkWidget>(*this));
  }

  void widget::reenable(bool to)
  {
    gtk_widget_set_sensitive(native::get_object_of<GtkWidget>(*this), to);
  }
}
