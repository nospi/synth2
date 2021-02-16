#define OLC_PGE_APPLICATION
#define OLC_PGEX_SOUND
#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION

#include "synth2.h"

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

void parse_args(int args, char* argv[], olc::parameters& pge_config, synth2::parameters& synth_config)
{
	if (args <= 1) return;
	std::string arg = "";
	for (int i = 1; i < args; i++)
	{
		arg = argv[i];
		if (arg == "--channels")
			synth_config.channels = std::stoi(argv[++i]);
		else if (arg == "--sampleRate")
			synth_config.sampleRate = std::stoi(argv[++i]);
		else if (arg == "--blocks")
			synth_config.blocks = std::stoi(argv[++i]);
		else if (arg == "--bufferSize")
			synth_config.bufferSize = std::stoi(argv[++i]);
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

int main(int args, char* argv[])
{
	olc::parameters pge_conf;
	synth2::parameters synth_conf;
	parse_args(args, argv, pge_conf, synth_conf);

	synth2 app(synth_conf);
	app.Construct(pge_conf.screenWidth, pge_conf.screenHeight, 1, 1, pge_conf.fullscreen, pge_conf.vsync, pge_conf.cohesion);
	app.Start();
	return 0;
}
