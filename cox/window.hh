#ifndef COX_WINDOW_HH__
#define COX_WINDOW_HH__

#include <memory>

#include "widget.hh"
#include "coordinates.hh"
#include "property.hh"
#include "single-container.hh"

namespace cox {
  class window : public single_widget_container {
  public:
    typedef boost::signal<void(window&)> signal_type;
  private:
    class impl;
    impl *pimpl;

    std::unique_ptr<signal_type> on_close_; // signal_type is not movable

    void construct();

  public:
    window();
    window(const std::string& title) { construct(); retitle(title); }
    window(const char* title) { construct(); retitle(title); }
    window(window&& o)
      : single_widget_container(std::move(o)),
        pimpl(o.pimpl),
        on_close_(std::move(o.on_close_))
    { o.pimpl = nullptr; }

    template <typename... Args,
              typename = typename enable_if_modify_parameters<Args...>::type>
    window(Args&&... args) { construct(); modify(*this, std::forward<Args>(args)...); }

    virtual ~window();

    void retitle(const char *str);
    void retitle(const std::string& str) { retitle(str.c_str()); }
    std::string title() const;

    void resize(distance w, distance h);

    signal_type& on_close() { return *on_close_; }
    const signal_type& on_close() const { return *on_close_; }

    virtual void show();
    void close();

    virtual const window& parent_window() const { return *this; }
  private:
    virtual void* get_object() const;
  };

  namespace property {
    struct close;
    template <> struct connect<close> {
      connect(window::signal_type::slot_type f) : f(f) {}
      window::signal_type::slot_type f;
      template <typename T> void on(T& w) const { w.on_close().connect(f); }
    };
  }
}

#endif /* COX_WINDOW_HH__ */
