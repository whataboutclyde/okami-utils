#include <iostream>
using namespace std;
#include <okami-utils/archive.h>

namespace OKAMI_UTILS {

bool GenericArchive::process_file(ifstream& fin) {
  fin.read(reinterpret_cast<char*>(&num_files), sizeof(num_files));
  for (int i=0; i<num_files; i++) {
    FileMetadata fm;
    fin.read(reinterpret_cast<char*>(&fm.offset), sizeof(fm.offset));
    files.push_back(fm);
  }

  // I don't expect any extensions to be longer than 10 characters.
  char buffer[10];
  for (int i=0; i<num_files; i++) {
    fin.getline(buffer, 10, '\0');
    files[i].filename = buffer;
    align(fin, 4);
  }

  for (int i=0; i<num_files; i++) {
    fin.seekg(files[i].offset-8, ios::beg);
    string extension=files[i].filename.string();
    int len = extension.length();
    fin.read(buffer, len);
    while (extension.compare(0, len, buffer) != 0) {
      fin.seekg(-1-len, ios::cur);
      fin.read(buffer, len);
    }
    files[i].name_offset = fin.tellg();
    files[i].name_offset -= len;
    align(fin,4);
    char c;
    files[i].filename = "";
    while (fin.get(c) && c != '\0')
      files[i].filename += c;
    files[i].filename += ".";
    files[i].filename += extension;

    if (i>0)
      files[i-1].size = files[i].name_offset-files[i-1].offset;
  }

  fin.seekg(0, ios::end);
  files[num_files-1].size = fin.tellg();
  files[num_files-1].size -= files[num_files-1].offset;

  for (int i=0; i<num_files; i++) {
    cout << files[i].filename.string() << ": " << hex << files[i].name_offset << endl;
  }

  return true;
}

int GenericArchive::size() {
  return num_files;
}

} // namespace OKAMI_UTILS