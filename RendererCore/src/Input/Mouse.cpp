/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Mouse.h"
#include "Diagnostics/Debug.h"

namespace LCH
{
	Mouse::Mouse()
	{

	}

	Mouse::~Mouse()
	{

	}

	void Mouse::Update()
	{
		mouseDown.reset();
		mouseUp.reset();
		while (!eventBuffer.empty())
		{
			InputEvent e = eventBuffer.front();
			eventBuffer.pop();

			// 监控键盘
			if (spyMouse)
				Debug::LogLine(std::format(L"{}", e));
			// 跳过非按键消息
			if (e.key == KeyCode::None)
				continue;

			size_t key = static_cast<size_t>(e.key);
			switch (e.type)
			{
			case InputEvent::Type::Press:
				mouseDown[key] = true;
				mouseStates[key] = true;
				break;

			case InputEvent::Type::Release:
				mouseUp[key] = true;
				mouseStates[key] = false;
				break;
			}
		}
	}

	void Mouse::OnMouseMove(const POINTS& position)
	{
		this->position = position;
		eventBuffer.push(InputEvent{ KeyCode::None, InputEvent::Type::MouseMove });
		while (eventBuffer.size() > BUFSIZE)
			eventBuffer.pop();
	}

	void Mouse::OnButtonPress(KeyCode key)
	{
		eventBuffer.push(InputEvent{ key, InputEvent::Type::Press });
		while (eventBuffer.size() > BUFSIZE)
			eventBuffer.pop();
	}

	void Mouse::OnButtonRelease(KeyCode key)
	{
		eventBuffer.push(InputEvent{ key, InputEvent::Type::Release });
		while (eventBuffer.size() > BUFSIZE)
			eventBuffer.pop();
	}

	const POINTS& Mouse::Position() const noexcept
	{
		return position;
	}
}