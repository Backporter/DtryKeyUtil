#pragma once

#include "../../../OrbisUtil/include/RelocationManager.h"
#include "../../../OrbisUtil/include/Logger.h"
#include "../../../OrbisUtil/include/FileSystem.h"

extern "C" __declspec (dllexport) int module_start(size_t argc, const void* argv) { xUtilty::RelocationManager(); return 0; }
extern "C" __declspec (dllexport) int module_stop(size_t argc, const void* argv) { return 0; }

#include "inputEventHandler.h"
#include "inputHandler/movementInputTracer.h"
#include "inputHandler/inputTracer.h"
#include "hooks.h"
#include "settings.h"

#include "../../../CSEL/EXTERNAL/SKSE/SKSE.h"

#include <stddef.h>

void InitializeLogging() 
{
	xUtilty::Log::GetSingleton()->OpenRelitive(xUtilty::FileSystem::Download, "OSEL/Plugins/DtryKeyUtil/DtryKeyUtil.log");
}

void InitializeMessaging() 
{
	if (!API::GetMessagingInterface()->RegisterListener([](Interface::MessagingInterface::Message* message)
	{
		switch (message->type) 
		{
			// Skyrim lifecycle events.
		case Interface::MessagingInterface::kPostLoad: // Called after all plugins have finished running SKSEPlugin_Load.
			// It is now safe to do multithreaded operations, or operations against other plugins.
		case Interface::MessagingInterface::kPostPostLoad: // Called after all kPostLoad message handlers have run.
		case Interface::MessagingInterface::kInputLoaded: // Called when all game data has been found.
			break;
		case Interface::MessagingInterface::kDataLoaded: // All ESM/ESL/ESP plugins have loaded, main menu is now active.
			// It is now safe to access form data.
			xUtilty::Log::GetSingleton()->Write(xUtilty::Log::logLevel::kNone,"SKSEMsgInterface: dataLoaded");
			inputEventHandler::Register();
			settings::readSettings();
			if (settings::bToggleMovementInputTrace) 
			{
				movementInputTracer::GetSingleton()->loadMovementTraceSpells();
			}
			inputTracer::GetSingleton()->loadInputTraceConfigs();
			hooks::install();
			break;

			// Skyrim game events.
		case Interface::MessagingInterface::kNewGame: // Player starts a new game from main menu.
		case Interface::MessagingInterface::kPreLoadGame: // Player selected a game to load, but it hasn't loaded yet.
			// Data will be the name of the loaded save.
		case Interface::MessagingInterface::kPostLoadGame: // Player's selected save game has finished loading.
			// Data will be a boolean indicating whether the load was successful.
		case Interface::MessagingInterface::kSaveGame: // The player has saved a game.
			// Data will be the save name.
		case Interface::MessagingInterface::kDeleteGame: // The player deleted a saved game from within the load menu.
			break;
		}
		})) 
	{
		// stl::report_and_fail("Unable to register message listener.");
	}
}

extern "C" DLLEXPORT bool Query(const Interface::QueryInterface* a_skse, PluginInfo* a_info)
{
	a_info->SetPluginName("DtryKeyUtil");
	a_info->SetPluginVersion(1);

	return true;
}

extern "C" DLLEXPORT bool Load(const Interface::QueryInterface* a_skse)
{
	InitializeLogging();

	API::Init(a_skse);
	InitializeMessaging();

	xUtilty::Log::GetSingleton()->Write(xUtilty::Log::logLevel::kNone, "finished loading.");

	return true;
}

extern "C" DLLEXPORT bool Revert()
{
	return true;
}