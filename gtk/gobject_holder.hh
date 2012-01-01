#ifndef COX_GTK_GOBJECT_HOLDER_HH__
#define COX_GTK_GOBJECT_HOLDER_HH__

namespace cox {
  namespace internal {
    template <typename GObject>
    struct gobject_holder {
      gobject_holder(GObject *obj) : obj(obj) { g_object_ref_sink(G_OBJECT(obj)); }
      ~gobject_holder() { g_object_unref(G_OBJECT(obj)); }
      GObject *obj;
    };
  }
}

#endif /* COX_GTK_GOBJECT_HOLDER_HH__ */
