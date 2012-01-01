#include <cox/composite.hh>
#include <cox/window.hh>
#include <cox/kit.hh>
#include <cox/table.hh>
#include <cox/model.hh>
#include <cox/button.hh>
#include <cox/box.hh>
#include <cox/dialogs.hh>
#include <vector>

class ttt_field : public cox::composite_widget<cox::table> {
public:
  typedef boost::signal<void(ttt_field&, const std::string&)>
          game_end_signal_type;
private:
  std::vector<cox::property_model<cox::property::text, cox::button> > field;
  int round;
  game_end_signal_type on_game_end_;

  void check();
public:
  game_end_signal_type& on_game_end() { return on_game_end_; }
  const game_end_signal_type& on_game_end() const { return on_game_end_; }
  
  template <typename... Args,
            typename = typename cox::enable_if_modify_parameters<Args...>::type>
  ttt_field(Args&&... args)
    : round(0)
  {
    field.reserve(9);    
    for (int y=0; y<3; ++y) {
      for (int x=0; x<3; ++x) {
        field.push_back(
          cox::model<cox::property::text>(
            content.at(y, x).emplace<cox::button>(
              cox::property::set<cox::property::text>(" "),
              cox::property::connect<cox::property::click>(
                [this](cox::button& b) { b.retext((++this->round %2)?"X":"O");
                                         b.disable(); check(); }
              )
            )
          )
        );
      }
    }
    modify(*this, std::forward<Args>(args)...);
  }
};

namespace cox {
  namespace property {
    struct game_end;
    template <> struct connect<game_end> {
      connect(ttt_field::game_end_signal_type::slot_type f) : f(f) {}
      ttt_field::game_end_signal_type::slot_type f;
      template <typename T> void on(T& w) const { w.on_game_end().connect(f); }
    };
  }
}

class main_window : public cox::window {
public:
  main_window()
    : window(
        cox::property::emplace<cox::vertical_box>(
          cox::property::emplace<ttt_field>(
            cox::property::connect<cox::property::game_end>(
              [](ttt_field& f, const std::string& text)
              { cox::message(text); f.disable(); }
            )
          ),
          cox::property::emplace<cox::button>(
            cox::property::set<cox::property::text>("Quit"),
            cox::property::connect<cox::property::click>(
              [](widget& b) { b.parent_window().close(); }
            )
          )
        )
      )
  { }
};


void ttt_field::check()
{
  // This algorithm is stolen from the GTK+ Tutorial
  static int rwins[8][3] = { { 0, 0, 0 }, { 1, 1, 1 }, { 2, 2, 2 },
                             { 0, 1, 2 }, { 0, 1, 2 }, { 0, 1, 2 },
                             { 0, 1, 2 }, { 0, 1, 2 } };
  static int cwins[8][3] = { { 0, 1, 2 }, { 0, 1, 2 }, { 0, 1, 2 },
                             { 0, 0, 0 }, { 1, 1, 1 }, { 2, 2, 2 },
                             { 0, 1, 2 }, { 2, 1, 0 } };

  for (const char* id : { "X", "O" }) {
    for (int k=0; k<8; ++k) {
      bool won = true;
      for (int i=0; won && i<3; ++i)
        if (field[3*rwins[k][i] + cwins[k][i]].get() != id)
          won = false;
      if (won) {
        on_game_end_(*this, std::string("The winner is that with the ") + id + ".");
        return;
      }
    }
  }
  if (round == 9)
    on_game_end_(*this, "The game ends drawn.");
}

int main(int argc, char *argv[])
{
  cox::kit app(argc, argv);
  app.run(main_window());
}
