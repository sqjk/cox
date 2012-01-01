#ifndef COX_PROPERTY_HH__
#define COX_PROPERTY_HH__

#include <vector>
#include <array>
#include <memory>
#include <string>

#include "coordinates.hh"

namespace cox {
  namespace property {
    template <typename T>
    struct property_trait;
    template <typename T>
    struct set;
    template <typename T>
    struct get;
    template <typename T>
    struct emplace;
    template <typename T>
    struct connect;

    struct title;
    template <> struct property_trait<title> { typedef std::string type; };
    template <> struct set<title> {
      set(const property_trait<title>::type& s) : s(s) {}
      property_trait<title>::type s;
      template <typename T> void on(T& w) const { w.retitle(s);}
    };
    template <> struct get<title> {
      template <typename T>
      static property_trait<title>::type on(T& w) { return w.title();}
    };

    struct size;
    template <> struct property_trait<size> { typedef rectangle type; };
    template <> struct set<size> {
      set(rectangle r) : dw(r.width()), dh(r.height()) {}
      set(distance w, distance h) : dw(w), dh(h) {}
      distance dw, dh;
      template <typename T> void on(T& w) const { w.resize(dw, dh); }
    };
    template <> struct get<size> {
      template <typename T>
      static property_trait<size>::type on(T& w) { return rectangle(w.width(), w.height()); }
    };

    struct text; template <>
    template <> struct property_trait<text> { typedef std::string type; };
    template <> struct set<text>{
      set(const property_trait<text>::type& s) : s(s) {}
      property_trait<text>::type s;
      template <typename T> void on(T& w) const { w.retext(s); }
    };
    template <> struct get<text> {
      template <typename T>
      static property_trait<text>::type on(T& w) { return w.text(); }
    };    

    struct enabled;
    template <> struct property_trait<enabled> { typedef bool type; };
    template <> struct set<enabled> {
      set(const property_trait<enabled>::type& s) : s(s) {}
      property_trait<enabled>::type s;
      template <typename T> void on(T& w) const { w.reenable(s);}
    };
    template <> struct get<enabled> {
      template <typename T>
      static property_trait<enabled>::type on(T& w) { return w.enabled();}
    };

    struct editable;
    template <> struct property_trait<editable> { typedef bool type; };
    template <> struct set<editable> {
      set(const property_trait<editable>::type& s) : s(s) {}
      property_trait<editable>::type s;
      template <typename T> void on(T& w) const { w.make_editable(s);}
    };
    template <> struct get<editable> {
      template <typename T>
      static property_trait<editable>::type on(T& w) { return w.editable();}
    };

    template <typename T>
    struct emplace {
      std::unique_ptr<T> value;
      template <typename... Args>
      emplace(Args&&... args) : value(new T(std::forward<Args>(args)...)) { }
      template <typename Container,
                typename = typename std::enable_if<std::is_base_of<typename Container::item_type,
                                                                   T>::value>::type>
      T& on(Container& c) {
        T* ret = value.get(); c.emplace_unique(std::move(value)); return *ret; }
    };

    template <typename T>
    struct remove {
      T& value;
      remove(T& value) : value(value) { }
      template <typename Container>
      void on(Container& c) { c.remove(value); }
    };
  }

  namespace helper {
    template <template <typename> class pred, typename...>
    struct query_all;
    template <template <typename> class pred>
    struct query_all<pred> : std::true_type {};
    template <template <typename> class pred, typename T, typename... Args>
    struct query_all<pred, T, Args...>
      : std::integral_constant<bool,
                               pred<T>::value &&
                               query_all<pred, Args...>::value> {};

    template <typename T>
    struct is_modify_parameter_helper : std::false_type {};
    template <typename T>
    struct is_modify_parameter_helper<property::set<T>     > : std::true_type {};
    template <typename T>
    struct is_modify_parameter_helper<property::emplace<T> > : std::true_type {};
    template <typename T>
    struct is_modify_parameter_helper<property::connect<T> > : std::true_type {};
    template <typename T>
    struct is_modify_parameter
      : is_modify_parameter_helper<typename std::remove_reference<T>::type> {};
  }

  template <typename... Args>
  struct are_modify_parameters : helper::query_all<helper::is_modify_parameter, Args...> {};

  template <typename... Args>
  struct enable_if_modify_parameters : std::enable_if<are_modify_parameters<Args...>::value> {};

  template <typename T>
  inline void modify(T&)
  {
  }
  template <typename T, typename Arg, typename... Args>
  inline void modify(T& me, property::set<Arg>&& value, Args&&... args)
  {
    value.on(me);
    modify(me, std::forward<Args>(args)...);
  }
  template <typename T, typename Arg, typename... Args>
  inline void modify(T& me, property::connect<Arg>&& value, Args&&... args)
  {
    value.on(me);
    modify(me, std::forward<Args>(args)...);
  }
  template <typename T, typename Arg, typename... Args>
  inline void modify(T& me, property::emplace<Arg>&& value, Args&&... args)
  {
    value.on(me);
    modify(me, std::forward<Args>(args)...);
  }
}
 
#endif /* COX_PROPERTY_HH__ */
