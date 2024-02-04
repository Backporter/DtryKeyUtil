#pragma once

#include "../../../OrbisUtil/include/FileSystem.h"

#include "lib/SimpleIni.h"

namespace utils
{
	inline void addSpellToPlayer(ConsoleRE::SpellItem* a_spell) 
	{
		auto pc = ConsoleRE::PlayerCharacter::GetSingleton();
		if (pc) 
		{
			pc->AddSpell(a_spell);
		}
	}

	inline void removeSpellFromPlayer(ConsoleRE::SpellItem* a_spell) 
	{
		auto pc = ConsoleRE::PlayerCharacter::GetSingleton();
		if (pc) 
		{
			pc->RemoveSpell(a_spell);
		}
	}

	inline void saeToPlayer(ConsoleRE::BSFixedString ae) 
	{
		auto pc = ConsoleRE::PlayerCharacter::GetSingleton();
		if (pc) 
		{
			pc->NotifyAnimationGraph(ae);
		}
	}

	inline void flipGraphVariableBoolForPlayer(ConsoleRE::BSFixedString& gvb) 
	{
		auto pc = ConsoleRE::PlayerCharacter::GetSingleton();
		if (pc) 
		{
			bool b;
			if (pc->GetGraphVariableBool(gvb, b)) 
			{
				pc->SetGraphVariableBool(gvb, !b);
			}
		}
	}



	/*Tokenize a string into vectors, separated by a common delimiter.
   @param s: string to be tokenized.
   @param delimiter: the delimiter based on which to tokenize the string.
   @return a vector containing all tokens.*/
	static std::vector<std::string> tokenize(std::string s, const char delimiter = '|')
	{
		size_t start = 0;
		size_t end = s.find_first_of(delimiter);

		std::vector<std::string> output;

		while (end <= std::string::npos)
		{
			std::string ss = s.substr(start, end - start);

			if (ss.size() != 0) 
			{ // if token has 0 size, skip it.
				output.emplace_back(ss);
			}


			if (end == std::string::npos)
				break;

			start = end + 1;
			end = s.find_first_of(delimiter, start);
		}

		return output;
	}

	static bool ToInt(std::string str, int& value)
	{
		const char* strVal = str.c_str();
		char* endVal = NULL;
		long ret = strtol(strVal, &endVal, 0);
		if (ret == LONG_MAX || ret == LONG_MIN || endVal != strVal + strlen(strVal))
			return false;
		value = ret;
		return true;
	}


}

class simpleIniUtils
{
public:
	static bool readSimpleIni(CSimpleIniA& a_ini, const char* a_iniAddress) 
	{
		if (xUtilty::FileSystem::PathExists(xUtilty::FileSystem::Full, a_iniAddress, false))
		{
			a_ini.LoadFile(a_iniAddress);
			return true;
		}
		else 
		{
			xUtilty::Log::GetSingleton()->Write(xUtilty::Log::logLevel::kNone, "Error: %s is not a valid .ini address.", a_iniAddress);
			return false;
		}
	}
	/// <summary>
	/// Run the function with every entry in the designated section as parameters.
	/// </summary>
	/// <param name="a_ini">ini to be loaded</param>
	/// <param name="a_section">section to be loaded</param>
	/// <param name="func">function that will take every entry in the section as a parameter</param>
	static void apply(CSimpleIniA& a_ini, const char* a_section, void (*func) (std::string a_string)) 
	{
		std::list<CSimpleIniA::Entry> keys;
		a_ini.GetAllKeys(a_section, keys);
		for (auto& one_key : keys) 
		{
			std::list<CSimpleIniA::Entry> values;
			a_ini.GetAllValues(a_section, one_key.pItem, values);
			for (auto& one_value : values) 
			{
				xUtilty::Log::GetSingleton()->Write(xUtilty::Log::logLevel::kNone, one_value.pItem);
				func(one_value.pItem);
			}
		}
	}
};

class gameDataUilts
{
public:
	template<class formType> 
	static void loadForm(ConsoleRE::TESDataHandler* a_TESDataHandler, formType*& a_form, std::string a_formID, std::string a_plugin) 
	{
		int raw_form = 0;
		utils::ToInt(a_formID, raw_form);
		auto read = a_TESDataHandler->LookupForm<formType>(raw_form, a_plugin.c_str());
		if (!read) 
		{
			xUtilty::Log::GetSingleton()->Write(xUtilty::Log::logLevel::kNone,"Invalid form. formID: %s, plugin: %s", a_formID.c_str(), a_plugin.c_str());
			return;
		}
		a_form = read;
	}
	template<class formType>
	static formType* getForm(ConsoleRE::TESDataHandler* a_TESDataHandler, std::string a_formID, std::string a_plugin) 
	{
		int raw_form = 0;
		utils::ToInt(a_formID, raw_form);
		formType* form = a_TESDataHandler->LookupForm<formType>(raw_form, a_plugin.c_str());
		if (!form) 
		{
			xUtilty::Log::GetSingleton()->Write(xUtilty::Log::logLevel::kNone,"Error: invalid form. formID: %s, plugin: %s", a_formID.c_str(), a_plugin.c_str());
			return nullptr;
		}
		return form;
	}

	/*Load a tesForm object to a_form.
	@param a_TESDataHandler: reference to dataHandler singleton.
	@param a_form: reference to the pointer to the form.
	@param a_iniLine: an ini line in the format of "0x0123|PluginName.esp"*/
	template<class formType> static void loadForm(ConsoleRE::TESDataHandler* a_TESDataHandler, formType*& a_form, std::string a_iniLine) 
	{
		auto configV = utils::tokenize(a_iniLine);
		auto formID = configV[0];
		auto plugin = configV[1];
		loadForm(a_TESDataHandler, a_form, formID, plugin);
	}
};
