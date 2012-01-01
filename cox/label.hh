#ifndef COX_LABEL_HH__
#define COX_LABEL_HH__

#include "widget.hh"
#include "property.hh"

namespace cox {
  class label : public widget {
  private:
    class impl; impl *pimpl;

  public:
    label();
    label(label&& o) : widget(std::move(o)), pimpl(o.pimpl) { o.pimpl = nullptr; }
    label(const char *text);
    label(const std::string& str);

    virtual ~label();

    std::string text() const;
    void retext(const std::string& name);

    virtual void show();
  private:
    virtual void* get_object() const;
  };
}

#endif /* COX_LABEL_HH__ */

