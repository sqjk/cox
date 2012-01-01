#ifndef COX_MODEL_HH__
#define COX_MODEL_HH__

#include <memory>
#include <vector>

#include "property.hh"

namespace cox {
  template <typename TProp, typename T>
  class property_model {
    T *target_;
  public:
    typedef typename property::property_trait<TProp>::type value_type;
    property_model(T& target) : target_(&target) {}
    template <typename Arg>
    void       set(const Arg& value) { modify(*target_, property::set<TProp>(value)); }
    value_type get() const { return property::get<TProp>::on(*target_); }

    T& target() { return *target_; }
  };

  template <typename TProp, typename TCont, typename TFunc>
  class emplace_model {
  public:
    typedef typename property::property_trait<TProp>::type value_type;
    typedef typename std::result_of<TFunc(/*value_type*/)>::type object_type;
    typedef property_model<TProp, object_type> model_type;
  private:
    TCont& target;
    std::vector<model_type> items;
    TFunc default_create;
  public:
    emplace_model(TCont& target, TFunc func) : target(target), items(), default_create(func) {}
    ~emplace_model() {while (!items.empty()) remove(items.size()-1); }
    template <typename Arg>
    void       push_back(const Arg& value)
    {
      items.emplace_back(property::emplace<object_type>(default_create()).on(target));
      items.back().set(value);
    }
    model_type at(size_t i) { return items[i]; }
    void remove(size_t i)
    {
      property::remove<object_type>(at(i).target()).on(target);
      items.erase(items.begin() + i);
    }
    void release() { items.clear(); }
  };

  template <typename TProp, typename T>
  static property_model<TProp, T> model(T& target) { return property_model<TProp, T>(target); }

  template <typename TProp, typename TCont, typename TFunc>
  static emplace_model<TProp, TCont, TFunc>
  model(TCont& target, TFunc f)
  {
    return emplace_model<TProp, TCont, TFunc>(target, f);
  }

  template <typename T>
  class polymorphic_model {
  public:
    typedef T value_type;
    typedef std::unique_ptr<polymorphic_model> handle;

    virtual void set(const T& val) =0;
    virtual  T   get() const =0;
  };

  template <typename T>
  class polymorphic_list_model {
  public:
    typedef polymorphic_model<T> value_type;
    typedef std::unique_ptr<polymorphic_list_model> handle;

    virtual ~polymorphic_list_model() {}
    virtual void push_back(const T& val) =0;
    virtual value_type& at(size_t i) =0;
    virtual void remove(size_t i) =0;
    virtual void release() =0;
  };

  template <typename TProp, typename TTarget,
            typename T=typename property::property_trait<TProp>::type>
  class polymorphic_property_model : public polymorphic_model<T> {
  public:
    property_model<TProp, TTarget> prop;
    polymorphic_property_model(TTarget& target) : prop(target) {}
    void set(const T& val) { prop.set(val); }
    T    get() const { return prop.get(); }

    TTarget& target() { return prop.target(); }
  };

  template <typename TProp, typename TCont, typename TFunc,
            typename T=typename property::property_trait<TProp>::type>
  class polymorphic_emplace_model : public polymorphic_list_model<T> {
  public:
    typedef typename std::result_of<TFunc()>::type object_type;
    typedef polymorphic_property_model<TProp, object_type> concrete_value_type;
    typedef typename polymorphic_list_model<T>::value_type value_type;
  private:
    TCont& target;
    std::vector<concrete_value_type> items;
    TFunc default_create;
  public:
    polymorphic_emplace_model(TCont& target, TFunc func) : target(target), items(), default_create(func) {}
    ~polymorphic_emplace_model() {while (!items.empty()) remove(items.size()-1); }
    void push_back(const T& value)
    {
      items.emplace_back(property::emplace<object_type>(default_create()).on(target));
      items.back().set(value);
    }
    value_type& at(size_t i) { return items[i]; }
    void remove(size_t i)
    {
      property::remove<object_type>(items[i].target()).on(target);
      items.erase(items.begin() + i);
    }
    void release() { items.clear(); }
  };

  template <typename TProp, typename TTarget>
  typename polymorphic_model<typename property::property_trait<TProp>::type>::handle
  create(TTarget& t)
  {
    typedef polymorphic_model<typename property::property_trait<TProp>::type> polymod;
    typedef polymorphic_property_model<TProp, TTarget> propmod;
    return std::unique_ptr<polymod>(new propmod(t));
  }

  template <typename TProp, typename T>
  std::unique_ptr<polymorphic_model<typename property::property_trait<TProp>::type> >
  dynamic_model(T& target) {
    typedef polymorphic_model<typename property::property_trait<TProp>::type> value;
    return  create<TProp, T>(target);
  }


  template <typename TProp, typename TCont, typename TFunc>
  typename polymorphic_list_model<typename property::property_trait<TProp>::type>::handle
  dynamic_list_model(TCont& t, TFunc func) {
    typedef typename property::property_trait<TProp>::type polymod;
    typedef typename polymorphic_list_model<polymod>::handle return_type;
     polymorphic_list_model<polymod> *ptr =
       new polymorphic_emplace_model<TProp, TCont, TFunc>(t, func);
     return std::unique_ptr<polymorphic_list_model<polymod> >(ptr);
  }

  template <typename TProp, typename TObj, typename TCont>
  typename polymorphic_list_model<typename property::property_trait<TProp>::type>::handle
  dynamic_list_model(TCont& t) {
    auto default_constructor = [](){return TObj();};
    return dynamic_list_model<TProp, TCont, decltype(default_constructor)>(t, default_constructor);
  }
}

#endif /* COX_MODEL_HH__ */

