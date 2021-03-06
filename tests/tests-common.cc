#include "tests-common.h"
#include "openscad.h"
#include "module.h"
#include "handle_dep.h"
#include "boosty.h"

#include <sstream>
#include <fstream>

/*!
	fakepath is used to force the parser to believe that the file is
	read from this location, in order to ensure that filepaths are
	eavluated relative to this path (for testing purposes).
*/
FileModule *parsefile(const char *filename, const char *fakepath)
{
	FileModule *root_module = NULL;

	handle_dep(filename);
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
		fprintf(stderr, "Can't open input file `%s'!\n", filename);
	}
	else {
		std::string text((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		text += "\n" + commandline_commands;
		std::string pathname;
		if (fakepath) pathname = fakepath;
		else pathname = boosty::stringy(fs::path(filename).parent_path());
		root_module = parse(text.c_str(), pathname.c_str(), false);
		if (root_module) {
			root_module->handleDependencies();
		}
	}
	return root_module;
}
