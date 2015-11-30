#include <iostream>
#include <fstream>

#include "../include/extract_objects.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/none.hpp>
using boost::optional;
using boost::none;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

boost::filesystem::path make_relative(boost::filesystem::path a_From,
                                      boost::filesystem::path a_To) {
  a_From = boost::filesystem::absolute(a_From);
  a_To = boost::filesystem::absolute(a_To);
  boost::filesystem::path ret;
  boost::filesystem::path::const_iterator itrFrom(a_From.begin()),
      itrTo(a_To.begin());
  // Find common base
  for (boost::filesystem::path::const_iterator toEnd(a_To.end()),
       fromEnd(a_From.end());
       itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo;
       ++itrFrom, ++itrTo)
    ;
  // Navigate backwards in directory to reach previously found base
  for (boost::filesystem::path::const_iterator fromEnd(a_From.end());
       itrFrom != fromEnd; ++itrFrom) {
    if ((*itrFrom) != ".")
      ret /= "..";
  }
  // Now navigate down the directory branch
  // ret.append( itrTo, a_To.end() );
  for (; itrTo != a_To.end(); ++itrTo) {
    ret /= *itrTo;
  }
  return ret;
}

int main(int argc, char const *argv[]) {
  std::string file_in;
  std::string dir_out;
  std::string json_out;

  po::options_description desc("Graphene command line options");
  desc.add_options()("help,h", "Help message")(
      "input,i", po::value<std::string>(&file_in)->required(),
      "Input ifc file")("output,o", po::value<std::string>(&dir_out),
                        "Output directory")(
      "json,j", po::value<std::string>(&json_out)->default_value(""),
      "Optional JSON index file")("shared,s",
                                  "Make vertices shared between faces");

  // Check for required options.
  po::variables_map vm;
  bool optionsException = false;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch (std::exception &e) {
    if (!vm.count("help")) {
      std::cout << e.what() << "\n";
    }
    optionsException = true;
  }
  if (optionsException || vm.count("help")) {
    std::cout << desc << "\n";
    return optionsException ? 1 : 0;
  }

  fs::path path_in(file_in);
  if (!fs::exists(path_in)) {
    std::cerr << "File \"" << file_in << "\" does not exist. Aborting.\n";
    return 1;
  }

  bool json = json_out != "";

  fs::path path_out(dir_out);
  if (!fs::exists(path_out)) {
    fs::create_directories(path_out);
  }

  std::string root_guid;
  auto objects =
      ifc_mesh_extract::extract_objects(root_guid, path_in, vm.count("shared"));

  auto filename_out = [=](std::string guid, std::string type) {
    std::string file_part =
        root_guid + "__GUID_" + guid + "__TYPE_" + type + ".obj";
    return (path_out / file_part).string();
  };

  if (!objects.size()) {
    std::cerr << "No objects extracted. Aborting."
              << "\n";
    return 1;
  }

  std::vector<ifc_mesh_extract::object_info> obj_infos;
  fs::path json_path(json_out);
  for (const auto &obj : objects) {
    std::string fname = filename_out(std::get<1>(obj), std::get<2>(obj));
    OpenMesh::IO::Options wopts;
    wopts += OpenMesh::IO::Options::VertexNormal;
    //wopts += OpenMesh::IO::Options::FaceNormal;
    if (!OpenMesh::IO::write_mesh(std::get<0>(obj), fname, wopts)) {
      std::cerr << "Unable to write mesh to \"" + fname + "\"\n";
    }
    if (json) {
      ifc_mesh_extract::object_info info;
      info.path =
          make_relative(json_path.parent_path(), fs::path(fname)).string();
      info.guid = std::get<1>(obj);
      info.type = std::get<2>(obj);
      obj_infos.push_back(info);
    }
  }

  if (json && !obj_infos.empty()) {
    std::ofstream out(json_out.c_str());
    if (!out.good()) {
      std::cout << "Unable to open file \"" << json_out
                << "\" for writing. No JSON output written."
                << "\n";
    }

    {
      cereal::JSONOutputArchive ar(out);
      ar(cereal::make_nvp("guid", root_guid));
      ar(cereal::make_nvp("objects", obj_infos));
    }

    out.close();
  }

  return 0;
}
