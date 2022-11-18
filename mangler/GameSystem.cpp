#include "GameSystem.h"

#include "EditorApplication.h"
#include "GameApplication.h"
#include <regex>
#include <io.h>
#include <QtWidgets>
//#include "ui_console.h"
#include "MainFrame.h"

void GameSystem::ConsoleSubsystem()
{
	// Create the console.
	//Ui_Dialog* consoleDiag;
	//consoleDiag = new Ui_Dialog();
	//consoleDiag->Show();
	//consoleDiag->setupUi();
}

int GameSystem::ApplicationLoader(GameConfig* SystemConfig)
{
	EngineApplication			*app;
	sf::VideoMode				mode;
	int	argc = SystemConfig->argc;
	QApplication	App(argc, SystemConfig->argv);

	app = nullptr;


	// Create the main Q frame.
	MainFrame* mainFrame = new MainFrame();
	mainFrame->setWindowTitle(QString(SystemConfig->GetAppTitle().c_str()));

	// Fully parse and apply the system configuration.
	SystemConfig->ApplyConfiguration();

	// Configure and execute the desired engine application.
	if (SystemConfig->IsEnabled(GameStartupOption::EDITOR_MODE))
	{
		// Run the Editor.
		mode = SystemConfig->GetVideoMode();
		mainFrame->resize(mode.width, mode.height);
		mainFrame->show();
		app = new EditorApplication(*SystemConfig, mainFrame, QPoint(0, 100), QSize(1600, 800));

		// Run the Application.
		app->Run();
		if (!app->IsGameFullscreen)
		{
			app->show();
		}

		return App.exec();
	}
	else if (SystemConfig->IsEnabled(GameStartupOption::GAME_RUNTIME_MODE))
	{
		// Play the Game.
		mainFrame->resize(0, 0); // HACK: resized QFrame to nothing so that it won't show because we are in game mode.
		mainFrame->show();
		app = new GameApplication(*SystemConfig, mainFrame);

		// Run the Application.
		app->Run();

		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

void GameSystem::ParseArgs(int argc, char **argv, GameConfig& system)
{
	GameConfig							opt;
	std::map<std::string, bool>			options;
	std::map<std::string, std::string>	optionStrs;
	std::string							arg;
	int									arg_i;

	options["test"] = false;
	options["editor"] = false;
	options["tests-only"] = false;
	options["hide-errors"] = false;

	for (arg_i = 0; arg_i < argc - 1; arg_i++)
	{
		arg = argv[arg_i + 1];

		if (arg == "--tests")
			options["test"] = true;
		else if (arg == "only")
			options["tests-only"] = true;
		else if (arg == "--editor")
			options["editor"] = true;
		else if (arg == "--show-errors")
			options["hide-errors"] = false;
		else if (arg == "--dataStore")
			optionStrs["data-store-uri"] = (arg_i + 2 < argc - 1) ? argv[arg_i + 2] : "invalid";
		// IF arg_i + 2 does not exist then the 2nd parameter following the current arg is missing.
	}

	opt.Options = options;
	opt.StartupParameters = optionStrs;
	opt.argc = argc;
	opt.argv = argv;

	system = opt;
}

void GameSystem::ParseSysDatabase(GameConfig& system, std::string sysDb)
{
	int					errors = 0;
	int					lineno = 0;
	std::ifstream		file;
	std::string			line;
	std::string			*tmp;
	std::string			curr_section = "";
	bool				verbose;
	std::map<std::string, std::map<std::string, std::string>> dictionarySections;

	verbose = system.Game["env"]["ATHENA_VERBOSE"] == "1";

	// SYSTEM CONF
	file.open(sysDb, std::ios::in);

	if (file.is_open())
	{
		std::cout << "Loading base system configuration " << std::endl;
		IniLine parsedLine;

		while (std::getline(file, line))
		{
			lineno++;

			switch (ConfParser::ParseLine(line, &parsedLine))
			{
			case IniToken::LINE_EMPTY:
			case IniToken::LINE_COMMENT:
				break;
			case IniToken::LINE_SECTION:
				curr_section = parsedLine.section;

				if (verbose)
				{
					std::cout << "Loading " << parsedLine.section << " settings.." << std::endl;
				}

				dictionarySections[curr_section] = std::map<std::string, std::string>();

				break;
			case IniToken::LINE_VALUE:

				if (curr_section == "")
					curr_section = "General";

				dictionarySections[curr_section][parsedLine.key] = parsedLine.value;

				if (verbose)
				{
					std::cout << "\t" << parsedLine.key << " " << parsedLine.value << std::endl;
				}

				break;

			case IniToken::LINE_ERROR:

				std::cout << "ConfParser: syntax error in " << sysDb << " (" << lineno << "):\n-> " << line << std::endl;

				errors++;

				break;

			default:
				break;
			}
		}

		file.close();
	}
	else
	{
		std::cout << "Missing " << sysDb << std::endl;
	}

	file.close();

	system.Game = dictionarySections;
}

void GameSystem::ParseEnvVariables(GameConfig& system, char **envp)
{
	if (envp == nullptr) return;

	int env_i;
	char* s;
	std::string name;
	std::string key;
	std::string value;
	char * pkey;

	// Determine if the program has a terminal subsystem
	// IF it does, then there will be environment variables.
	// There is no guarantee that envp won't return garbage memory 
	// as it is not in the C++11 standard.

	bool isterminal = false;
#ifdef WIN32
	if (_isatty(_fileno(stdout)))
	{
		isterminal = true;
	}
#elif WIN64
	if (_isatty(_fileno(stdout)))
	{
		isterminal = true;
	}
#else
	if (isatty(STDOUT_FILENO))
	{
		isterminal = true;
	}
#endif
	if (!isterminal) return;

	// Having a terminal guarantees environment variables.
	// Enumerate the environment variables.
	for (env_i = 0; envp[env_i] != '\0'; env_i++)
	{
		name = std::string(envp[env_i]);

		pkey = strtok((char*)name.c_str(), "=");
		key = std::string(pkey);
		value = std::string(std::getenv(pkey));

		system.Game["env"][key] = value;
	}
}