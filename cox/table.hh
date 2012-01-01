#ifndef COX_TABLE_HH
#define COX_TABLE_HH

#include <memory>
#include <type_traits>

#include "widget.hh"
#include "container.hh"

namespace cox {

  class table : public widget {
    class impl;
    impl *pimpl;
    void construct();

    std::list<std::unique_ptr<widget> > items;

    void handle_add(widget& w, size_t y, size_t x);
  public:
    template <typename Table>
    struct at_proxy {
      Table& that;
      size_t y;
      size_t x;

      at_proxy(Table& that, size_t y, size_t x)
        : that(that), y(y), x(x) {}
      
      template <typename T, typename... Args,
                typename = typename std::enable_if<std::is_base_of<widget, T>::value>::type>
      T& emplace(Args&&... args)
      {
        T *t = new T(std::forward<Args>(args)...);
        that.items.emplace_back(t);
        t->reparent(that);
        that.handle_add(*t, y, x);
        return *t;
      }
    };

    at_proxy<table> at(size_t y, size_t x) { return at_proxy<table>(*this, y, x); }
    at_proxy<const table> at(size_t y, size_t x) const { return at_proxy<const table>(*this, y, x); }

    table();
    template <typename... Args,
              typename = typename enable_if_modify_parameters<Args...>::type>
    table(Args&&... args) { construct(); modify(*this, std::forward<Args>(args)...); }
    virtual ~table();
    virtual void show();
  private:
    virtual void* get_object() const;
  };
}

#endif /* COX_TABLE_HH */
