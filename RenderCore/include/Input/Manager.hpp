/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Event.hpp"
#include "DeviceFactory.hpp"
#include <queue>

namespace TG::Input
{
    // 输入管理器
    class Manager
	{
	public:
        Manager();
        Manager(const Manager&) = delete;
        Manager(Manager&&) = delete;
        Manager& operator=(const Manager&) = delete;
        Manager& operator=(Manager&&) = delete;
		~Manager();

		void PreUpdate();                                                       // 更新各种输入设备的状态

        void AddDevice(DeviceType type);                                        // 添加输入设备
        void RemoveDevice(DeviceType type);                                     // 移除输入设备
        void Dispatch(const Event& event);                                      // 分发输入事件

        void SpyInputEvent(bool enable, DeviceType type = DeviceType::None);    // 是否监控输入事件
		[[nodiscard]] bool GetKey(KeyCode key) const;		                    // 是否按住按键
		[[nodiscard]] bool GetKeyDown(KeyCode key) const;	                    // 是否按下按键
		[[nodiscard]] bool GetKeyUp(KeyCode key) const;		                    // 是否释放按键

    private:
        static DeviceType GetDeviceType(KeyCode key);                           // 根据按键码获取设备类型

	private:
        std::unordered_map<DeviceType, std::unique_ptr<Device>> m_devices;      // 输入设备列表
	};
}