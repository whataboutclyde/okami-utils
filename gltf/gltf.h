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
    void change_buffer_length(int buffer, int len);
    int add_bufferView(int buffer, int len, int& offset);
    int add_mesh(std::string name, int pos, int norm, int ind, int itm, int tcw, int mat=-1);
    int add_parent_node(std::string name, int parent_node=-1);
    void add_node(int mesh, std::string name, int parent_node=-1);
    void scale_node(int node, PackedTuple<float> t);
    void rotate_node(int node, PackedTuple<float> t);
    void translate_node(int node, PackedTuple<float> t);
    void add_accessor_fv3(int view, int count);
    void add_accessor_fv3(int view, int count, FloatConstraints constraints);
    void add_accessor_ubv2(int view, int count);
    void add_accessor_ubv4(int view, int count);
    void add_accessor_uss(int view, int count);
    int add_material(const Value& mat);
    int add_image(std::string path);
    int add_texture(int src, string line="");
    void write(ofstream& fout);
  private:
    Document doc;
    Document::AllocatorType& al = doc.GetAllocator();
    static const int gltf_version;
    static const char* array_names[];
};

}

#endif // OKAMI_UTILS_GLTF_H