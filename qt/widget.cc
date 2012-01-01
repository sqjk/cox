#include <cox/widget.hh>

#include <QtGui>

namespace cox {
  widget::~widget()
  {
  }

  bool widget::enabled() const
  {
    return native::get_object_of<QWidget>(*this)->isEnabled();
  }

  void widget::reenable(bool to)
  {
    native::get_object_of<QWidget>(*this)->setEnabled(to);
  }
}
