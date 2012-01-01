#include <QtGui>

namespace cox {
  struct layout_medium : public QWidget {
    Q_OBJECT
    
  public:
    layout_medium() : have_to_delete(true) {}
    ~layout_medium() { have_to_delete = false; }
    bool have_to_delete;
  };
}
