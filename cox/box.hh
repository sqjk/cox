#ifndef COX_BOX_HH
#define COX_BOX_HH

#include <memory>

#include "widget.hh"
#include "container.hh"

namespace cox {
  class box : public container {
  public:
    box() =default;
    template <typename... Args,
              typename = typename enable_if_modify_parameters<Args...>::type>
    box(Args&&... args) : container(std::forward<Args>(args)...) { }
  };

  class horizontal_box : public box {
    class impl;
    impl *pimpl;
    void construct();
  public:
    horizontal_box();
    template <typename... Args,
              typename = typename enable_if_modify_parameters<Args...>::type>
    horizontal_box(Args&&... args) : box(std::forward<Args>(args)...) { construct();  }
    virtual ~horizontal_box();
    virtual void show();
  private:
    virtual void* get_object() const;
  };

  class vertical_box : public box {
    class impl;
    impl *pimpl;
    void construct();
  public:
    vertical_box();
    template <typename... Args,
              typename = typename enable_if_modify_parameters<Args...>::type>
    vertical_box(Args&&... args) : box(std::forward<Args>(args)...) { construct();  }
    virtual ~vertical_box();
    virtual void show();
  private:
    virtual void* get_object() const;
  };
}

#endif /* COX_BOX_HH */
