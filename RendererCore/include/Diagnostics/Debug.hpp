/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "WinAPIException.hpp"
#include <iostream>

#ifdef ThrowIfFailed
#undef ThrowIfFailed
#endif

#ifdef ThrowIfFailedWithDesc
#undef ThrowIfFailedWithDesc
#endif

#ifdef ThrowLastError
#undef ThrowLastError
#endif

#ifdef ThrowLastErrorWithDesc
#undef ThrowLastErrorWithDesc
#endif

#define ThrowBaseExcept(description)\
			throw LCH::BaseException(description);

#define ThrowIfFailed(hr)\
			if (FAILED(hr))\
			{\
				throw LCH::WinAPIException(hr);\
			}

#define ThrowIfFailedWithDesc(hr, description)\
			if (FAILED(hr))\
			{\
				throw LCH::WinAPIException(hr, description);\
			}

#define ThrowLastError()\
		{\
			DWORD hr = GetLastError();\
			if (hr > 0)\
			{\
				throw LCH::WinAPIException(hr);\
			}\
		}

#define ThrowLastErrorWithDesc(description)\
		{\
			DWORD hr = GetLastError();\
			if (hr > 0)\
			{\
				throw LCH::WinAPIException(hr, description);\
			}\
		}

namespace LCH
{
	template<typename Text> struct text_trait;
	template<typename Text> struct text_trait<const Text> : text_trait<Text> {};

	template<typename CharT>
	struct text_trait<std::basic_string<CharT>>
	{
		static constexpr bool WideStream = std::is_same_v<CharT, wchar_t>;
	};

	template<typename CharT>
	struct text_trait<CharT const*>
	{
		static constexpr bool WideStream = std::is_same_v<CharT, wchar_t>;
	};

	template<typename CharT>
	struct text_trait<CharT*>
	{
		static constexpr bool WideStream = std::is_same_v<CharT, wchar_t>;
	};

	// 字符串常量
	template<typename CharT, size_t N>
	struct text_trait<CharT[N]>
	{
		static constexpr bool WideStream = std::is_same_v<CharT, wchar_t>;
	};
	
	class Debug
	{
		Debug() = delete;
		Debug(const Debug&) = delete;
		Debug& operator=(const Debug&) = delete;

	public:
		template<typename Text>
		static void Log(const Text& log) { std::cout << log; }
		template<typename Text> requires text_trait<Text>::WideStream
		static void Log(const Text& log) { std::wcout << log; }
		template<typename Text>
		static void LogLine(const Text& log) { std::cout << log << std::endl; }
		template<typename Text> requires text_trait<Text>::WideStream
		static void LogLine(const Text& log) { std::wcout << log << std::endl; }
	};
}