## cox -- A lightweight GUI Toolkit for C++11

cox is a GUI Toolkit written in modern C++.  It currently wrapps parts of Qt and Gtk.

## Basics

cox's widgets support RAII and are noncopyable but movable:

```cpp
cox::window example1()
{
  cox::window mainwin;
  mainwin.retitle("Hello world");
  mainwin.resize(cox::px(300), cox::px(200));
    
  auto& b = mainwin.emplace<cox::button>("Quit"); // only a reference, the
                                                  // real owner is mainwin.

  // events are supported using Boost.Signal
  b.on_click().connect([](cox::button& b) { b.parent_window().close(); });

  return std::move(mainwin); // movable
}
```
Why is there a need for smartpointers in gui toolkits?

## Features
```cpp
cox::window example4()
{
  using namespace cox;

  return window(
    property::set<property::title>("property-cascades test"),
    property::set<property::size >(px(300), px(200)),
    property::emplace<vertical_box>(
      property::emplace<label> ("property-cascades test"),
      property::emplace<button>("button 1"),
      property::emplace<button>("button 2"),
      property::emplace<button>("button 3"),
      property::emplace<button>(
        property::set<property::text>("close"),
        property::connect<property::click>([&](widget& b) { b.parent_window().close(); })
      )
    )
  );
}
```
Simple and expressive -- through the property-design and variadic templates.

Using the properties the other way, we can create a property binder:

```cpp
cox::window example6()
{
  cox::window w("My Window");
  auto& l = w.emplace<cox::label>();
  auto m = cox::model<cox::property::text>(l);
  m.set("label text was set");
  std::cout << m.get() << std::endl;
  m.set("... and was set again");
  std::cout << m.get() << std::endl;
  return std::move(w);
}
```

This even goes so far, that we can bind a list of strings to a list of labels inside
a vertical box ... see our [main.cc](https://github.com/sqjk/cox/blob/master/main.cc)
for more details and see
[tictactoe.cc](https://github.com/sqjk/cox/blob/master/tictactoe.cc)
for a real word example.

## License

Copyright 2011 by me

Liscensed under the [LGPL](http://www.gnu.org/licenses/lgpl.html).
