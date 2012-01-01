#include <cox/kit.hh>

#include <memory>
#include <list>

#include <QtGui>

namespace cox {
  struct kit::impl {
    impl(int& argc, char **& argv) : app(argc, argv), children() {}
    QApplication app;
    std::list<std::unique_ptr<widget> > children;
  };

  kit::kit(int& argc, char **& argv)
  {
    pimpl = new impl(argc, argv);
  }

  kit::~kit()
  {
    delete pimpl;
  }

  void kit::run()
  {
    pimpl->app.exec();
  }

  void kit::handle_unique(widget *ptr)
  {
    pimpl->children.emplace_back(ptr);
  }

  void kit::show_window(window *ptr)
  {
    ptr->show();
  }

  void kit::quit()
  {
    pimpl->app.quit();
  }
}
