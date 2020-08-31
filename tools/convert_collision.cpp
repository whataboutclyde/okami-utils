#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <fmt/format.h>
#include <okami-utils/akt.h>
using namespace std;
namespace fs = filesystem;

#define VERSION 0.1

void usage() {
  cerr << "Okami HD AKT to Wavefront Object (.obj) Converter v" << VERSION << " by whataboutclyde" << endl;
  cerr << "Usage: convert_collision [-h] [-s] [-o output_directory] input_file" << endl;
  cerr << "Options:" << endl;
  cerr << "  -a                   Tells the converter the input file is archive-type .AKT." << endl;
  cerr << "  -e                   Tells the converter the input file is entry-type .AK." << endl;
  cerr << "                       If neither -a nor -e is specified, it'll decide based on extension." << endl;
  cerr << "  -s                   If set, .obj will be split for each AK sub-file." << endl;
  cerr << "                       Otherwise, it will be stitched together." << endl;
  cerr << "                       Split has no effect on entry/.AK files." << endl;
  cerr << "  -o output_directory  Directory the .obj files should be written to." << endl;
  cerr << "                       Files will be named input_file.obj if stitched." << endl;
  cerr << "                       Files will be named input_file.##.obj if split." << endl;
  cerr << "                       DEFAULT: current working directory" << endl;
  cerr << "  -h                   View this usage information." << endl;
  cerr << "  input_file           Only Okami HD PC .AKT files are supported at this time." << endl;
}

void write_coordinates(ofstream& fout, OKAMI_UTILS::AK ak) {
  int coord_count = ak.num_coordinates();
  OKAMI_UTILS::Int16Tuple* coords = ak.get_coordinates();
  for (int i=0; i<coord_count; i++)
    fout << "v " << (float)coords[i].x << " " << (float)coords[i].y << " " << (float)coords[i].z << endl;
}

void write_vector_normals(ofstream& fout, OKAMI_UTILS::AK ak) {
  int coord_count = ak.num_coordinates();
  OKAMI_UTILS::Int8Tuple* vn = ak.get_vector_normals();
  for (int i=0; i<coord_count; i++)
    fout << "vn " << (float)vn[i].x << " " << (float)vn[i].y << " " << (float)vn[i].z << endl;
}

// AKT offsets from 0 but .obj requires offset from 1.
void write_index_sets(ofstream& fout, OKAMI_UTILS::AK ak, int offset = 1) {
  int index_count = ak.num_index_sets();
  uint16_t* indices = ak.get_index_sets();

  for (int n=0; n<index_count; n++)
    fout << "f " << indices[3*n]+offset << "//" << indices[3*n]+offset
         <<  " " << indices[3*n+1]+offset << "//" << indices[3*n+1]+offset
         <<  " " << indices[3*n+2]+offset << "//" << indices[3*n+2]+offset << endl; 
}

int convert_stitched(fs::path path, fs::path out_dir, OKAMI_UTILS::AKT akt) {
  fs::path obj_path = out_dir;
  obj_path /= path.filename();
  obj_path += ".obj";
  ofstream fout(obj_path, ios::out);
  if (!fout.is_open()) {
    cerr << "Can't write file: " << obj_path << endl;
    return -1;
  }

  for (int i=0; i<akt.size(); i++)
    write_coordinates(fout, akt.get(i));

  for (int i=0; i<akt.size(); i++)
    write_vector_normals(fout, akt.get(i));

  int index_offset = 1;
  for (int i=0; i<akt.size(); i++) {
    write_index_sets(fout, akt.get(i), index_offset);
    index_offset += akt.get(i).num_coordinates();
  }

  return 0;
}

int convert_split(fs::path path, fs::path out_dir, OKAMI_UTILS::AKT akt) {
  fs::path base_path = out_dir;
  base_path /= path.filename();

  for (int i=0; i<akt.size(); i++) {
    fs::path obj_path = base_path;
    obj_path += fmt::format(".{:#02}.obj", i);
    ofstream fout(obj_path, ios::out);
    if (!fout.is_open()) {
      cerr << "Can't write file: " << obj_path << endl;
      continue;
    }
    write_coordinates(fout, akt.get(i));
    write_vector_normals(fout, akt.get(i));
    write_index_sets(fout, akt.get(i));
    fout.close();
  }

  return 0;
}

int convert_archive(fs::path path, fs::path out_dir, bool split) {
  OKAMI_UTILS::AKT akt;
  if (!akt.parse_file(path)) {
    cerr << path << " is not a proper AKT file. Aborting." << endl;
    return -1;
  }

  if (split)
    return convert_split(path, out_dir, akt);
  else
    return convert_stitched(path, out_dir, akt);
}

int convert_entry(fs::path path, fs::path out_dir) {
  OKAMI_UTILS::AK ak;
  if (!ak.parse_file(path)) {
    cerr << path << " is not a proper AK file. Aborting." << endl;
    return -1;
  }

  fs::path obj_path = out_dir;
  obj_path /= path.filename();
  obj_path += ".obj";
  ofstream fout(obj_path, ios::out);
  if (!fout.is_open()) {
    cerr << "Can't write file: " << obj_path << endl;
    return -1;
  }

  write_coordinates(fout, ak);
  write_vector_normals(fout, ak);
  write_index_sets(fout, ak);
  fout.close();
  return 0;
}

enum FILE_TYPES {
  UNDEFINED_TYPE,
  ARCHIVE_TYPE,
  ENTRY_TYPE
};

int main(int argc, char *argv[]) {
  int opt;
  bool split = false;
  FILE_TYPES file_type = UNDEFINED_TYPE;
  fs::path out_dir = fs::current_path();
  while ((opt = getopt(argc,argv, "aehso:")) != EOF) {
    switch (opt) {
      case 's': split = true; break;
      case 'o': out_dir = fs::path(optarg); break;
      case 'h':
      case '?': usage(); return -1;
      case 'a': 
        if (file_type != UNDEFINED_TYPE) {
          cerr << "Conflicting type flags." << endl;
          return -1;
        }
        file_type = ARCHIVE_TYPE;
        break;
      case 'e': 
        if (file_type != UNDEFINED_TYPE) {
          cerr << "Conflicting type flags." << endl;
          return -1;
        }
        file_type = ENTRY_TYPE;
        break;
    }
  }

  if (argc-optind == 0) {
    cerr << "No input file supplied. Aborting." << endl;
    return -1;
  }

  if (argc-optind != 1) {
    cerr << "Too many arguments. Aborting." << endl;
    return -1;
  }

  fs::path path(argv[optind]);
  if (!fs::is_regular_file(path)) {
    cerr << path << " is not a regular file. Aborting." << endl;
    return -1;
  }

  if (file_type == UNDEFINED_TYPE) {
    if (path.extension() == ".AKT")
      file_type = ARCHIVE_TYPE;
    else if (path.extension() == ".AK")
      file_type = ENTRY_TYPE;
    else {
      cerr << "Can't determine type based on extension. Aborting." << endl;
      return -1;
    }
  }

  if (!fs::exists(out_dir) && !fs::create_directories(out_dir)) {
    cerr << "Could not create directory " << out_dir << ". Aborting." << endl;
    return -1;
  } else if (!fs::is_directory(out_dir)) {
    cerr << out_dir << " is not a directory. Aborting." << endl;
    return -1;
  }

  switch (file_type) {
    case ARCHIVE_TYPE: return convert_archive(path, out_dir, split);
    case ENTRY_TYPE:   return convert_entry(path, out_dir);
  }

  return -1;
}