#include <cox/textbox.hh>

#include <QtGui>

namespace cox {
  struct textbox::impl : public QLineEdit {
    Q_OBJECT
 
    public:
    typedef textbox::signal_type signal_type;

    impl(textbox *owner, const char *text = "")
      : owner(owner),
        have_to_delete(true)
    {
      setText(text);
    }
    ~impl() { have_to_delete = false; }

    textbox *owner;
    bool have_to_delete;
  };

  textbox::textbox()
    : pimpl(new impl(this))
  {
  }

  textbox::textbox(const std::string& str)
    : pimpl(new impl(this, str.c_str()))
  {
  }

  textbox::textbox(const char *text)
    : pimpl(new impl(this, text))
  {
  }

  void textbox::construct()
  {
    pimpl = new impl(this);
  }

  textbox::~textbox()
  {
    if (pimpl && pimpl->have_to_delete) {
      delete pimpl;
    }
  }

  std::string textbox::text() const
  {
    return pimpl->text().toStdString();
  }

  void textbox::retext(const char *str)
  {
    return pimpl->setText(str);
  }


  bool textbox::editable() const
  {
    return pimpl->isReadOnly();
  }

  void textbox::make_editable(bool value)
  {
    pimpl->setReadOnly(!value);
  }

  void textbox::show()
  {
  }

  void* textbox::get_object() const
  {
    return pimpl;
  }
}

#include "build/textbox.moc"
