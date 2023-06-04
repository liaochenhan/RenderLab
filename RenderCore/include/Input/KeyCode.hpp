/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Input
{
	enum class KeyCode : unsigned char
	{
		None				= 0x00,

		LeftMouseButton		= 0x01,
		RightMouseButton	= 0x02,
		MidMouseButton		= 0x04,

		Backspace	= 0x08,
		Tab			= 0x09,
		Enter		= 0x0D,
		Esc			= 0x1B,
		Space		= 0x20,
		PageUp		= 0x21,
		PageDown	= 0x22,
		End			= 0x23,
		Home		= 0x24,
		LeftArrow	= 0x25,
		UpArrow		= 0x26,
		RightArrow	= 0x27,
		DownArrow	= 0x28,
		Insert		= 0x2D,
		Delete		= 0x2E,
		Num0 = 0x30,
		Num1 = 0x31,
		Num2 = 0x32,
		Num3 = 0x33,
		Num4 = 0x34,
		Num5 = 0x35,
		Num6 = 0x36,
		Num7 = 0x37,
		Num8 = 0x38,
		Num9 = 0x39,
		A = 0x41, 
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		Keypad0 = 0x60,
		Keypad1 = 0x61,
		Keypad2 = 0x62,
		Keypad3 = 0x63,
		Keypad4 = 0x64,
		Keypad5 = 0x65,
		Keypad6 = 0x66,
		Keypad7 = 0x67,
		Keypad8 = 0x68,
		Keypad9 = 0x69,
		KeypadMutiply	= 0x6A,
		KeypadAdd		= 0x6B,
		KeypadSubtract	= 0x6D,
		KeypadPeriod	= 0x6E,
		KeypadDivide	= 0x6F,
		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		LeftShift		= 0xA0,
		RightShift		= 0xA1,
		LeftControl		= 0xA2,
		RightControl	= 0xA3,
		LeftAlt			= 0xA4,
		RightAlt		= 0xA5,
		Semicolon		= 0xBA,
		Comma			= 0xBC,
		Add				= 0xBB,
		Subtract		= 0xBD,
		Period			= 0xBE,
		Slash			= 0xBF,
		BackQuote		= 0xC0,
		LeftBracket		= 0xDB,
		BackSlash		= 0xDC,
		RightBracket	= 0xDD,
		Quote			= 0xDE
	};
}