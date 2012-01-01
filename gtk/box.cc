#include <gtk/gtk.h>

#include <cox/box.hh>
#include "sig_data.hh"
#include "gobject_holder.hh"

namespace cox {
  struct horizontal_box::impl : public internal::gobject_holder<GtkHBox> {
    impl(GtkHBox *b) : gobject_holder(b) {}
  };

  struct vertical_box::impl : public internal::gobject_holder<GtkVBox> {
    impl(GtkVBox *b) : gobject_holder(b) {}
  };

  horizontal_box::horizontal_box()
    : pimpl(new impl(GTK_HBOX(gtk_hbox_new(FALSE, 0)))) {}
  vertical_box::vertical_box()
    : pimpl(new impl(GTK_VBOX(gtk_vbox_new(FALSE, 0)))) {}

  void horizontal_box::construct()
  {
    pimpl = new impl(GTK_HBOX(gtk_hbox_new(FALSE, 0)));
  }
  void vertical_box::construct()
  {
    pimpl = new impl(GTK_VBOX(gtk_vbox_new(FALSE, 0)));
  }

  horizontal_box::~horizontal_box() { delete pimpl; }
  vertical_box::~vertical_box() { delete pimpl; }

  void *horizontal_box::get_object() const { return pimpl->obj; }
  void *vertical_box::get_object() const { return pimpl->obj; }

  namespace {
    void show_content(box& b, GtkBox *gbox)
    {
      for (auto& w : b) {
        w.show();
        gtk_box_pack_start(GTK_BOX(gbox),
                           GTK_WIDGET(native::get_object_of<GtkWidget>(w)),
                           TRUE,
                           FALSE,
                           0);
      }
      gtk_widget_show(GTK_WIDGET(gbox));
    }
  }
  void vertical_box::show() { cox::show_content(*this, GTK_BOX(pimpl->obj)); }
  void horizontal_box::show() { cox::show_content(*this, GTK_BOX(pimpl->obj)); }
}
