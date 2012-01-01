#include <cox/box.hh>

#include "layout_medium.hh"

#include <QtGui>

namespace cox {
  struct hbox_layout : public QHBoxLayout {
    Q_OBJECT
    
    public:
    hbox_layout(QWidget *parent) : QHBoxLayout(parent) {}
  };
  struct vbox_layout : public QVBoxLayout {
    Q_OBJECT
    
    public:
    vbox_layout(QWidget *parent) : QVBoxLayout(parent) {}
  };

  struct horizontal_box::impl {
    impl() : medium(new layout_medium),
             layout(new hbox_layout(medium))
    {
      medium->setLayout(layout);
    }
    ~impl() { have_to_delete()=false; delete medium; }
    layout_medium *medium;
    hbox_layout *layout;
    bool& have_to_delete() { return medium->have_to_delete; }
  };

  struct vertical_box::impl {
    impl() : medium(new layout_medium),
             layout(new vbox_layout(medium))
    {
      medium->setLayout(layout);
    }
    ~impl() { have_to_delete()=false; delete medium; }
    layout_medium *medium;
    vbox_layout *layout;
    bool& have_to_delete() { return medium->have_to_delete; }
  };

  horizontal_box::horizontal_box()
    : pimpl(new impl())
  {
  }
  vertical_box::vertical_box()
    : pimpl(new impl())
  {
  }

  void horizontal_box::construct()
  {
    pimpl = new impl();
  }
  void vertical_box::construct()
  {
    pimpl = new impl();
  }

  horizontal_box::~horizontal_box()
  {
    if (pimpl && pimpl->have_to_delete()) {
      delete pimpl;
    }
  }
  vertical_box::~vertical_box()
  {
    if (pimpl && pimpl->have_to_delete()) {
      delete pimpl;
    }
  }

  void *horizontal_box::get_object() const { return pimpl->medium; }
  void *vertical_box::get_object() const { return pimpl->medium; }

  namespace {
    void show_content(box& b, QBoxLayout *pimpl)
    {
      for (auto& w : b) {
        w.show();
        QWidget *obj = native::get_object_of<QWidget>(w);
        if (obj)
          pimpl->addWidget(obj);
      }
    }
  }
  void vertical_box::show() { show_content(*this, pimpl->layout); }
  void horizontal_box::show() { show_content(*this, pimpl->layout); }
}

#include "build/box.moc"
