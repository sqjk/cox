#ifndef COX_WIDGET_HH__
#define COX_WIDGET_HH__

#include <memory>
#include <boost/signal.hpp>
#include <stdexcept>

namespace cox {
  class widget;

  namespace native {
    template <typename T>
    T *get_object_of(const widget& w);
  }

  class window;

  class widget {
  public:
    widget *parent_widget;

    typedef boost::signal<void(widget&)> signal_type;
    widget() : parent_widget(nullptr)  {}
    virtual ~widget();

    widget(widget&& w) : parent_widget(w.parent_widget) { w.parent_widget = nullptr; }
    widget(const widget&) =delete;
    widget& operator=(const widget&) =delete;

    virtual void show() =0;

    widget& parent()
    {
      return const_cast<widget&>(const_cast<const widget*>(this)->parent());
    }
    const widget& parent() const
    {
      if (!parent_widget)
        throw std::runtime_error("no parent set");
      return *parent_widget;
    }
    void reparent(widget& w) { parent_widget = &w; }

    window& parent_window()
    {
      return const_cast<window&>(const_cast<const widget*>(this)->parent().parent_window());
    }
    virtual const window& parent_window() const { return parent().parent_window(); }

    bool enabled() const;
    virtual void reenable(bool to=true);
    void disable() { reenable(false); }

  private:
    template <typename T> friend T *native::get_object_of(const widget& w);
    virtual void* get_object() const=0;// { return nullptr; }
  };

  namespace native {
    template <typename T>
    T *get_object_of(const widget& w) { return static_cast<T*>(w.get_object()); }
  }
}

#endif /* COX_WIDGET_HH__ */
