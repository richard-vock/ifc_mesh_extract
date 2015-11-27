#ifndef IFC_MESH_EXTRACT_EXTRACT_OBJECTS_HPP_
#define IFC_MESH_EXTRACT_EXTRACT_OBJECTS_HPP_

#include <vector>
#include <string>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <cartan/openmesh_traits.hpp>

#include "osdef.hpp"

namespace ifc_mesh_extract {

struct object_info {
  std::string path;
  std::string guid;
  std::string type;

  template <typename Archive> void serialize(Archive &ar) {
    ar(CEREAL_NVP(path));
    ar(CEREAL_NVP(guid));
    ar(CEREAL_NVP(type));
  }
};

template <class ColorType>
using ifc_object_t =
    std::tuple<cartan::openmesh_t<ColorType>, std::string, std::string>;

template <class ColorType>
using ifc_objects_t = std::vector<ifc_object_t<ColorType>>;

template <class ColorType = OpenMesh::Vec4f>
EXPORT_DECL ifc_objects_t<ColorType>
extract_objects(std::string &root_guid, const fs::path &ifc_file,
                bool shared_vertices = false);

template <class ColorType = OpenMesh::Vec4f>
EXPORT_DECL cartan::openmesh_t<ColorType>
extract_single_mesh(std::string &root_guid, const fs::path &ifc_file);

template <class ColorType = OpenMesh::Vec4f>
EXPORT_DECL cartan::openmesh_t<ColorType>
extract_single_mesh(const ifc_objects_t<ColorType> &objects,
                    std::vector<std::vector<int>> *vertex_index_map = nullptr);

} // ifc_mesh_extract

#endif /* IFC_MESH_EXTRACT_EXTRACT_OBJECTS_HPP_ */
