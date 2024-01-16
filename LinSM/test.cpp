#include <iostream>

using namespace std;

extern "C" {
  #include "src/LinSM.c"
  #include "src/Det.c"
  #include "src/LinIf.c"
  #include "src/BswM_LinSM.c"
  #include "src/ComM.c"
}

int main (int argc, char **argv) {
    cout << "Hello World!" << endl;
}