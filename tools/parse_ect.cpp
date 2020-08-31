#include <okami-utils/ect.h>

int main(int argc, char** argv) {
  OKAMI_UTILS::ECT ect;
  ect.parse_file(argv[1]);
  return 0;
}