#include <windows.h>
#include "wclasslearn.h"

// MAIN WINDOW DEFS

PCWSTR MainWindow::ClassName() const
{
	return L"Sample Window Class (but an actual class)";
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDeviceContext = BeginPaint(m_hwnd, &ps);
		FillRect(hDeviceContext, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(m_hwnd, &ps);
	}
	return 0;
	default: 
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}
	
}