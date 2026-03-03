#include "parser.h"

namespace app_parser
{
	InputArgs ParseArgs(int argc, char **argv)
	{
		CLI::App app("CineBar CLI - Movie Barcode Generator");
		InputArgs args;

		app.add_flag("-v,--version", args.show_info, "Display application version info");
		app.add_option("input", args.input_video_path, "Input video file")->check(CLI::ExistingFile);
		app.add_option("-o,--output", args.output_img_path, "Output barcode image filename. If not provided, a name will be automatically generated.");

		app.parse(argc, argv);

		if (!args.show_info && args.input_video_path.empty())
		{
			throw CLI::RequiredError("Input video");
		}

		return args;
	}
}