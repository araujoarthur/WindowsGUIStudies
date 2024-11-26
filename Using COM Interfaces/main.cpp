#include <windows.h>
#include <shobjidl.h>

/*
* What is COM?
* COM is a specification for creating reusable software components. Many of the features that you will use in a modern Windows-based program rely on COM, such as the following:
* - Graphics (Direct2D)
* - Text (DirectWrite)
* - The Windows Shell
* - The Ribbon control
* - UI animation
* COM is a binary standard, not a language standard: It defines the binary interface between an application and a software component. 
* As a binary standard, COM is language-neutral, although it maps naturally to certain C++ constructs.
*/

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPWSTR pCmdLine, int nShowCmd)
{

	// Initializing the COM Library https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-coinitializeex
	// COINIT_APARTMENTTHREADED	-> If each COM object is going to be accessed from a single thread and the thread has a message loop
	// COINIT_MULTITHREADED -> Otherwise.

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;

		// Create Generic Com Object Instances: https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstance
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
		if (SUCCEEDED(hr))
		{
			hr = pFileOpen->Show(NULL);
			
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						CoTaskMemFree(pszFilePath); //https://learn.microsoft.com/nb-no/windows/win32/api/combaseapi/nf-combaseapi-cotaskmemfree
					}
					// Runs directly if GetDisplayName fails
					pItem->Release();
				}

			}
			// Runs Directly if Show fails
			pFileOpen->Release();
		}
		// Runs Directly if CoCreateInstance Failed
		CoUninitialize(); // UnInitialize https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-couninitialize
	}
	// Runs Directly if  CoInitialize failed
	return 0;


}