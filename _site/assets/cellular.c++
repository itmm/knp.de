#include <iostream>
#include <vector>
#include <getopt.h>

#pragma mark - configuration

static int automata_nr = 30;
static int display_rows = 60;
static int display_column_begin = -39;
static int display_column_end = 40;

void handle_options(int argc, char **argv) {
    static struct option long_options[] = {
        { "nr", required_argument, 0, 'n' },
        { "rows", required_argument, 0, 'r' },
        { "begin", required_argument, 0, 'b' },
        { "end", required_argument, 0, 'e' },
        { 0 }
    };

    for (;;) {
        int choice = getopt_long(
            argc, argv,
            "n:r:b:e:",
            long_options, NULL
        );

        if (choice == -1) break;
        switch (choice) {
            case 'n':
                automata_nr = atoi(optarg);
                break;
            case 'r':
                display_rows = atoi(optarg);
                break;
            case 'b':
                display_column_begin = atoi(optarg);
                break;
            case 'e':
                display_column_end = atoi(optarg);
        }
    }
}

#pragma mark - handling bits in one line

class Line {
public:
    Line(bool placeholder, int begin);

    bool placeholder() const { return _placeholder; }
    int begin() const { return _begin; }
    int end() const { return _begin + _data.size(); }
    int default_mask() const;

    bool operator[](int index) const;
    void push_back(bool value);

    std::ostream &print(std::ostream &out) const;

private:
    Line();
  
    bool _placeholder;
    int _begin;
    std::vector<bool> _data;
};

inline Line::Line(bool placeholder, int begin):
    _placeholder(placeholder), _begin(begin), _data()
{}

inline int Line::default_mask() const {
    return _placeholder ? 0b111 : 0b000;
}

inline bool Line::operator[](int i) const {
    if (i < begin() || i >= end()) return _placeholder;
    return _data[i - begin()];
}

inline void Line::push_back(bool value) {
    _data.push_back(value);
}

std::ostream &Line::print(std::ostream &out) const {
    for (
        int i = display_column_begin;
        i < display_column_end;
        ++i
    ) {
        out << (*this)[i];
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const Line &line) {
    return line.print(out);
}

#pragma mark - run simulation

int main(int argc, char **argv) {
    handle_options(argc, argv);
  
    const bool resulting_bit[8] = {
        (automata_nr & 0b00000001), (automata_nr & 0b00000010),
        (automata_nr & 0b00000100), (automata_nr & 0b00001000),
        (automata_nr & 0b00010000), (automata_nr & 0b00100000),
        (automata_nr & 0b01000000), (automata_nr & 0b10000000)
    };

    Line previous(false, 0);
    previous.push_back(true);
    std::cout << previous << std::endl;

    for (int i = 0; i < display_rows; ++i) {
        Line current(
            resulting_bit[previous.default_mask()],
            previous.begin() - 1
        );
        
        int gliding = previous.default_mask();
        for (
            int j = previous.begin();
            j <= previous.end() + 1;
            ++j
        ) {
            gliding = ((gliding << 1) + previous[j]) & 0b111;
            current.push_back(resulting_bit[gliding]);
        }

        std::cout << current << std::endl;
        previous = current;
    }
}
