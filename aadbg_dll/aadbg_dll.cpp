﻿// strongOD_dll.cpp : 定义 DLL 应用程序的导出函数。


#include <Windows.h>
#include <cstring>
#include <cstdio>
#include "plugin.h"
#include "Ioctl.h"
#include "func.h"

#pragma comment(lib, "ollydbg.lib")


char g_szPluginName[] = "aadbg";
HWND g_hWndMain = NULL;
HINSTANCE g_hModule = NULL;




BOOL APIENTRY DllMain(
	HINSTANCE hModule,
	DWORD reason,
	LPVOID lpReserved)
{
	if (DLL_PROCESS_ATTACH == reason)
	{
		g_hModule = hModule;
	}

	return TRUE;
}


extc int _export cdecl ODBG_Plugindata(
	char shortname[32])
{
	strcpy(shortname, g_szPluginName);

	return PLUGIN_VERSION;
}


HANDLE g_hSys = NULL;

extc int _export cdecl ODBG_Plugininit(
	int ollydbgversion,
	HWND hw,
	ulong* features)
{

	if (ollydbgversion < PLUGIN_VERSION)
		return -1;

	g_hWndMain = hw; // od的窗口句柄

	if (!SetPrivilege(SE_DEBUG_NAME, TRUE))
	{
		MessageBox(g_hWndMain, "SetPrivilege failed!", g_szPluginName, MB_OK);
		return -1;
	}

	//int hook_cnt = HookAllProcess();
	//char str_hookcnt[64] = {0};
	//sprintf(str_hookcnt, "hook cnt: %d", hook_cnt);
	//MessageBox(g_hWndMain, str_hookcnt, g_szPluginName, MB_OK);

	// 调用aadbg_sys.sys驱动程序，hook ssdt和shadowssdt等
	//g_hSys = CreateFile("\\.\\aadbg_sys", FILE_ALL_ACCESS, FILE_SHARE_READ, NULL, 0, 0, NULL);
	//if (g_hSys == INVALID_HANDLE_VALUE || g_hSys == NULL)
	//	return -1;
	//BOOL bRet = DeviceIoControl(g_hSys, IOCTL_HOOK_SSDT, NULL, 0, NULL, 0, NULL, NULL);
	//if (!bRet)
	//	return -1;


	// 加载信息输出到log window，od启动后alt+l即可看到
	//MessageBox(g_hWndMain, "plugin init", g_szPluginName, MB_OK);
	Addtolist(0, 0, "aadbg v1.0");
	Addtolist(0, -1, "    Copyright (C) 2018 Xiang");

	return 0;

}



extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT* debug_event)
{
	if (debug_event == NULL) return;
	
	DWORD dwPID = debug_event->dwProcessId;

	if (dwPID == GetCurrentProcessId())
	{
		return;
	}

	if( CREATE_PROCESS_DEBUG_EVENT == debug_event->dwDebugEventCode )
	{
		HANDLE hProcess = debug_event->u.CreateProcessInfo.hProcess;
		if (!HookProcess(dwPID, TRUE, hProcess))
		{
			MessageBox(g_hWndMain, "hook failed", g_szPluginName, MB_OK);
		}
	}
	else if (CREATE_THREAD_DEBUG_EVENT == debug_event->dwDebugEventCode)
	{
		//HANDLE hThread = debug_event->u.CreateThread.hThread;
		//DWORD dwPID = GetProcessIdOfThread(hThread);
		//if (!HookProcess(dwPID, TRUE, NULL))
		//{
		//	MessageBox(g_hWndMain, "hook failed", g_szPluginName, MB_OK);
		//}
		
	}
}


extc int _export cdecl ODBG_Pluginmenu(
	int origin,
	char data[4096],
	void* item)
{
	//if (PM_MAIN == origin)
	//{
	//	// breakall可以，break all就不行，有空格会影响
	//	strcpy(data, "0 Hello | 1 breakall | 2 About");
	//	return 1;
	//}

	t_dump* pd;

	switch (origin)
	{
	case PM_MAIN: // 表示菜单项
		strcpy(data, "1 breakall | 2 About");
		return 1;
	case PM_DISASM: // 表示界面中右键菜单项
		pd = (t_dump*)item;
		if (pd == NULL)
			return 0;
		strcpy(data, "1 breakall");
		return 1;
	default:
		break;
	}

	return 0;
}


// 菜单响应
extc void _export cdecl ODBG_Pluginaction(
	int origin,
	int action,
	void* item)
{
	//if (PM_MAIN == origin)
	//{
		switch (action)
		{
		case 1:
			//MessageBox(g_hWndMain, "break all.", g_szPluginName, MB_OK);


			break;
		case 2:
			MessageBox(g_hWndMain, "Writen by Xiang", g_szPluginName, MB_OK);
			break;
		}
	//}
}



// 重新打开程序
extc void _export cdecl ODBG_Pluginreset(void)
{
	Addtolist(0, -1, "reset");
}


// WM_CLOSE消息尚未发送
extc int _export cdecl ODBG_Pluginclose(void)
{
	//MessageBox(g_hWndMain, "plugin close", g_szPluginName, MB_OK);
	return 0;
}

// WM_DESTROY消息已经收到
extc void _export cdecl ODBG_Plugindestroy(void)
{
	//UnhookAllProcess();

	//BOOL bRet = DeviceIoControl(g_hSys, IOCTL_UNHOOK_SSDT, NULL, 0, NULL, 0, NULL, NULL);

	//MessageBox(NULL, "plugin destroy", g_szPluginName, MB_OK);
	
}