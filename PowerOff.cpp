// PowerOff.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <Windows.h>

//判断是否在RELEASE模式下，如果是则不显示控制台窗口
#ifdef NDEBUG
//不显示控制台窗口
#pragma comment(linker,"/subsystem:windows /entry:mainCRTStartup")
#endif

// 计划之内的系统升级关闭
#define REASON_BASIC (SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)

/// <summary>
/// 我的Windows关机扩展函数
/// </summary>
/// <param name="uFlags">关闭选项(默认为关闭计算机:EWX_SHUTDOWN)</param>
/// <returns></returns>
BOOL MyExitWindowsEx(UINT uFlags = EWX_SHUTDOWN);

BOOL MyExitWindowsEx(UINT uFlags)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// Get a token for this process. 
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return(FALSE);

	// Get the LUID for the shutdown privilege. 
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process. 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;

	// Shut down the system and force all applications to close. 
	if (!ExitWindowsEx(uFlags | EWX_FORCE, REASON_BASIC))
		return FALSE;

	//shutdown was successful
	return TRUE;
}

int main()
{
	MyExitWindowsEx();
	//ShellExecuteW(NULL, NULL, L"powershell", L"Stop-Computer", NULL, SW_HIDE);
}