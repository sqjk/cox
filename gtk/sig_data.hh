#ifndef COX_SIG_DATA_HH__
#define COX_SIG_DATA_HH__

namespace cox {
  namespace internal {
    template <typename T>
    struct sig_data {
      typedef typename T::signal_type signal_type;
      T *owner;
      signal_type *sig;
      void set(T *o, signal_type *s)
      {
        owner = o;
        sig = s;
      }
    };
    
#define COX_INSTANTIZE_CALL_FUNCS(type)                         \
    do {                                                        \
      auto tmp___ = &internal::call_funcs<type>;                \
    } while(0)

    template <typename T>
    void call_funcs(sig_data<T> *dat)
    {
      (*static_cast<typename sig_data<T>::signal_type*>(dat->sig))(*dat->owner);
    }
  }
}

#endif /* COX_SIG_DATA_HH__ */
