//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//

#pragma once

#include "pch.h"

namespace Utility
{
#ifdef _CONSOLE
	inline void Print(const char* msg) { printf("%s", msg); }
	inline void Print(const wchar_t* msg) { wprintf(L"%ws", msg); }
#else
	inline void Print(const char* msg) { OutputDebugStringA(msg); }
	inline void Print(const wchar_t* msg) { OutputDebugString(msg); }
#endif

	inline void Printf(const char* format, ...)
	{
		char buffer[256];
		va_list ap;
		va_start(ap, format);
		vsprintf_s(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
	}

	inline void Printf(const wchar_t* format, ...)
	{
		wchar_t buffer[256];
		va_list ap;
		va_start(ap, format);
		vswprintf(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
	}

#ifdef RELEASE
	inline void PrintSubMessage(const char* format, ...)
	{
		Print("--> ");
		char buffer[256];
		va_list ap;
		va_start(ap, format);
		vsprintf_s(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
		Print("\n");
	}
	inline void PrintSubMessage(const wchar_t* format, ...)
	{
		Print("--> ");
		wchar_t buffer[256];
		va_list ap;
		va_start(ap, format);
		vswprintf(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
		Print("\n");
	}
	inline void PrintSubMessage()
	{

	}
#endif

	std::wstring AnsiToWideString(const std::string& str);
	std::string WideStringToAnsi(const std::wstring& wstr);
	std::string ToLower(const std::string& str);
	std::wstring ToLower(const std::wstring& wstr);
	std::string GetBasePath(const std::string& str);
	std::wstring GetBasePath(const std::wstring& wstr);
	std::string RemoveBasePath(const std::string& str);
	std::wstring RemoveBasePath(const std::wstring& wstr);
	std::string GetFileExtension(const std::string& str);
	std::wstring GetFileExtension(const std::wstring& wstr);
	std::string RemoveExtension(const std::string& str);
	std::wstring RemoveExtension(const std::wstring& wstr);
}