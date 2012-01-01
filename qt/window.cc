#include <cox/window.hh>

#include <QtGui>

namespace cox {
  struct window::impl : public QMainWindow {
    Q_OBJECT
 
public:
    typedef cox::window::signal_type signal_type;

    impl(cox::window *owner, signal_type *sig)
      : owner(owner),
        sig(sig)
    {
    }

    cox::window *owner;
    signal_type *sig;

 protected:
    void closeEvent(QCloseEvent */*event*/)
    {
      (*sig)(*owner);
    }
  };

  window::window()
    : pimpl(new impl(this, nullptr)),
      on_close_(new signal_type())
  {
    pimpl->sig = on_close_.get();
  }

  void window::construct()
  {
    on_close_.reset(new signal_type());
    pimpl = new impl(this, on_close_.get());
  }

  void window::show()
  {
    if (has_item()) {
      item().show();
      QWidget *obj = native::get_object_of<QWidget>(item());
      if (obj)
        pimpl->setCentralWidget(obj);
    }
    pimpl->show();
  }

  void window::close()
  {
    pimpl->close();
  }

  void window::retitle(const char *str)
  {
    pimpl->setWindowTitle(str);
  }

  void window::resize(distance w, distance h)
  {
    pimpl->resize(w, h);
  }

  std::string window::title() const
  {
    return pimpl->windowTitle().toStdString();
  }

  window::~window()
  {
    if (pimpl)
      delete pimpl;
  }

  void* window::get_object() const
  {
    return pimpl;
  }
}

#include "build/window.moc"
