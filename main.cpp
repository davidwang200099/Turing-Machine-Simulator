#include <iostream>
#include "TM.h"
using namespace std;
int main() {
    TM m;
    m.setCode("01010001010011001010001010011000100100100100");
    //m.showCode();
    m.analyseCode();
    m.show_graph();
    m.acceptInput();
    m.run();
    return 0;
}