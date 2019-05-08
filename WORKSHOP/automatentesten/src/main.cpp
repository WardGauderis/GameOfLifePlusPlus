#include <QApplication>
#include <iostream>
#include "./input-output/parser.h"
#include "./automata/pda.h"
#include "./automata/tm.h"
#include "./automata/dfa.h"
#include "./automata/nfa.h"



void testStringPDA(const PDA* testPDA, std::string test)
{
    if ((*testPDA)(test))
    {
        std::cout << test << " was accepted by PDA" << std::endl;
    }
    else std::cout << test << " was not accepted by PDA" << std::endl;
}

void testStringTM(const TM* testTM, std::string test)
{
    if ((*testTM)(test))
    {
        std::cout << test << " was accepted by Turing Machine" << std::endl;
    }
    else std::cout << test << " was not accepted by Turing Machine" << std::endl;
}

//void testStringDFA(const DFA* testDFA, std::string test)
//{
//    if ((*testDFA)(test))
//    {
//        std::cout << test << " was accepted by DFA" << std::endl;
//    }
//    else std::cout << test << " was not accepted by DFA" << std::endl;
//}

//void testStringENFA(const NFA* testENFA, std::string test)
//{
//    if ((*testENFA)(test))
//    {
//        std::cout << test << " was accepted by ENFA" << std::endl;
//    }
//    else std::cout << test << " was not accepted by ENFA" << std::endl;
//}
int main()
{
    std::map<std::string, char> testAlphabet;

    testAlphabet["alive"] = '1';
    testAlphabet["dead"] = '0';

    const PDA * testPDA = Parser::parsePDA("./input/pda.json", testAlphabet);
    const TM * testTM = Parser::parseTM("./input/tm.json", testAlphabet);

//    const NFA *testENFA = Parser::parseNFA("./input/enfa.json", testAlphabet);
//    const DFA * testDFA = Parser::parseDFA("./input/dfa.json", testAlphabet);

//    std::cout << "-----------------------------------------------------------------------------------" << "\n\n";
//
//    testStringDFA(testDFA, "11100000");
//    testStringDFA(testDFA, "01010101");
//    testStringDFA(testDFA, "00010011");

//    std::cout << "-----------------------------------------------------------------------------------" << "\n\n";
//
//    testStringENFA(testENFA, "11100000");
//    testStringENFA(testENFA, "01010101");
//    testStringENFA(testENFA, "00010011");
    
    std::cout << "-----------------------------------------------------------------------------------" << "\n\n";

    testStringPDA(testPDA, "11110000");
    testStringPDA(testPDA, "10101010");
    testStringPDA(testPDA, "00001111");

    std::cout << "-----------------------------------------------------------------------------------" << "\n\n";

    testStringTM(testTM, "00001111");
    testStringTM(testTM, "10101010");
    testStringTM(testTM, "11110000");


}