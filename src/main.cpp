#include <iostream>
#include <fstream>
#include <cstdlib>

#include <argparse/argparse.hpp>
#include "alpha.h"
#include "png.h"

int main(int argc, char* argv[])
{
	argparse::ArgumentParser cmd_bleed("bleed");
	cmd_bleed.add_description("Alpha-bleed an image");
	cmd_bleed.add_argument("input")
		.required()
		.help("The input image");
	cmd_bleed.add_argument("output")
		.required()
		.help("The file where to save the result");

	argparse::ArgumentParser cmd_rm("remove");
	cmd_rm.add_description("Remove the transparency from an image");
	cmd_rm.add_argument("input")
		.required()
		.help("The input image");
	cmd_rm.add_argument("output")
		.required()
		.help("The file where to save the result");

	argparse::ArgumentParser cmd_set("set");
	cmd_set.add_description("Set the alpha channel for an image");
	cmd_set.add_argument("-a", "--alpha")
		.scan<'i', unsigned char>()
		.required()
		.help("The alpha value to set(0 - 255)")
		.metavar("alpha");

	cmd_set.add_argument("input")
		.required()
		.help("The input image");
	cmd_set.add_argument("output")
		.required()
		.help("The file where to save the result");

	argparse::ArgumentParser program("alpha", "1.0.1");

	program.add_subparser(cmd_bleed);
	program.add_subparser(cmd_rm);
	program.add_subparser(cmd_set);

	std::string input;
	std::string output;
	unsigned char value = 255;

	try {
		program.parse_args(argc, argv);
		if (program.is_subcommand_used(cmd_bleed)) {
			input = cmd_bleed.get<std::string>("input");
			output = cmd_bleed.get<std::string>("output");
		}
		else if (program.is_subcommand_used(cmd_rm)) {
			input = cmd_rm.get<std::string>("input");
			output = cmd_rm.get<std::string>("output");
		}
		else if (program.is_subcommand_used(cmd_set)) {
			input = cmd_set.get<std::string>("input");
			output = cmd_set.get<std::string>("output");
			value = cmd_set.get<unsigned char>("-a");
		}
		else {
			throw std::exception("No command provided!");
		}

		if (!fopen(input.c_str(), "rb")) {
			throw std::exception("Input file does not exist!");
		}
	}
	catch (const std::exception& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << program;
		return 1;
	}

	// If the output file already exists, confirm whether we want to overwrite it
	if (std::ifstream(output).good())
	{
		char response;
		std::cout << "Output file already exists! ";

		do
		{
			std::cout << "Overwrite? [y/n] ";
			std::cin >> response;
			if (response == 'n' || response == 'N') {
				std::cout << "Aborting." << std::endl;
				return 0;
			}
			else if (response == 'y' || response == 'Y') {
				break;
			}
		} while (!std::cin.fail());

		if (std::cin.fail()) {
			std::cerr << "cin failure. Could not get user input" << std::endl;
			return 1;
		}
	}

	// Load the file and check it's a RGBA PNG
	int w, h, c;
	unsigned char* data = png_load(input.c_str(), &w, &h, &c);

	if (data == nullptr)
	{
		std::cerr << "Error loading image. Must be PNG format." << std::endl;
		return 1;
	}

	if (c != 4)
	{
		std::cerr << "The image must be 32 bits (RGB with alpha channel)! Found " << c << " channels in this image." << std::endl;
		delete[] data;
		return 1;
	}

	// Process file
	if (program.is_subcommand_used(cmd_bleed)) {
		alpha_bleeding(data, w, h);
	}
	else {
		alpha_set(data, w, h, value);
	}

	// Save file and cleanup
	unsigned char exitcode = 0;
	if (png_save(output.c_str(), w, h, data)) {
		std::cout << "Saved image to " << output << std::endl;
	}
	else {
		std::cerr << "Error: could not save image!" << std::endl;
		exitcode = 1;
	}
	delete[] data;

	return exitcode;
}
