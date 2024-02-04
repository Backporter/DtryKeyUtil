using EventResult = ConsoleRE::BSEventNotifyControl;

class inputEventHandler : public ConsoleRE::BSTEventSink<ConsoleRE::InputEvent*>
{
public:
    virtual EventResult ProcessEvent(ConsoleRE::InputEvent* const* a_event, ConsoleRE::BSTEventSource<ConsoleRE::InputEvent*>* a_eventSource) override;

    static void Register()
    {
        auto deviceManager = ConsoleRE::BSInputDeviceManager::GetSingleton();
        deviceManager->AddEventSink(inputEventHandler::GetSingleton());
    }

private:
    inline void offsetInputKeyIndex(uint32_t& a_key, ConsoleRE::INPUT_DEVICES::INPUT_DEVICE a_inputDevice);
    inline void processEventID(ConsoleRE::ButtonEvent* a_buttonEvent, bool isDown);
	inline void processAnimEventTrigger(ConsoleRE::ButtonEvent* a_buttonEvent);

    inline void processUserEventTrace(ConsoleRE::BSFixedString a_userEvent, bool isDown);

    enum : std::uint32_t
    {
        kInvalid = static_cast<std::uint32_t>(-1),

        kKeyboardOffset = 0,
        kMouseOffset = 0,// 256,
        kGamepadOffset =0, // 266
    };
    static inputEventHandler* GetSingleton()
    {
        static inputEventHandler singleton;
        return std::addressof(singleton);
    }

    static std::uint32_t GetGamepadIndex(ConsoleRE::BSOrbisGamepadDevice::Key a_key);
};
