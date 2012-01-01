#ifndef COX_CONTAINER_HH__
#define COX_CONTAINER_HH__

#include <memory>
#include <boost/iterator/indirect_iterator.hpp>

#include "widget.hh"
#include "property.hh"

namespace cox {
  template <typename TItem>
  class basic_container : public widget {
  public:
    typedef TItem item_type;
  private:
    std::list<std::unique_ptr<item_type> > items;

  protected:
    void handle_add_back(item_type&)  {}//=0;
    void handle_add_front(item_type&) {}//=0;

  public:
    basic_container() : items() {}
    basic_container(basic_container&& o)
      : widget(std::move(o)),
        items(std::move(o.items)) { for (auto& w : items) w.reparent(*this); }
    template <typename... Args,
              typename = typename enable_if_modify_parameters<Args...>::type>
    basic_container(Args&&... args)
      : items()
    {
      modify(*this, std::forward<Args>(args)...);
    }

    typedef boost::indirect_iterator<
      typename std::list<std::unique_ptr<item_type> >::const_iterator> const_iterator;    
    typedef boost::indirect_iterator<
      typename std::list<std::unique_ptr<item_type> >::iterator> iterator;
    typedef const item_type* const_pointer;
    typedef const item_type& const_reference;
    typedef typename iterator::difference_type difference_type;
    typedef item_type* pointer;
    typedef item_type& reference;
    typedef typename std::list<std::unique_ptr<item_type> >::size_type size_type;
    typedef item_type value_type;

    size_type size() const { return items.size(); }

    iterator begin() { return iterator(items.begin()); }
    const_iterator begin() const { return iterator(items.begin()); }
    const_iterator cbegin() const { return iterator(items.begin()); }
    iterator end() { return iterator(items.end()); }
    const_iterator end() const { return iterator(items.end()); }
    const_iterator cend() const { return iterator(items.end()); }
    
    template <typename T, typename... Args,
              typename = typename std::enable_if<std::is_base_of<item_type, T>::value>::type>
    T& emplace(Args&&... args)
    {
      T *t = new T(std::forward<Args>(args)...);
      items.emplace_back(t);
      t->reparent(*this);
      handle_add_back(*t);
      return *t;
    }
    template <typename T, typename... Args,
              typename = typename std::enable_if<std::is_base_of<item_type, T>::value>::type>
    T& emplace_front(Args&&... args)
    {
      T *t = new T(std::forward<Args>(args)...);
      items.emplace_front(t);
      t->reparent(*this);
      handle_add_front(*t);
      return *t;
    }
    template <typename T,
              typename = typename std::enable_if<std::is_base_of<item_type, T>::value>::type>
    T& emplace_unique(std::unique_ptr<T>&& ptr)
    {
      T *t = ptr.get();
      items.push_back(std::move(ptr));
      t->reparent(*this);
      handle_add_back(*t);
      return *t;
    }

    void remove(item_type& item)
    {
      items.remove_if([&item](const std::unique_ptr<item_type>& p)
                      { return &item == p.get(); });      
    }
  };
  typedef basic_container<widget> container;
}

#endif /* COX_CONTAINER_HH__ */
