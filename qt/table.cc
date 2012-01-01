#include <cassert>
#include <cox/table.hh>

#include "layout_medium.hh"

#include <QtGui>

namespace cox {
  struct table_layout : public QGridLayout {
    Q_OBJECT
    
    public:
    table_layout(QWidget *parent) : QGridLayout(parent) {}
  };

  struct table::impl {
    impl() : medium(new layout_medium),
             layout(new table_layout(medium))
    {
      medium->setLayout(layout);
    }

    ~impl() { if (medium->have_to_delete) { medium->have_to_delete=false; delete medium; } }
    layout_medium *medium;
    table_layout *layout;
  };

  table::table()
    : pimpl(new impl())
  {
  }

  void table::construct()
  {
    pimpl = new impl();
  }

  table::~table()
  {
    delete pimpl;
  }

  void *table::get_object() const { return pimpl->medium; }

  void table::show()
  {
    for (auto& w : items) {
      w->show();
    }
  }

  void table::handle_add(widget& w, size_t y, size_t x)
  {
    QWidget *obj = native::get_object_of<QWidget>(w);
    assert(obj);
    pimpl->layout->addWidget(obj, x, y);
  }
}

#include "build/table.moc"
