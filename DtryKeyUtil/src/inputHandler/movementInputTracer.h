#pragma once

class movementInputTracer
{
public:
	enum thumbStickZone 
	{
		neutral = 0,
		up,
		down,
		left,
		right,
		upLeft,
		upRight,
		downLeft,
		downRight
	};

	enum traceMode 
	{
		keyBoard,
		gamePad
	};

	movementInputTracer() 
	{
		prevThumbStickZone = thumbStickZone::neutral;
		currentTraceMode = traceMode::keyBoard;
	}

	static movementInputTracer* GetSingleton()
	{
		static movementInputTracer singleton;
		return  std::addressof(singleton);
	}
	void loadMovementTraceSpells();
	//inline void onPerfectThumbStickMovementInput(float x, float y);
	inline bool updateProximityThumbstickZone(thumbStickZone new_thumbStickZone);
	inline void updateThumbstickInputTrace(thumbStickZone new_thumbStickZone);
	inline thumbStickZone getThumbStickZone(float x, float y);
	void onThumbStickMovement(ConsoleRE::ThumbstickEvent* a_thumbStickMovementInput);
	void onKeyBoardMovement(ConsoleRE::ButtonEvent* a_keyboardMovementInput);
private:
	inline void onForward(bool activate = true);
	inline void onBack(bool activate = true);
	inline void onLeft(bool activate = true);
	inline void onRight(bool activate = true);
	thumbStickZone prevThumbStickZone;
	ConsoleRE::SpellItem* movementSpell_forward;
	ConsoleRE::SpellItem* movementSpell_back;
	ConsoleRE::SpellItem* movementSpell_left;
	ConsoleRE::SpellItem* movementSpell_right;
	ConsoleRE::EffectSetting* TDM_TargetLockMGEF;
	traceMode currentTraceMode;
};