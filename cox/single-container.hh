#ifndef COX_SINGLE_CONTAINER_HH__
#define COX_SINGLE_CONTAINER_HH__

#include <memory>
#include <type_traits>

#include "widget.hh"
#include "property.hh"

namespace cox {
  template <typename TItem>
  class single_item_container : public widget {
    std::unique_ptr<TItem> item_;
  protected:
    bool has_item() const { return item_.get(); }
    TItem& item() { return *item_; }
    const TItem& item() const { return *item_; }

  public:
    typedef TItem item_type;

    single_item_container() : item_() {}
    single_item_container(single_item_container&& o)
      : widget(std::move(o)),
        item_(std::move(o.item_)) { item_->reparent(*this); }

    template <typename T, typename... Args,
              typename = typename std::enable_if<std::is_base_of<item_type, T>::value>::type>
    T& emplace(Args&&... args)
    {
      T *t = new T(std::forward<Args>(args)...);
      item_.reset(t);
      item_->reparent(*this);
      return *t;
    }
    template <typename T,
              typename = typename std::enable_if<std::is_base_of<item_type, T>::value>::type>
    T& emplace_unique(std::unique_ptr<T>&& ptr)
    {
      T *val = ptr.get();
      item_ = std::move(ptr);
      item_->reparent(*this);
      return *val;
    }
  };
  typedef single_item_container<widget> single_widget_container;
}

#endif /* COX_SINGLE_CONTAINER_HH__ */
