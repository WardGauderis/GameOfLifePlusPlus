#include<iostream>
#include<vector>
#include <algorithm>
#include <sstream>
#include "DFA.h"

using namespace std;

int main() {
    DFAPlusPlus me("input.json");
    me.print("output1");
    system("xdg-open output1.png&");
    cout << "input: 001\toutput:" << me("001") << "\n";
    cout << "input: 0\toutput:" << me("0") << "\n";
    cout << "input: 1010\toutput:" << me("1010") << "\n";
    cout << "input: 10\toutput:" << me("10") << "\n";
    getchar();
    cout << "TFA++\n" << "----------------------------------------------------------\n\n";
    me.TFAPlusPlus();
    me.print("output2");
    system("xdg-open output2.png&");
    cout << "input: 001\toutput:" << me("001") << "\n";
    cout << "input: 0\toutput:" << me("0") << "\n";
    cout << "input: 1010\toutput:" << me("1010") << "\n";
    cout << "input: 10\toutput:" << me("10") << "\n";
    for (const auto &state: DFAPlusPlus::states) delete state;

}