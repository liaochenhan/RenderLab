/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "TimeSystem.h"
#include "Input/InputSystem.h"
#include <locale>
#include <iostream>

namespace LCH
{
	enum class TestEnum : unsigned char
	{
		TEST1 = 1,
		TEST2 = 3,
		TEST3 = 5
	};

	class Application
	{
	public:
		Application();
		~Application();
		Application(const Application&) = delete;
		const Application& operator=(const Application&) = delete;

		int Run();

	private:
		std::locale locale;		// 语言区域
		TimeSystem timer;		// 时间系统
	};
}