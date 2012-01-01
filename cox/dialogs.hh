#ifndef COX_DIALOGS_HH__
#define COX_DIALOGS_HH__

#include <string>
#include "window.hh"

namespace cox {
#ifdef COX_BOOST_FILESYSTEM
  typedef boost::filesystem::path path;
#else
  typedef std::string path;
#endif  

  path select_save_file();
  path select_save_file(const window& parent);
  path select_save_folder();
  path select_save_folder(const window& parent);
  path select_open_file();
  path select_open_file(const window& parent);
  path select_open_folder();
  path select_open_folder(const window& parent);

  void message(const char *text);
  inline void message(const std::string& s) { message(s.c_str()); }
  void message(const char *text, const window& parent);
  inline void message(const std::string& s, const window& parent) { message(s.c_str(), parent); }
}

#endif /* COX_DIALOGS_HH__ */
