/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#ifdef _WIN64
	#include <winsdkver.h>
	#define _WIN32_WINNT 0x0A00
	#include <sdkddkver.h>

	#define WIN32_LEAN_AND_MEAN

	#define NOGDICAPMASKS
	#define NOMENUS
	#define NOICONS
	#define NOSYSCOMMANDS
	#define NORASTEROPS
	#define OEMRESOURCE
	#define NOATOM
	#define NOCLIPBOARD
	#define NOCOLOR
	#define NOCTLMGR
	#define NODRAWTEXT
	#define NOKERNEL
	#define NOMEMMGR
	#define NOMETAFILE
	#define NOOPENFILE
	#define NOSCROLL
	#define NOSERVICE
	#define NOSOUND
	#define NOTEXTMETRIC
	#define NOWH
	#define NOCOMM
	#define NOKANJI
	#define NOHELP
	#define NOPROFILER
	#define NODEFERWINDOWPOS
	#define NOMCX
	#define NORPC
	#define NOPROXYSTUB
	#define NOIMAGE
	#define NOTAPE
	#define NOMINMAX

	#include <windows.h>
	#include <windowsx.h>

	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
		#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#else
		#define DBG_NEW new
	#endif
#else
	#error Unsupport Platforms
#endif
