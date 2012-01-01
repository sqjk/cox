#include <cox/button.hh>

#include <QtGui>

namespace cox {
  struct button::impl : public QPushButton {
    Q_OBJECT
 
    public:
    typedef cox::button::signal_type signal_type;

    impl(cox::button *owner, const char *text = "")
      : owner(owner),
        sig_click(0),
        have_to_delete(true)
    {
      setText(text);
    }
    ~impl() { have_to_delete = false; }

    cox::button *owner;
    signal_type *sig_click;
    bool have_to_delete;

  protected:
    void mouseReleaseEvent(QMouseEvent */*event*/)
    {
      (*sig_click)(*owner);
    }
  };

  button::button()
    : pimpl(new impl(this)),
      on_click_(new signal_type())
  {
    pimpl->sig_click = on_click_.get();
  }
  void button::construct()
  {
    pimpl = new impl(this);
    on_click_.reset(new signal_type());
    pimpl->sig_click = on_click_.get();
  }


  button::button(const std::string& str)
    : pimpl(new impl(this, str.c_str())),
      on_click_(new signal_type())
  {
    pimpl->sig_click = on_click_.get();
  }

  button::button(const char *text)
    : pimpl(new impl(this, text)),
      on_click_(new signal_type())
  {
    pimpl->sig_click = on_click_.get();
  }

  button::button(button&& o)
    : widget(std::move(o)),
      pimpl(o.pimpl),
      on_click_(std::move(o.on_click_))
  {
    o.pimpl = nullptr;
    pimpl->owner = this;
  }

  button::~button()
  {
    if (pimpl && pimpl->have_to_delete) {
      delete pimpl;
    }
  }

  std::string button::text() const
  {
    return pimpl->text().toStdString();
  }

  void button::retext(const char *text)
  {
    return pimpl->setText(text);
  }

  void button::show()
  {
    //pimpl->show();
  }

  void* button::get_object() const
  {
    return pimpl;
  }
}

#include "build/button.moc"
