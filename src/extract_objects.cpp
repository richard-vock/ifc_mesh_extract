#include <extract_objects.hpp>

#include <ifcgeom/IfcGeomIterator.h>
#include <ifcparse/IfcParse.h>
#include <ifcparse/Ifc2x3.h>
#include <set>

namespace ifc_mesh_extract {

template <class ColorType>
ifc_objects_t<ColorType>
extract_objects(std::string& root_guid, const fs::path& ifc_file, bool shared_vertices) {
    typedef typename cartan::openmesh_t<ColorType>::Point        point_t;
    typedef typename cartan::openmesh_t<ColorType>::Normal       normal_t;
    typedef typename cartan::openmesh_t<ColorType>::VertexHandle vertex_t;

    if (!fs::exists(ifc_file)) {
        throw std::runtime_error("Unable to open file \"" + ifc_file.string() + "\" for reading.");
    }

    IfcGeom::IteratorSettings settings;

    settings.set(IfcGeom::IteratorSettings::APPLY_DEFAULT_MATERIALS,      true);
    settings.set(IfcGeom::IteratorSettings::USE_WORLD_COORDS,             true);
    settings.set(IfcGeom::IteratorSettings::WELD_VERTICES,                shared_vertices);
    settings.set(IfcGeom::IteratorSettings::DISABLE_TRIANGULATION,        false);

    IfcGeom::Iterator<double> context_iterator(settings, ifc_file.string());

    IfcParse::IfcFile* file_ptr = context_iterator.getFile();
    auto root_entity = file_ptr->entitiesByType<IfcSchema::IfcRoot>();
    root_guid = (*root_entity->begin())->GlobalId();

    std::set<std::string> ignore_types;
    ignore_types.insert("ifcopeningelement");
    ignore_types.insert("ifcspace");

    context_iterator.excludeEntities(ignore_types);
    context_iterator.initialize();

    ifc_objects_t<ColorType> objects;

    do {
        ifc_object_t<ColorType> object;
        std::get<0>(object).request_vertex_normals();
        std::get<0>(object).request_face_normals();

        const IfcGeom::Element<double>* geom_object = context_iterator.get();
        auto triangulation = static_cast<const IfcGeom::TriangulationElement<double>*>(geom_object);
        const IfcGeom::Representation::Triangulation<double>& mesh = triangulation->geometry();

        // get type
        std::get<2>(object) = geom_object->type();
        for (std::string::iterator c = std::get<2>(object).begin(); c != std::get<2>(object).end(); ++c) {
            *c = tolower(*c);
        }

        std::get<1>(object) = geom_object->guid();

        // get polygon data
        auto& fs = mesh.faces();
        auto& vs = mesh.verts();
        auto& ns = mesh.normals();

        uint32_t v_count = vs.size() / 3;
        std::vector<vertex_t> vertex_handles;
        for (uint32_t v = 0; v < v_count; ++v) {
            auto vertex = std::get<0>(object).add_vertex(point_t(vs[3*v + 0], vs[3*v + 1], vs[3*v + 2]));

            if (!shared_vertices) {
                auto normal = normal_t(ns[3*v + 0], ns[3*v + 1], ns[3*v + 2]);
                float length = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
                normal[0] /= length;
                normal[1] /= length;
                normal[2] /= length;
                std::get<0>(object).set_normal(vertex, normal);
            }

            vertex_handles.push_back(vertex);
        }

        for (uint32_t f = 0; f < fs.size(); f += 3) {
            std::vector<vertex_t> vertices;
            for (uint32_t v = 0; v < 3; ++v) {
                int idx = fs[f+v];
                vertices.push_back(vertex_handles[idx]);
            }
            std::get<0>(object).add_face(vertices);
        }

        // update mesh-internal normal state
        if (shared_vertices) {
            std::get<0>(object).update_normals();
        } else {
            std::get<0>(object).update_face_normals();
        }

        objects.push_back(object);
    } while (context_iterator.next());

    return objects;
}

template <class ColorType>
cartan::openmesh_t<ColorType>
extract_single_mesh(std::string& root_guid, const fs::path& ifc_file) {
    auto objects = extract_objects<ColorType>(root_guid, ifc_file);
    return extract_single_mesh(objects);
}

template <class ColorType>
cartan::openmesh_t<ColorType>
extract_single_mesh(const ifc_objects_t<ColorType>& objects, std::vector<std::vector<int>>* vertex_index_map) {
    typedef typename cartan::openmesh_t<ColorType>::VertexHandle vertex_t;

    cartan::openmesh_t<ColorType> single_mesh = cartan::openmesh_t<ColorType>();
    for (const auto& object : objects) {
        if (vertex_index_map) {
            vertex_index_map->push_back(std::vector<int>());
        }
        const auto& mesh = std::get<0>(object);
        std::map<vertex_t, vertex_t> vertex_map;

        for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it) {
            auto vertex = mesh.point(*it);
            auto normal = mesh.normal(*it);
            auto color = mesh.color(*it);
            auto vh = single_mesh.add_vertex(vertex);
            single_mesh.set_normal(vh, normal);
            single_mesh.set_color(vh, color);
            vertex_map[*it] = vh;
            vertex_index_map->back().push_back(vh.idx());
        }
        for (auto it = mesh.faces_begin(); it != mesh.faces_end(); ++it) {
            std::vector<vertex_t> face;
            for (auto fvIt = mesh.cfv_iter(*it); fvIt.is_valid(); ++fvIt) {
                face.push_back(vertex_map[*fvIt]);
            }
            single_mesh.add_face(face);
        }
    }
    single_mesh.request_vertex_normals();
    single_mesh.request_face_normals();
    single_mesh.update_face_normals();
    single_mesh.update_normals();

    return single_mesh;
}


} // ifc_mesh_extract


// explicit instatiation for color types
#define INSTANTIATE_COLOR_TYPE(type) \
    template ifc_mesh_extract::ifc_objects_t<type> ifc_mesh_extract::extract_objects<type>(std::string&, const fs::path&, bool shared_vertices); \
    template cartan::openmesh_t<type> ifc_mesh_extract::extract_single_mesh<type>(std::string&, const fs::path&); \
    template cartan::openmesh_t<type> ifc_mesh_extract::extract_single_mesh<type>(const ifc_mesh_extract::ifc_objects_t<type>&, std::vector<std::vector<int>>*);
#include <cartan/openmesh_colors.def>
