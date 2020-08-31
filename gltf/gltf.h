#define RAPIDJSON_HAS_STDSTRING 1

#ifndef OKAMI_UTILS_GLTF_H
#define OKAMI_UTILS_GLTF_H

#include <string>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <okami-utils/common.h>
using namespace rapidjson;

namespace OKAMI_UTILS {

class GLTF {
  public:
    GLTF();
    ~GLTF() {};
    int add_buffer(int len, std::string uri);
    int add_bufferView(int buffer, int len, int offset=0);
    int add_mesh(std::string name, int pos, int norm, int ind, int mat=0);
    void add_node(int mesh, std::string name);
    void add_accessor_fv3(int view, int count);
    void add_accessor_fv3(int view, int count, FloatConstraints constraints);
    void add_accessor_uss(int view, int count);
    void add_material(const Value& mat);
    void write(ofstream& fout);
  private:
    Document doc;
    Document::AllocatorType& al = doc.GetAllocator();
    static const int gltf_version;
    static const char* array_names[];
};

}

#endif // OKAMI_UTILS_GLTF_H