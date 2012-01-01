#ifndef COX_COORDINATES_HH__
#define COX_COORDINATES_HH__

namespace cox {
  struct pixel_distance {
    explicit pixel_distance(int pixels) : pixels(pixels) {}
  private:
    friend class distance;
    int pixels;
  };

  struct distance {
    distance(pixel_distance pd) : value_(pd.pixels) {}
    operator int  () const { return value_; }
  private:
    int value_;
  };

  struct rectangle {
    rectangle(distance w, distance h) : w(w), h(h) {}
    distance width() const { return w; }
    distance height() const { return w; }
  private:
    distance w, h;
  };
  
  inline distance /*operator "" */px(unsigned int d) { return pixel_distance(d); }
}


#endif /* COX_COORDINATES_HH__ */
