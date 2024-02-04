#pragma once
namespace hooks
{
	class hook_movement
	{
	public:
		static void install()
		{

			REL::Relocation<std::uintptr_t> MovementHandlerVtbl{ ConsoleRE::VTABLE_MovementHandler[0] };

			//REL::Relocation<std::uintptr_t> MovementHandlerVtbl{ REL::ID(263056) };
			
			_ProcessThumbstick = MovementHandlerVtbl.write_vfunc(0x5, ProcessThumbstick);
			_ProcessButton = MovementHandlerVtbl.write_vfunc(0x2, ProcessButton);
		}

	private:
		static void ProcessThumbstick(ConsoleRE::MovementHandler* a_this, ConsoleRE::ThumbstickEvent* a_event, ConsoleRE::PlayerControlsData* a_data);
		static void ProcessButton(ConsoleRE::MovementHandler* a_this, ConsoleRE::ButtonEvent* a_event, ConsoleRE::PlayerControlsData* a_data);

		static inline REL::Relocation<decltype(ProcessThumbstick)> _ProcessThumbstick;
		static inline REL::Relocation<decltype(ProcessButton)> _ProcessButton;
	};

	void install();
}
