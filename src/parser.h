#ifndef PARSER_H_
#define PARSER_H_

#include "cinebar_generator.h"

#include <CLI/CLI.hpp>

#include <string>

namespace app_parser
{
	cinebar::InputArgs ParseArgs(int argc, char **argv);
}

#endif