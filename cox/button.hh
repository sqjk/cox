#ifndef COX_BUTTON_HH__
#define COX_BUTTON_HH__

#include "widget.hh"
#include "property.hh"

namespace cox {
  class button : public widget {
  public:
    typedef boost::signal<void(button&)> signal_type;
  private:
    class impl;
    impl *pimpl;
    std::unique_ptr<signal_type> on_click_;

    void construct();
  public:
    button();
    button(button&& o);
    button(const char *text);
    button(const std::string& str);
    template <typename... Args,
              typename = typename enable_if_modify_parameters<Args...>::type>
    button(Args&&... args) { construct(); modify(*this, std::forward<Args>(args)...); }

    virtual ~button();

    std::string text() const;
    void retext(const char * name);
    void retext(const std::string& name) { retext(name.c_str()); }

    signal_type& on_click() { return *on_click_; }
    const signal_type& on_click() const { return *on_click_; }

    virtual void show();
  private:
    virtual void* get_object() const;
  };

  namespace property {
    struct click; template <>
    struct connect<click>{ connect(button::signal_type::slot_type f) : f(f) {}
      button::signal_type::slot_type f;
      void on(button& b) const {b.on_click().connect(f);}
    };
  }
}

#endif /* COX_BUTTON_HH__ */

