namespace Grid {
  struct Point {
    int x, y;
  };

  struct Neighbors {
    struct Point v[8] = {Point{-1, -1}};
  };

  int GetIndex(struct Point pt) {
    int v = pt.x + LEDS_PER_ROW*pt.y;

    return v;
  }

  struct Point GetPoint(int index) {
    Point out;

    if((index>N_CELLS) || (index<0)) {
      out.x = -1;
      out.y = -1;

      return out;
    }

    out.x = index % LEDS_PER_ROW;
    out.y = index / LEDS_PER_ROW;

    return out;
  }

  struct Neighbors GetNeighbors(struct Point pt) {
    struct Neighbors out;

    int cnt = 0;
    for(int i=-1; i<2; i++) {
      for(int j=-1; j<2; j++){
        if((i==0) && (j==0)){
          continue;
        }

        out.v[cnt] = GetPoint(GetIndex(Point{(pt.x+i), (pt.y+j)}));

        cnt++;
      }
    }

    return out;
  }

  int InfiniteGetIndex(struct Point pt) {
    int v = pt.x + LEDS_PER_ROW*pt.y;

    return (v+N_CELLS)%N_CELLS;
  }

  struct Point InfiniteGetPoint(int index) {
    Point out;
    index = index%N_CELLS;

    out.x = index % LEDS_PER_ROW;
    out.y = index / LEDS_PER_ROW;

    return out;
  }

  struct Neighbors InfiniteGetNeighbors(struct Point pt) {
    struct Neighbors out;

    int cnt = 0;
    for(int i=-1; i<2; i++) {
      for(int j=-1; j<2; j++){
        if((i==0) && (j==0)){
          continue;
        }

        out.v[cnt] = InfiniteGetPoint(InfiniteGetIndex(Point{(pt.x+i), (pt.y+j)}));

        cnt++;
      }
    }

    return out;
  }

  bool IsBottomRow(struct Point pt) {
    return pt.y == (N_CELLS/LEDS_PER_ROW-1);
  }
}
