#ifndef COX_KIT_HH__
#define COX_KIT_HH__

#include <memory>
#include "window.hh"
#include "widget.hh"

namespace cox {
  class kit {
    class impl; impl *pimpl;
    void handle_unique(widget *ptr);
    void show_window(window *ptr);
  public:
    kit(int& argc, char **& argv);
    ~kit();
    void run();
    void run(window&& w) { show(std::move(w)); run(); }
    void show(window&& w) { show_window(&handle(std::move(w))); }
    void show(window& w) { show_window(&w); }
    template<typename T>
    T& handle(T&& w) { T*ptr=new T(std::move(w)); handle_unique(ptr); return *ptr; }
    void quit();
  };
}

#endif /* COX_KIT_HH__ */
