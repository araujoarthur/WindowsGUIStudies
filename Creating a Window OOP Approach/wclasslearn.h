#pragma once

// Remember that template classes must be declared and defined in header files, god knows why.
/*
Template classes and functions are not compiled independently like normal classes. Instead, the compiler generates code for templates only when they are instantiated.
Therefore, the full definition and implementation of the template class and its methods must be visible to the compiler at the point of instantiation.
*/
template <class DERIVED_TYPE>
class BaseWindow
{
public: 
	BaseWindow() : m_hwnd(NULL) { };
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE* pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			// Happens only if the message is WM_NCCREATE. Sets the m_hwnd of the structure passed via
			// lParam to the value of the window's hwnd. 
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = reinterpret_cast<DERIVED_TYPE*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
			pThis->m_hwnd = hwnd;
		}
		else
		{
			// If it's any other message, means it's already created and already have a GWLP_USERDATA information
			// So, retrieves the pThis pointer.
			pThis = reinterpret_cast<DERIVED_TYPE*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		// Checks if pThis is defined
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			// If there's no pThis set, just let the default handle.
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	BOOL Create(
			PCWSTR lpWindowName,
			DWORD dwStyle = 0,
			DWORD dwExStyle = 0,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = CW_USEDEFAULT,
			int nHeight = CW_USEDEFAULT,
			HWND hWndParent = 0,
			HMENU hMenu = 0
		)
	{
		WNDCLASS wc{ 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);

		m_hwnd = CreateWindowEx(
			dwExStyle,
			ClassName(),
			lpWindowName,
			dwStyle,
			x,
			y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			GetModuleHandle(NULL),
			this); // Passes pointer to this object to the CreateWindowEx, that will end being set inside the if (uMsg == WM_NCCREATE) above.

		return (m_hwnd ? TRUE : FALSE);
	}
	HWND Window() const
	{
		return m_hwnd;
	}

protected:
	virtual PCWSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT, WPARAM, LPARAM) = 0;

	HWND m_hwnd;
};


class MainWindow : public BaseWindow<MainWindow>
{
public:
	PCWSTR ClassName() const;
	LRESULT HandleMessage(UINT, WPARAM, LPARAM);
};