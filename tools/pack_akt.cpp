#include <iostream>
#include <okami-utils/akt.h>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cerr << "Didn't supply args: dir_path target_akt.";
    return -1;
  }

  OKAMI_UTILS::AKT akt;
  akt.pack_dir(argv[1], argv[2]);

  return 0;
}