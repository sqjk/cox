#include <gtk/gtk.h>

#include <cox/kit.hh>

namespace cox {
  struct kit::impl {
    impl() : children(), instances(0), running(false) {}
    std::list<std::unique_ptr<widget> > children;
    int instances;
    bool running;
  };

  kit::kit(int& argc, char **& argv)
    : pimpl(new impl())
  {
    gtk_init(&argc, &argv);
  }

  kit::~kit()
  {
    quit();
    delete pimpl;
  }

  void kit::run()
  {
    pimpl->running = true;
    gtk_main();
  }

  void kit::handle_unique(widget *ptr)
  {
    pimpl->children.emplace_back(ptr);
  }

  void kit::show_window(window *ptr)
  {
    ++pimpl->instances;
    ptr->on_close().connect([this](window& w){
        pimpl->children.remove_if([&w](const std::unique_ptr<widget>& p)
                                  { return &w == p.get(); });
        if (--pimpl->instances == 0)
          quit();
      });
    ptr->show();
  }

  void kit::quit()
  {
    if (pimpl->running) {
      gtk_main_quit();
      pimpl->running = false;
    }
  }
}
