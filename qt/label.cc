#include <cox/label.hh>

#include <QtGui>

namespace cox {
  struct label::impl : public QLabel {
    Q_OBJECT
 
    public:

    impl(const char *text = "")
      : have_to_delete(true)
    {
      setText(text);
    }
    ~impl() { have_to_delete = false; }

    bool have_to_delete;
  };

  label::label()
    : pimpl(new impl())
  {
  }

  label::label(const std::string& str)
    : pimpl(new impl(str.c_str()))
  {
  }

  label::label(const char *text)
    : pimpl(new impl(text))
  {
  }

  label::~label()
  {
    if (pimpl && pimpl->have_to_delete) {
      delete pimpl;
    }
  }

  std::string label::text() const
  {
    return pimpl->text().toStdString();
  }

  void label::retext(const std::string& text)
  {
    return pimpl->setText(text.c_str());
  }

  void label::show()
  {
    //pimpl->show();
  }

  void* label::get_object() const
  {
    return pimpl;
  }
}

#include "build/label.moc"
