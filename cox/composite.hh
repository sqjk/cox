#ifndef COX_COMPOSED_HH
#define COX_COMPOSED_HH

#include "widget.hh"

namespace cox {
  template <typename W>
  class composite_widget : public cox::widget {
  protected:
    W content;
  private:
    virtual void *get_object() const { return cox::native::get_object_of<void>(content); }
  public:
    virtual void show() { content.show(); }

    template <typename... Args>
    composite_widget(Args&&... args) : content(std::forward<Args>(args)...) {}
    composite_widget(composite_widget&& o) :
      widget(std::move(o)),
      content(std::move(o.content)) {}
  };
}

#endif /* COX_COMPOSED_HH */
