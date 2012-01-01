#include <gtk/gtk.h>

#include <cox/button.hh>
#include "sig_data.hh"
#include "gobject_holder.hh"

namespace cox {
  typedef internal::sig_data<button> sig_data;

  struct button::impl : public internal::gobject_holder<GtkButton> {
    impl(GtkButton *b) : gobject_holder(b), click_dat() {}
    sig_data click_dat;
  };

  button::button()
    : pimpl(new impl(GTK_BUTTON(gtk_button_new()))),
      on_click_(new signal_type())
  {
    COX_INSTANTIZE_CALL_FUNCS(button);
    pimpl->click_dat.set(this, on_click_.get());
    g_signal_connect_swapped(G_OBJECT (pimpl->obj), "clicked",
                             G_CALLBACK (&internal::call_funcs<button>),
                             &pimpl->click_dat);
  }

  void button::construct()
  {
    pimpl = new impl(GTK_BUTTON(gtk_button_new()));
    on_click_.reset(new signal_type());
    pimpl->click_dat.set(this, on_click_.get());
    g_signal_connect_swapped(G_OBJECT (pimpl->obj), "clicked",
                             G_CALLBACK (&internal::call_funcs<button>),
                             &pimpl->click_dat);
  }

  button::button(const std::string& str)
    : pimpl(new impl(GTK_BUTTON(gtk_button_new_with_label(str.c_str())))),
      on_click_(new signal_type())
  {
    pimpl->click_dat.set(this, on_click_.get());
    g_signal_connect_swapped(G_OBJECT (pimpl->obj), "clicked",
                             G_CALLBACK (&internal::call_funcs<button>),
                             &pimpl->click_dat);
  }

  button::button(const char *text)
    : pimpl(new impl(GTK_BUTTON(gtk_button_new_with_label(text)))),
      on_click_(new signal_type())
  {
    pimpl->click_dat.set(this, on_click_.get());
    g_signal_connect_swapped(G_OBJECT (pimpl->obj), "clicked",
                             G_CALLBACK (&internal::call_funcs<button>),
                             &pimpl->click_dat);
  }

  button::button(button&& o)
    : widget(std::move(o)),
      pimpl(o.pimpl),
      on_click_(std::move(o.on_click_))
  {
    o.pimpl = nullptr;
    pimpl->click_dat.owner = this;
  }

  button::~button()
  {
    delete pimpl;
  }

  void button::show()
  {
    gtk_widget_show(GTK_WIDGET(pimpl->obj));
  }

  std::string button::text() const
  {
    return std::string(gtk_button_get_label(pimpl->obj));
  }

  void button::retext(const char * text)
  {
    return gtk_button_set_label(pimpl->obj, text);
  }


  void* button::get_object() const
  {
    return pimpl->obj;
  }
}
