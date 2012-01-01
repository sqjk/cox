#ifndef COX_TEXTBOX_HH__
#define COX_TEXTBOX_HH__

#include "widget.hh"
#include "property.hh"

namespace cox {
  class textbox : public widget {
  public:
    typedef boost::signal<void(textbox&)> signal_type;
  private:
    class impl; impl *pimpl;
    //signal_type on_edit_;
    void construct();
  public:
    textbox();
    textbox(textbox&& other) : widget(std::move(other)),
                               pimpl(other.pimpl) { other.pimpl = nullptr; }
    textbox(const char *text);
    textbox(const std::string& str);
    template <typename... Args,
              typename = typename enable_if_modify_parameters<Args...>::type>
    textbox(Args&&... args) { construct(); modify(*this, std::forward<Args>(args)...); }
    virtual ~textbox();

    // signal_type& on_click() { return on_click_; }
    // const signal_type& on_click() const { return on_click_; }

    void retext(const char *str);
    void retext(const std::string& str) { retext(str.c_str()); }
    std::string text() const;

    void make_editable(bool value=true);
    void make_uneditable() { make_editable(false); }
    bool editable() const;

    virtual void show();
  protected:
    virtual void* get_object() const;
  };
}


#endif /* COX_TEXTBOX_HH__ */
