#include <gtk/gtk.h>

#include <cox/table.hh>
#include "sig_data.hh"
#include "gobject_holder.hh"

namespace cox {
  struct table::impl : public internal::gobject_holder<GtkTable> {
    impl(GtkTable *t) : gobject_holder(t) {}
  };

  table::table()
    : pimpl(new impl(GTK_TABLE(gtk_table_new(0, 0, FALSE)))) {}

  void table::construct()
  {
    pimpl = new impl(GTK_TABLE(gtk_table_new(0, 0, FALSE)));
  }

  table::~table() { delete pimpl; }

  void *table::get_object() const { return pimpl->obj; }

  void table::show()
  {
    for (auto& w : items) {
      w->show();
    }
  }

  void table::handle_add(widget& w, size_t y, size_t x)
  {
    GtkWidget *obj = native::get_object_of<GtkWidget>(w);
    assert(obj);
    gtk_table_attach_defaults(pimpl->obj, obj, x, x+1, y, y+1);
  }
}
