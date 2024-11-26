#include <iostream>
#include "Windows.h"


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void OnSize(HWND, UINT, int, int);

struct StateInfo {
	int n1;
	int n2;
	float n3;

};

int WINAPI wWinMain( 
	HINSTANCE hInstance, //  is the handle to an instance or handle to a module. 
						 //  The operating system uses this value to identify the executable or EXE when it's loaded in memory. 
						 //  Certain Windows functions need the instance handle, for example to load icons or bitmaps.
	HINSTANCE hPreviousInstance, // hPrevInstance HAS NO MEANING. It was used in 16-bit Windows, but is now always zero.
	PWSTR pCmdLine,		 //	contains the command-line arguments as a Unicode string
	int nCmdShow		 //  is a flag that indicates whether the main application window is minimized, maximized, or shown normally.
)
{
	StateInfo* pState = new (std::nothrow) StateInfo; // https://en.cppreference.com/w/cpp/memory/new/nothrow --> std::nothrow basically makes the new operator return nullptr instead of throwing the std::bad_alloc exception. How it does it and why is it written this way? idk

	if (pState == nullptr)
	{
		return 0;
	}

	pState->n1 = 100;
	pState->n2 = 250;
	pState->n3 = 25.5;

	WNDCLASS windowClass = { };

	// 1. To register a window class, we must fill an WNDCLASS struct first.
	windowClass.lpfnWndProc = WindowProc; // FORWARD DECLARATION.  It is a pointer to an application-defined function called the window procedure or window proc. The window procedure defines most of the behavior of the window.
	windowClass.hInstance = hInstance;    // is the handle to the application instance. 
	windowClass.lpszClassName = L"Sample Window Class"; // is a string that identifies the window class.

	// 2. Now, we need to register the class
	RegisterClass(&windowClass); // This function registers the window class with the operating system.

	// 3. Creating a new instance of a window:
	HWND hwnd = CreateWindowEx(
		WS_EX_COMPOSITED, // Optional. Window Styles.
		L"Sample Window Class", // Window Class. (same as defined in lpszClassName).
		L"Sample Window Title", // Window Text
		WS_OVERLAPPEDWINDOW, // Window Style
		// Size and position of the window
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, // Parent Window (NULL means this is the main window)
		NULL, // Menu (???)
		hInstance, // Instance Handle
		pState // Additional Application Data.
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	// 4. Show the Window
	ShowWindow(hwnd, nCmdShow);

	// Window Messaging Loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(
	HWND hwnd, // Handle para a instancia da janela
	UINT uMsg, // Mensagem
	WPARAM wParam, // Wide Parameter
	LPARAM lParam// Long Parameter. O Significado de o Wide e do Long Parameter dependem da mensagem em questão.
	)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		// In WM_CREATE the lParam contains a pointer to CREATESTRUCT (that has the same body as the CreateWindowEx calling arguments)
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		StateInfo* pState = reinterpret_cast <StateInfo*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);
		// The purpose of this last function call is to store the StateInfo pointer in the instance data for the window.
		// Once you do this, you can always get the pointer back from the window by calling GetWindowLongPtr.

	} return 0;
	case WM_PAINT:
	{

		LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
		StateInfo* pState = reinterpret_cast<StateInfo*>(ptr);

		PAINTSTRUCT ps = {};
		HDC hdc = BeginPaint(hwnd, &ps); // Fills the PAINTSTRUCT with the current paint request.
		// HDC stands for Handle to Device Context.

		// The FillRect function is part of the Graphics Device Interface (GDI), which has powered Windows graphics for a very long time
		// Toda a "pintura" da tela ocorre entre o BeginPaint e EndPaint.
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1)); // COLOR_WINDOW depends on user's color scheme.
		// ps.rcPaint contains the current update region.

		EndPaint(hwnd, &ps);

	}return 0;
	case WM_CLOSE: // DefWindowProc can handle this if no user confirmation is required.
	{
		if (MessageBox(hwnd, L"Really Quit?", L"My Application", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}
		// The user cancelled. Do Nothing.
		return 0;
	} return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} return 0;
	case WM_SIZE:
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);

		OnSize(hwnd, (UINT)wParam, width, height);
	} return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void OnSize(HWND hwnd, UINT flag, int width, int height)
{
	ShowWindow(hwnd, SW_RESTORE); // Just to check what happens.
}
