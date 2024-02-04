#pragma once

#include "../../../OrbisUtil/include/FileSystem.h"

#include "inputTracer.h"
#include "utils.h"

const char* InputTraceConfigDir = "/app0/Data/SKSE/Plugins/dtryKeyUtil/config/custom/";

void inputTracer::processUserInputTrace(userInputEvent a_userInput, bool isButtonDown) 
{
	auto it = keyTraceMap_userEvent.find(a_userInput);
	if (it != keyTraceMap_userEvent.end()) 
	{
		if (isButtonDown) 
		{
			for (auto& one_spell : *it->second) 
			{
				utils::addSpellToPlayer(one_spell);
			}
		} 
		else 
		{
			for (auto& one_spell : *it->second) 
			{
				utils::removeSpellFromPlayer(one_spell);
			}
		}
	}
	
}

void inputTracer::processInputTrace(std::unordered_map<IDEvent, std::shared_ptr<std::vector<ConsoleRE::SpellItem*>>> a_keyTraceMap, IDEvent a_eventID, bool isButtonDown)
{
	auto it = a_keyTraceMap.find(a_eventID);
	if (it != a_keyTraceMap.end()) 
	{
		if (isButtonDown) 
		{
			for (auto& spell : *it->second) 
			{
				utils::addSpellToPlayer(spell);
			}
		} 
		else
		{
			for (auto& spell : *it->second) 
			{
				utils::removeSpellFromPlayer(spell);
			}
		}
	}
}

void inputTracer::processIDInputTrace(IDEvent a_eventID, ConsoleRE::INPUT_DEVICE a_device, bool isButtonDown) 
{
	switch (a_device) 
	{
	case ConsoleRE::INPUT_DEVICE::kGamepad:
		processInputTrace(keyTraceMap_EventID_GamePad, a_eventID, isButtonDown);
		break;
	case ConsoleRE::INPUT_DEVICE::kMouse:
		processInputTrace(keyTraceMap_EventID_Mouse, a_eventID, isButtonDown);
		break;
	case ConsoleRE::INPUT_DEVICE::kKeyboard:
		processInputTrace(keyTraceMap_EventID_Keyboard, a_eventID, isButtonDown);
		break;
	}
}

void inputTracer::loadInputTraceConfigs() 
{
	xUtilty::Log::GetSingleton()->Write(xUtilty::Log::logLevel::kNone,"Loading custom input trace configurations...");

	int i = 0;
	if (xUtilty::FileSystem::IsDirectory(InputTraceConfigDir))
	{
		for (const auto& entry : xUtilty::FileSystem::GetDirectoryEntries(InputTraceConfigDir, nullptr, false, xUtilty::FileSystem::Full, true))
		{ 
			//iterates through all .ini files
			const char* path = entry.filename.c_str();

			xUtilty::Log::GetSingleton()->Write(xUtilty::Log::logLevel::kNone,"Loading from %s", path);
			loadInputTraceConfig(path);
			i++;
		}
		
		xUtilty::Log::GetSingleton()->Write(xUtilty::Log::logLevel::kNone,"..done; loaded %d ini configs.", i);
	}
	else 
	{
		// ERROR("Error: input trace config directory not found");
	}
	
}

void inputTracer::loadInputTraceConfig(const char* a_iniPath) 
{
	CSimpleIniA ini;
	ini.SetMultiKey(true);
	ini.LoadFile(a_iniPath);
	loadUserInputTraceConfig(ini);
	loadIDInputTraceConfig(ini);
}


void inputTracer::loadUserInputTraceConfig(CSimpleIniA& a_ini) 
{
	auto readOneLine = [](std::string a_line)
	{
		//logger::info(a_line);
		auto vec = utils::tokenize(a_line);
		std::string userInput = vec[0];
		std::string formID = vec[1];
		std::string plugin = vec[2];
		ConsoleRE::SpellItem* spell = gameDataUilts::getForm<ConsoleRE::SpellItem>(ConsoleRE::TESDataHandler::GetSingleton(), formID, plugin);
		auto ptr = inputTracer::GetSingleton();
		auto it = ptr->keyTraceMap_userEvent.find(userInput);
		if (it != ptr->keyTraceMap_userEvent.end()) 
		{
			it->second->push_back(spell);
		}
		else 
		{
			auto spellVec = std::make_shared<std::vector<ConsoleRE::SpellItem*>>();
			spellVec->push_back(spell);
			ptr->keyTraceMap_userEvent.insert({ userInput, spellVec });
		}
	};
	simpleIniUtils::apply(a_ini, "KeyTrace_userEvent", readOneLine);
}


