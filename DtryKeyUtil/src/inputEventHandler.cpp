#include "inputEventHandler.h"
#include "inputHandler/movementInputTracer.h"
#include "inputHandler/inputTracer.h"
#include "inputHandler/animEvent.h"
#include "hasher.hpp"
#include "settings.h"


using EventType = ConsoleRE::INPUT_EVENT_TYPE;
using DeviceType = ConsoleRE::INPUT_DEVICE;
const auto ui = ConsoleRE::UI::GetSingleton();

inline void processKeyTrace(ConsoleRE::BSFixedString a_userEvent) 
{
	
}


inline void processAnimationEvent(ConsoleRE::BSFixedString a_userEvent) 
{
	
}

void inputEventHandler::offsetInputKeyIndex(uint32_t& a_key, ConsoleRE::INPUT_DEVICES::INPUT_DEVICE a_inputDevice) 
{
	switch (a_inputDevice) 
	{
	case DeviceType::kMouse:
		a_key += kMouseOffset;
		break;
	case DeviceType::kKeyboard:
		a_key += kKeyboardOffset;
		break;
	case DeviceType::kGamepad:
		a_key = GetGamepadIndex((ConsoleRE::BSOrbisGamepadDevice::Key)a_key);
		break;
	default:
		return;
	}
}

void inputEventHandler::processEventID(ConsoleRE::ButtonEvent* a_buttonEvent, bool isDown) 
{
	auto id = a_buttonEvent->idCode;
	auto device = a_buttonEvent->device;
	offsetInputKeyIndex(id, device);
	inputTracer::GetSingleton()->processIDInputTrace(id, device, isDown);
	if (isDown) 
	{
		animEvent::GetSingleton()->processInput(id, device);
	}
}
void inputEventHandler::processUserEventTrace(ConsoleRE::BSFixedString a_userEvent, bool isDown) 
{
	std::string userEvent = a_userEvent;
	inputTracer::GetSingleton()->processUserInputTrace(userEvent, isDown);
}


EventResult inputEventHandler::ProcessEvent(ConsoleRE::InputEvent* const* a_event, ConsoleRE::BSTEventSource<ConsoleRE::InputEvent*>*) 
{
	if (!a_event || ConsoleRE::UI::GetSingleton()->GameIsPaused()) 
	{
		return EventResult::kContinue;
	}

	for (auto one_event = *a_event; one_event; one_event = one_event->next) 
	{
		if (one_event->GetEventType() != ConsoleRE::INPUT_EVENT_TYPE::kButton) 
		{
			// get rid of all none-button events.
			return EventResult::kContinue;
		}

		auto buttonEvent = static_cast<ConsoleRE::ButtonEvent*>(one_event);
		bool isDown = buttonEvent->IsDown();

		if (!isDown && !buttonEvent->IsUp()) 
		{  // only perform trace on downs/ups
			return EventResult::kContinue;
		}
		auto userEvent = one_event->QUserEvent();

		if (settings::bLogUserEvent) 
		{
			ConsoleRE::ConsoleLog::GetSingleton()->Print(userEvent.c_str());
		}

		if (settings::bToggleUserEventInputTrace) 
		{
			processUserEventTrace(userEvent, isDown);
		}
		if (settings::bToggleEventIDInputTrace) 
		{
			processEventID(buttonEvent, isDown);
		}
	}

	return EventResult::kContinue;
}

std::uint32_t inputEventHandler::GetGamepadIndex(ConsoleRE::BSOrbisGamepadDevice::Key a_key) 
{
	return a_key;
}
