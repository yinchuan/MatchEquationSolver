#ifndef MATCHEQUATION_EQUATION_H
#define MATCHEQUATION_EQUATION_H

#include <cassert>
#include <map>

using namespace std;

class Equation {
public:
    Equation(int first, char op, int second, int third) {
        equation = encode(first, op, second, third);
    }

    void print() const {
        int first, second, third;
        char op;
        if (!decode(first, op, second, third)) {
            cout << "failed to decode\n";
            exit(1);
        }
        cout << first << op << second << "=" << third << endl;
    }

    vector<Equation> solve() {
        vector<Equation> ret;
        for (int i = 0; i < 23; ++i) {
            if (!isSet(i)) {
                continue;
            }
            pick(i);
            for (int j = 0; j < 23; ++j) {
                if (j == i || isSet(j)) {
                    continue;
                }
                drop(j);
                if (isValid()) {
                    ret.push_back(*this); // save a solution
                }
                pick(j);
            }
            drop(i);
        }

        return ret;
    };

    bool isValid() const {
        int first, second, third;
        char op;
        if (!decode(first, op, second, third)) {
            return false;
        }

        if (op == '+') {
            return first + second == third;
        }
        if (op == '-') {
            return first - second == third;
        }

        assert(true); // shouldn't be here
        return false;
    }

private:

    int equation; // an equation is 3 digits * 7bits + 2 bits for op = 21 bits

    void pick(int position) { // set to 0
        equation &= ~(1 << position);
    }

    void drop(int position) {
        equation |= (1 << position);
    }

    bool isSet(int position) const {
        return (equation & (1 << position)) != 0;
    }

    bool decode(int &first, char &op, int &second, int &third) const {
        map<int, int> decoder;
        decoder[0b01111011] = 0;
        decoder[0b01001000] = 1;
        decoder[0b00111101] = 2;
        decoder[0b01101101] = 3;
        decoder[0b01001110] = 4;
        decoder[0b01100111] = 5;
        decoder[0b01110111] = 6;
        decoder[0b01001001] = 7;
        decoder[0b01111111] = 8;
        decoder[0b01001111] = 9;
        decoder[0b00000011] = '+';
        decoder[0b00000001] = '-';

        auto mask = 0b01111111; // extract least 7 bits
        auto codedFirst = (equation >> 16) & mask;
        auto codedOp = (equation >> 14) & 0b00000011;
        auto codedSecond = (equation >> 7) & mask;
        auto codedThird = equation & mask;

        if (decoder.find(codedFirst) == decoder.end() ||
            decoder.find(codedOp) == decoder.end() ||
            decoder.find(codedSecond) == decoder.end() ||
            decoder.find(codedThird) == decoder.end()) {
            return false;
        }

        first = decoder[codedFirst];
        op = decoder[codedOp];
        second = decoder[codedSecond];
        third = decoder[codedThird];

        return true;
    }

    int encode(int first, char op, int second, int third) const {
        assert(first >= 0 && first <= 9);
        assert(second >= 0 && second <= 9);
        assert(third >= 0 && third <= 9);
        assert(op == '+' || op == '-');

        map<int, int> encoder;
        encoder[0] = 0b01111011;
        encoder[1] = 0b01001000;
        encoder[2] = 0b00111101;
        encoder[3] = 0b01101101;
        encoder[4] = 0b01001110;
        encoder[5] = 0b01100111;
        encoder[6] = 0b01110111;
        encoder[7] = 0b01001001;
        encoder[8] = 0b01111111;
        encoder[9] = 0b01001111;
        encoder['+'] = 0b00000011;
        encoder['-'] = 0b00000001;

        if (encoder.find(first) == encoder.end() ||
            encoder.find(op) == encoder.end() ||
            encoder.find(second) == encoder.end() ||
            encoder.find(third) == encoder.end()) {
            return -1;
        }

        return 0 | (encoder[first] << 16) | (encoder[op] << 14) | (encoder[second] << 7) | encoder[third];
    }
};

#endif //MATCHEQUATION_EQUATION_H
