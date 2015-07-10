#include <iostream>

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

int main (int argc, char const* argv[]) {
	std::string  file_in;
	std::string  dir_out;

	po::options_description desc("Graphene command line options");
	desc.add_options()
		("help,h",  "Help message")
		("input,i",     po::value<std::string>(&file_in) ->required(), "Input ifc file")
		("output,o",     po::value<std::string>(&dir_out), "Output directory")
		("shared,s",  "Make vertices shared between faces")
	;

	// Check for required options.
	po::variables_map vm;
	bool optionsException = false;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	} catch (std::exception& e) {
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

    fs::path path_out(dir_out);
    if (!fs::exists(path_out)) {
        std::cerr << "Directory \"" << dir_out << "\" does not exist. Aborting.\n";
        return 1;
    }

    std::string root_guid;
    auto objects = ifc_mesh_extract::extract_objects(root_guid, path_in, vm.count("shared"));

    auto filename_out = [=](std::string guid, std::string type) {
        std::string file_part = root_guid + "__GUID_" + guid + "__TYPE_" + type + ".obj";
        return (path_out / file_part).string();
    };

    if (!objects.size()) {
        std::cerr << "No objects extracted. Aborting." << "\n";
        return 1;
    }

    for (const auto& obj : objects) {
        std::string fname = filename_out(std::get<1>(obj), std::get<2>(obj));
        if (!OpenMesh::IO::write_mesh(std::get<0>(obj), fname)) {
            std::cerr << "Unable to write mesh to \"" + fname + "\"";
        }
    }

    return 0;
}
