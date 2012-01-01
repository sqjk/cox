#include <cox/dialogs.hh>

#include <QtGui>

namespace cox {
  path select_save_file()
  {
    return path(QFileDialog::getSaveFileName().toStdString());
  }
  path select_save_folder()
  {
    QFileDialog dialog;
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    return path(dialog.getOpenFileName().toStdString());
  }
  path select_open_file()
  {
    return path(QFileDialog::getOpenFileName().toStdString());
  }
  path select_open_folder()
  {
    return path(QFileDialog::getExistingDirectory().toStdString());
  }

  path select_save_file(const window& w)
  {
    return path(QFileDialog::getSaveFileName(native::get_object_of<QWidget>(w)).toStdString());
  }
  path select_save_folder(const window& w)
  {
    QFileDialog dialog;
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    return path(dialog.getOpenFileName(native::get_object_of<QWidget>(w)).toStdString());
  }
  path select_open_file(const window& w)
  {
    return path(QFileDialog::getOpenFileName(native::get_object_of<QWidget>(w)).toStdString());
  }
  path select_open_folder(const window& w)
  {
    return path(QFileDialog::getExistingDirectory(native::get_object_of<QWidget>(w)).toStdString());
  }

  void message(const char *text)
  {
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
  }
  void message(const char *text, const window& parent)
  {
    QMessageBox msgBox(native::get_object_of<QWidget>(parent));
    msgBox.setText(text);
    msgBox.exec();
  }
}
