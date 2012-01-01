#include <type_traits>
#include <functional>
#include <iostream>

#include <cox/kit.hh>
#include <cox/window.hh>
#include <cox/button.hh>
#include <cox/textbox.hh>
#include <cox/box.hh>
#include <cox/label.hh>
#include <cox/model.hh>
#include <cox/dialogs.hh>
#include <cox/composite.hh>

const char *example1_desc = "Hello World";
cox::window example1()
{
  cox::window mainwin;
  mainwin.retitle("Hello World");
  mainwin.resize(cox::px(300), cox::px(200));
    
  auto& b = mainwin.emplace<cox::button>("Exit");
  b.on_click().connect([](cox::button& b) { b.parent_window().close(); });

  return std::move(mainwin);
}

const char *example2_desc = "Textbox and RAII";
cox::window example2()
{
  auto create_textbox = []() -> cox::textbox
    { cox::textbox t; t.retext("input"); return std::move(t); };
  
  cox::window w(cox::property::set<cox::property::title>("Write something"),
                cox::property::set<cox::property::size >(cox::px(300), cox::px(200))
               );
  auto& t = w.emplace<cox::textbox>(create_textbox());
  // As long as w exists, there exists t
  w.on_close().connect([&t](cox::window&){
      std::cout << "The input was: `" << t.text() << "'\n";});
  
  return std::move(w);
}

const char *example3_desc = "Layout test";
cox::window example3()
{
  cox::window w(cox::property::set<cox::property::title>("Layout test"),
                cox::property::set<cox::property::size >(cox::px(300), cox::px(200))
               );
  auto& l1 =  w.emplace<cox::vertical_box>();
  auto& l2 = l1.emplace<cox::horizontal_box>();
  l2.emplace<cox::textbox>("one");
  l2.emplace<cox::textbox>("two");
  l1.emplace<cox::button>("enter").on_click().connect(
    [](cox::widget& b) { b.parent_window().close(); }
  );
  return std::move(w);
}

const char *example4_desc = "property-cascades test";
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

const char *example5_desc = "nested boxes";
cox::window example5()
{
  cox::window w("My Window");
  auto& mainpanel = w.emplace<cox::vertical_box>();
  cox::modify(mainpanel, cox::property::emplace<cox::label>("huhyo"));
  auto& panel1 = mainpanel.emplace<cox::horizontal_box>();
  panel1.emplace<cox::label>("Some Text");
  panel1.emplace<cox::textbox>("Some Text");
  auto &b = mainpanel.emplace<cox::button>("One Button");
  b.on_click().connect([&](cox::widget& w) { w.parent_window().close();});
  return std::move(w);
}

const char *example6_desc = "model test";
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

const char *example7_desc = "list model test";
cox::window example7()
{
  cox::window tw;
  auto& cont = tw.emplace<cox::vertical_box>();
  auto create_button = [](){
    return cox::button(cox::property::connect<cox::property::click>(
                         [](cox::button& b){b.parent_window().close();}
                       ));
  };
  auto ms = cox::model<cox::property::text>(cont, create_button);
  for (int i=0; i<20; ++i)
    ms.push_back(std::string("hello ") + std::string(1, i+'0'));
  ms.at(10).set("13th button: ");
  std::cout << ms.at(13).get() << '\n';
  ms.remove(7);
  ms.release(); // otherwise ms would destruct cont's labels
  return std::move(tw);
}

const char *example8_desc = "polymorphic list model test";
cox::window example8()
{
  cox::window w;
  auto& cont = w.emplace<cox::vertical_box>();
  std::unique_ptr<cox::polymorphic_list_model<std::string> > m
    = cox::dynamic_list_model<cox::property::text, cox::button>(cont);
  for (auto str : { "huxux", "huhux", "huhuh", "xuhuh", "xuxuh" })
    m->push_back(str);
  m->release();
  return std::move(w);
}

class file_selector : public cox::composite_widget<cox::horizontal_box> {
  cox::property_model<cox::property::text, cox::textbox> filepath;
public:

  file_selector()
    : composite_widget(
        cox::property::emplace<cox::button>(
          cox::property::set<cox::property::text>("..."),
          cox::property::connect<cox::property::click>(
            [this](cox::button&){ filepath.set(cox::select_open_file()); }
          )
        )
      ),
      filepath(
        cox::model<cox::property::text>(
          content.emplace_front<cox::textbox>(
            cox::property::set<cox::property::text>("filepath"),
            cox::property::set<cox::property::editable>(false)
          )
        )
      )
  { }

  cox::path path() const { return cox::path(filepath.get()); }
  void repath(cox::path p) { filepath.set(p); }
};

const char *example9_desc = "custom widget test";
cox::window example9()
{
  cox::window w;
  w.emplace<file_selector>();
  return std::move(w);
}

int main(int argc, char *argv[])
{
  cox::kit app(argc, argv);
  cox::window w(example4());

  auto& box = w.emplace<cox::vertical_box>();

#define EMPLACE_EXAMPLE(nr) \
  box.emplace<cox::button>(example##nr##_desc).                         \
    on_click().connect([&](cox::button&){app.show(example##nr());})

  EMPLACE_EXAMPLE(1);
  EMPLACE_EXAMPLE(2);
  EMPLACE_EXAMPLE(3);
  EMPLACE_EXAMPLE(4);
  EMPLACE_EXAMPLE(5);
  EMPLACE_EXAMPLE(6);
  EMPLACE_EXAMPLE(7);
  EMPLACE_EXAMPLE(8);
  EMPLACE_EXAMPLE(9);
  box.emplace<cox::button>("exit").on_click().connect(
    [](cox::button& b) { b.parent_window().close(); }
  );

  app.run(std::move(w));
}
