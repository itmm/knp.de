#include <iostream>
#include <vector>
#include <getopt.h>

static int NR = 30;
static int BEGIN = -39;
static int END = 40;
static int ROWS = 60;

class Line {
public:
  Line(bool empty): _empty(empty), _begin(0), _data() {}

  bool empty() const { return _empty; }
  int begin() const { return _begin; }
  int end() const { return _begin + _data.size(); }

  int get(int i) const;
  void set(int i, bool v);

  void print() const;

private:
  bool _empty;
  int _begin;
  std::vector<bool> _data;
};

inline int Line::get(int i) const {
  if (i < begin() || i >= end()) return _empty;
  return _data[i - begin()];
}

void Line::set(int i, bool v) {
  if (v == _empty) { return; }
  if (_data.size()) {
    while (end() < i) {
      _data.push_back(_empty);
    }
  }
  else _begin = i; // first set defines leftmost
  _data.push_back(v);
}

void Line::print() const {
  for (int i = BEGIN; i < END; ++i) {
    std::cout << (get(i) ? "1": "0");
  }
  std::cout << std::endl;
}

int main(int argc, char **argv) {
  for (;;) {
    static struct option long_options[] = {
      {"nr", required_argument, 0, 'n'},
      {"rows", required_argument, 0, 'r'},
      {"begin", required_argument, 0, 'b'},
      {"end", required_argument, 0, 'e'},
      {0}
    };

    int option_index = 0;
    int choice = getopt_long(
      argc, argv, "n:r:b:e:", long_options, &option_index
    );

    if (choice == -1) break;
    switch (choice) {
      case 'n': NR = atoi(optarg); break;
      case 'r': ROWS = atoi(optarg); break;
      case 'b': BEGIN = atoi(optarg); break;
      case 'e': END = atoi(optarg); break;
      default: break;
    }
  }

  const bool BITS[8] = 
  {
    (NR & 0x01) != 0, (NR & 0x02) != 0, 
    (NR & 0x04) != 0, (NR & 0x08) != 0, 
    (NR & 0x10) != 0, (NR & 0x20) != 0,
    (NR & 0x40) != 0, (NR & 0x80) != 0
  };

  Line o(false); o.set(0, true); o.print();

  for (int i = 0; i < ROWS; ++i) {
    Line n(BITS[o.empty()? 7: 0]);
    int c = o.empty()? 7: 0;
    for (int j = o.begin() - 1; j <= o.end(); ++j) {
      c = (c * 2 + (o.get(j + 1)? 1: 0)) & 0x07;
      n.set(j, BITS[c]);
    }

    n.print();
    o = n;
  }

  return 0;
}
