#include "hooks.h"
#include "inputHandler/movementInputTracer.h"
#include "settings.h"
namespace hooks
{
	void hook_movement::ProcessButton(ConsoleRE::MovementHandler* a_this, ConsoleRE::ButtonEvent* a_event, ConsoleRE::PlayerControlsData* a_data)
	{
		if (settings::bToggleMovementInputTrace) 
		{
			movementInputTracer::GetSingleton()->onKeyBoardMovement(a_event);
		}

		_ProcessButton(a_this, a_event, a_data);
	}

	void hook_movement::ProcessThumbstick(ConsoleRE::MovementHandler* a_this, ConsoleRE::ThumbstickEvent* a_event, ConsoleRE::PlayerControlsData* a_data) 
	{
		if (settings::bToggleMovementInputTrace) 
		{
			movementInputTracer::GetSingleton()->onThumbStickMovement(a_event);
		}

		_ProcessThumbstick(a_this, a_event, a_data);
	}

	void install()
	{
		hook_movement::install();
	}
}
