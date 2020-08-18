#include <iostream>
#include <okami-utils/akt.h>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cerr << "Didn't supply .AKT file.";
    return -1;
  }

  OKAMI_UTILS::AKT akt(argv[1]);

  return 0;
}