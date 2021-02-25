#define OLC_PGE_APPLICATION
#define OLC_PGEX_SOUND
#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION

#include "synth2.h"
#include <iostream>

namespace olc
{
	struct parameters
	{
		int screenWidth = 1280;
		int screenHeight = 720;
		bool fullscreen = false;
		bool vsync = false;
		bool cohesion = false;
	};
}

void print_help(int args, char* argv[])
{
	std::cout
		<< "USAGE:\t" << argv[0] << " [options]\n"
		<< "\nOPTIONS:\n"
		<< "\t--channels [count]\tnumber of audio channels (1 by default)\n"
		<< "\t--sampleRate [rate]\tsamples per second (default 44100)\n"
		<< "\t--bufferSize [samples]\taudio processing buffer size (default 512)\n"
		<< "\t--width [width]\t\twindow width in pixels\n"
		<< "\t--height [height]\twindow height in pixels\n"
		<< "\t--disable-midi\t\texplicitly disables MIDI input\n"
		<< "\t--fullscreen\t\tenables fullscreen\n"
		<< "\t--vsync\t\t\tenables vsync, framerate locked to 60fps\n"
		<< "\t--help\t\t\tprints this page\n\n"
		<< "NOTE: Multi-channel support not yet implemented.\n\n";
}

void parse_args(int argc, char* argv[], olc::parameters& pge_config, synth2::parameters& synth_config)
{
	if (argc <= 1) return;
	std::string arg = "";
	try
	{
		for (int i = 1; i < argc; i++)
		{
			arg = argv[i];
			if (arg == "--help" || arg == "-h" || arg == "/?" || arg == "/h" || arg == "/help")
			{
				print_help(argc, argv);
				exit(EXIT_SUCCESS);
			}
			else if (arg == "--channels")
				synth_config.channels = std::stoi(argv[++i]);

			else if (arg == "--sampleRate")
				synth_config.sampleRate = std::stoi(argv[++i]);

			else if (arg == "--blocks")
				synth_config.blocks = std::stoi(argv[++i]);

			else if (arg == "--bufferSize")
				synth_config.bufferSize = std::stoi(argv[++i]);

			else if (arg == "--disable-midi")
				synth_config.midiEnabled = false;

			else if (arg == "--fullscreen")
				pge_config.fullscreen = true;

			else if (arg == "--vsync")
				pge_config.vsync = true;

			else if (arg == "--cohesion")
				pge_config.cohesion = true;

			else if (arg == "--width")
				pge_config.screenWidth = std::stoi(argv[++i]);

			else if (arg == "--height")
				pge_config.screenHeight = std::stoi(argv[++i]);
		}
	}
	catch (std::exception& e)
	{
		// improperly formatted args
		std::cerr << e.what() << std::endl;
		print_help(argc, argv);
		exit(EXIT_FAILURE);
	}
}

void enumerate_midi_devices(synth2::parameters& config)
{
	if (config.midiEnabled)
	{
		try {
			RtMidiIn midi;
			
			if (midi.getPortCount() == 0)
				throw RtMidiError("No MIDI devices available. Enabling QWERTY input.", RtMidiError::NO_DEVICES_FOUND);
			
			if (midi.getPortCount() > 1)
			{
				// have user choose a MIDI device if more than 1 available
				std::cout << "There are multiple MIDI input devices availble.\nPlease select one of the following:\n\n";
				for (int i = 0, N = midi.getPortCount(); i < N; i++)
				{
					std::cout << "\t" << i << ") " << midi.getPortName(i) << std::endl;
				}
				std::cout << "Device #: ";
				std::cin >> config.midiPort;
				std::cout << std::endl;
			}
		}
		catch (RtMidiError& error)
		{
			error.printMessage();
			config.midiEnabled = false;
		}
	}
}

int main(int argc, char* argv[])
{
	olc::parameters pge_conf;
	synth2::parameters synth_conf;
	parse_args(argc, argv, pge_conf, synth_conf);
	enumerate_midi_devices(synth_conf);

	synth2 app(synth_conf);
	app.Construct(pge_conf.screenWidth, pge_conf.screenHeight, 1, 1, pge_conf.fullscreen, pge_conf.vsync, pge_conf.cohesion);
	app.Start();
	return 0;
}