void inputTracer::loadIDInputTraceConfig(CSimpleIniA& a_ini) 
{
	loadIDInputTraceConfig_GamePad(a_ini);
	loadIDInputTraceConfig_Keyboard(a_ini);
	loadIDInputTraceConfig_Mouse(a_ini);
}

void inputTracer::loadIDInputTraceConfig_Mouse(CSimpleIniA& a_ini) 
{
	auto readOneLine = [](std::string a_line)
	{
		auto vec = utils::tokenize(a_line);
		uint32_t EventID = static_cast<uint32_t>(std::stoul(vec[0]));
		std::string formID = vec[1];
		std::string plugin = vec[2];
		ConsoleRE::SpellItem* spell = gameDataUilts::getForm<ConsoleRE::SpellItem>(ConsoleRE::TESDataHandler::GetSingleton(), formID, plugin);
		auto ptr = inputTracer::GetSingleton();
		auto it = ptr->keyTraceMap_EventID_Mouse.find(EventID);
		if (it != ptr->keyTraceMap_EventID_Mouse.end()) 
		{
			it->second->push_back(spell);
		}
		else 
		{
			auto spellVec = std::make_shared<std::vector<ConsoleRE::SpellItem*>>();
			spellVec->push_back(spell);
			ptr->keyTraceMap_EventID_Mouse.insert({ EventID, spellVec });
		}
	};
	simpleIniUtils::apply(a_ini, "KeyTrace_EventID_Mouse", readOneLine);
}


void inputTracer::loadIDInputTraceConfig_GamePad(CSimpleIniA& a_ini) 
{
	auto readOneLine = [](std::string a_line)
	{
		//logger::info(a_line);
		auto vec = utils::tokenize(a_line);
		uint32_t EventID = static_cast<uint32_t>(std::stoul(vec[0]));
		std::string formID = vec[1];
		std::string plugin = vec[2];
		ConsoleRE::SpellItem* spell = gameDataUilts::getForm<ConsoleRE::SpellItem>(ConsoleRE::TESDataHandler::GetSingleton(), formID, plugin);

		auto ptr = inputTracer::GetSingleton();
		auto it = ptr->keyTraceMap_EventID_GamePad.find(EventID);
		if (it != ptr->keyTraceMap_EventID_GamePad.end()) 
		{
			it->second->push_back(spell);
		}
		else 
		{
			auto spellVec = std::make_shared<std::vector<ConsoleRE::SpellItem*>>();
			spellVec->push_back(spell);
			ptr->keyTraceMap_EventID_GamePad.insert({ EventID, spellVec });
		}
	};
	simpleIniUtils::apply(a_ini, "KeyTrace_EventID_GamePad", readOneLine);
}

void inputTracer::loadIDInputTraceConfig_Keyboard(CSimpleIniA& a_ini) 
{
	auto readOneLine = [](std::string a_line)
	{
		//logger::info(a_line);
		auto vec = utils::tokenize(a_line);
		uint32_t EventID = static_cast<uint32_t>(std::stoul(vec[0]));
		std::string formID = vec[1];
		std::string plugin = vec[2];
		ConsoleRE::SpellItem* spell = gameDataUilts::getForm<ConsoleRE::SpellItem>(ConsoleRE::TESDataHandler::GetSingleton(), formID, plugin);
		auto ptr = inputTracer::GetSingleton();
		auto it = ptr->keyTraceMap_EventID_Keyboard.find(EventID);
		if (it != ptr->keyTraceMap_EventID_Keyboard.end()) 
		{
			it->second->push_back(spell);
		}
		else 
		{
			auto spellVec = std::make_shared<std::vector<ConsoleRE::SpellItem*>>();
			spellVec->push_back(spell);
			ptr->keyTraceMap_EventID_Keyboard.insert({ EventID, spellVec });
		}
	};
	simpleIniUtils::apply(a_ini, "KeyTrace_EventID_Keyboard", readOneLine);
}
