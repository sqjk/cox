#include <gtk/gtk.h>

#include <cox/window.hh>
#include "sig_data.hh"
#include "gobject_holder.hh"

namespace cox {
  typedef internal::sig_data<window> sig_data;
    typedef decltype (&internal::call_funcs<window>) D;

  struct window::impl : public internal::gobject_holder<GtkWindow> {
    impl(GtkWindow *w) : gobject_holder(w), close_dat() {}
    internal::sig_data<window> close_dat;
  };

  window::window()
    : pimpl(new impl(GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL)))), 
      on_close_(new signal_type())
  {
  }

  void window::construct()
  {
    pimpl = new impl(GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL)));
    on_close_.reset(new signal_type());
  }

  void window::retitle(const char *str)
  {
    gtk_window_set_title(pimpl->obj, str);
  }

  void window::resize(distance w, distance h)
  {
    gtk_window_set_default_size(pimpl->obj, w, h);
  }

  std::string window::title() const
  {
    return std::string(gtk_window_get_title(pimpl->obj));
  }

  void window::show()
  {
    if (has_item()) {
      item().show();
      GtkWidget *obj = native::get_object_of<GtkWidget>(item());
      if (obj)
        gtk_container_add(GTK_CONTAINER(pimpl->obj), GTK_WIDGET(obj));
    }

    gtk_widget_show_all(GTK_WIDGET(pimpl->obj));

    COX_INSTANTIZE_CALL_FUNCS(window);
    pimpl->close_dat.set(this, on_close_.get());
    g_signal_connect_swapped(G_OBJECT (pimpl->obj), "destroy",
                             G_CALLBACK (&internal::call_funcs<window>),
                             &pimpl->close_dat);
  }

  void window::close()
  {
    gtk_widget_destroy(GTK_WIDGET(pimpl->obj));
  }

  window::~window() { if (pimpl) { close(); delete pimpl; } }

  void* window::get_object() const
  {
    return pimpl->obj;
  }
}
