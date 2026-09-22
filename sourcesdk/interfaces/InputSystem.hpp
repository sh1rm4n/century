//
// Created by sh1rm4n on 5/16/26.
//

#ifndef CENTURY_INPUTSYSTEM_HPP
#define CENTURY_INPUTSYSTEM_HPP

enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE, // 106

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST, // 107
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,
};

class InputSystem {
public:
	virtual void Connect(void* (*)(char const*, int*)) = 0;
	virtual void Disconnect(void) = 0;
	virtual void QueryInterface(char const*) = 0;
	virtual void Init(void) = 0;
	virtual void Shutdown(void) = 0;

	virtual void Undocumented1(void) = 0; // All of those 4 functions are unimplemented. Probably disabled by the devs or the compiler.
	virtual void Undocumented2(void) = 0;
	virtual void Undocumented3(void) = 0;
	virtual void Undocumented4(void) = 0;

	virtual void AttachToWindow(void*) = 0;
	virtual void DetachFromWindow(void) = 0;
	virtual void EnableInput(bool) = 0;
	virtual void EnableMessagePump(bool) = 0;
	virtual void PollInputState(void) = 0;
	virtual int GetPollTick(void)const = 0;
	virtual bool IsButtonDown(ButtonCode_t)const = 0;
	virtual int GetButtonPressedTick(ButtonCode_t)const = 0;
	virtual int GetButtonReleasedTick(ButtonCode_t)const = 0;
	virtual int GetAnalogValue(void*)const = 0;
	virtual int GetAnalogDelta(void*)const = 0;
	virtual int GetEventCount(void)const = 0;
	virtual int GetEventData(void)const = 0;
	virtual void PostUserEvent(void const*) = 0;
	virtual int GetJoystickCount(void)const = 0;
	virtual void EnableJoystickInput(int, bool) = 0;
	virtual void EnableJoystickDiagonalPOV(int, bool) = 0;
	virtual void SampleDevices(void) = 0;
	virtual void SetRumble(float, float, int) = 0;
	virtual void StopRumble(void) = 0;

	virtual void Undocumented5(void) = 0;
	virtual void Undocumented6(void) = 0;
	virtual void Undocumented7(void) = 0;
	virtual void Undocumented8(void) = 0;
	virtual void Undocumented9(void) = 0;
	virtual void Undocumented10(void) = 0;
	virtual void Undocumented11(void) = 0;
	virtual void Undocumented12(void) = 0;
	virtual void Undocumented13(void) = 0;
	virtual void Undocumented14(void) = 0;
	virtual void Undocumented15(void) = 0;

	virtual const char* ButtonCodeToString(ButtonCode_t code) const = 0;
	virtual const char* AnalogCodeToString(void* code) const = 0; // AnalogCode_t
	virtual ButtonCode_t StringToButtonCode(const char* pString) const = 0;
	virtual void* StringToAnalogCode(const char* pString) const = 0;

	virtual void SleepUntilInput(int nMaxSleepTimeMS = -1) = 0;

	virtual ButtonCode_t VirtualKeyToButtonCode(int nVirtualKey) const = 0;
	virtual int ButtonCodeToVirtualKey(ButtonCode_t code) const = 0;
};


#endif //CENTURY_INPUTSYSTEM_HPP
