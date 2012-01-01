#include <gtk/gtk.h>

#include <cox/textbox.hh>
#include "sig_data.hh"

namespace cox {
  struct textbox::impl {
    impl(GtkEntry *e) : e(e){}
    GtkEntry *e;
  };

  textbox::textbox()
    : pimpl(new impl(GTK_ENTRY(gtk_entry_new())))
  {
  }

  textbox::textbox(const std::string& str)
    : pimpl(new impl(GTK_ENTRY(gtk_entry_new())))
  {
    gtk_entry_set_text(pimpl->e, str.c_str());
  }

  textbox::textbox(const char *text)
    : pimpl(new impl(GTK_ENTRY(gtk_entry_new())))
  {
    gtk_entry_set_text(pimpl->e, text);
  }

  void textbox::construct()
  {
    pimpl = new impl(GTK_ENTRY(gtk_entry_new()));
  }

  textbox::~textbox()
  {
    delete pimpl;
  }

  std::string textbox::text() const
  {
    return gtk_entry_get_text(pimpl->e);
  }

  void textbox::retext(const char *str)
  {
    gtk_entry_set_text(pimpl->e, str);
  }

  bool textbox::editable() const
  {
    return gtk_editable_get_editable(GTK_EDITABLE(pimpl->e));
  }

  void textbox::make_editable(bool value)
  {
    gtk_editable_set_editable(GTK_EDITABLE(pimpl->e), value);
  }

  void textbox::show()
  {
    gtk_widget_show(GTK_WIDGET(pimpl->e));
  }

  void* textbox::get_object() const
  {
    return pimpl->e;
  }
}
