//============================================================================
// @name        : main.cpp
// @author      : Thomas Dooms
// @date        : 3/28/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================
#include "input-output/parser.h"

int main(int argc, char *argv[]) {
    std::cout
            << "--------------------------------------------\neNFA voorbeeldbestand\n--------------------------------------------\n";
    const DFA *x = reinterpret_cast<const DFA *>(Parser::parseAutomaton("input/enfa.json", {{"m", 'm'},
                                                                                            {"a", 'a'}}));
    std::cout << "INPUT mamma: " << (x->operator()("mamma") ? "ACCEPT\n" : "REJECT\n");
    std::cout << "INPUT ma: " << (x->operator()("ma") ? "ACCEPT\n" : "REJECT\n");
    std::cout << "INPUT mammm: " << (x->operator()("mammm") ? "ACCEPT\n" : "REJECT\n");
    std::cout << "INPUT mmaamma: " << (x->operator()("mmaamma") ? "ACCEPT\n" : "REJECT\n");
    x->dot("output1.png");
    system("xdg-open output1.png");
    std::cout
            << "--------------------------------------------\neNFA voor (0+1)*10\n--------------------------------------------\n";
    const DFA *y = reinterpret_cast<const DFA *>(Parser::parseAutomaton("input/enfa1.json", {{"0", '0'},
                                                                                             {"1", '1'}}));
    std::cout << "INPUT 10101010101000010: " << (y->operator()("10101010101000010") ? "ACCEPT\n" : "REJECT\n");
    std::cout << "INPUT 0101010101: " << (y->operator()("0101010101") ? "ACCEPT\n" : "REJECT\n");
    std::cout << "INPUT 0: " << (y->operator()("0") ? "ACCEPT\n" : "REJECT\n");
    std::cout << "INPUT 1: " << (y->operator()("1") ? "ACCEPT\n" : "REJECT\n");
    y->dot("output2.png");
    system("xdg-open output2.png&");
}