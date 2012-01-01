#include <gtk/gtk.h>

#include <cox/label.hh>
#include "gobject_holder.hh"

namespace cox {
  struct label::impl : public internal::gobject_holder<GtkLabel> {
    impl(GtkLabel *l) : gobject_holder(l) { }
  };

  label::label()
    : pimpl(new impl(GTK_LABEL(gtk_label_new(""))))
  {
  }

  label::label(const std::string& str)
    : pimpl(new impl(GTK_LABEL(gtk_label_new(str.c_str()))))
  {  }

  label::label(const char *text)
    : pimpl(new impl(GTK_LABEL(gtk_label_new(text))))
  {
  }

  label::~label()
  {
    delete pimpl;
  }

  void label::show()
  {
    gtk_widget_show(GTK_WIDGET(pimpl->obj));
  }

  std::string label::text() const
  {
    return std::string(gtk_label_get_text(pimpl->obj));
  }

  void label::retext(const std::string& text)
  {
    gtk_label_set_text(pimpl->obj, text.c_str());
  }

  void* label::get_object() const
  {
    return pimpl->obj;
  }
}
