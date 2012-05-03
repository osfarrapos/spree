
#ifdef _XWEBPAGE_EXPORTS
#ifdef __cplusplus
#define XWEBPAGE_API extern "C"__declspec(dllexport)
#else
#define XWEBPAGE_API __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define XWEBPAGE_API extern "C" __declspec(dllimport)
#else
#define XWEBPAGE_API __declspec(dllimport)
#endif
#endif


XWEBPAGE_API long WINAPI EmbedBrowserObject(HWND hwnd);
XWEBPAGE_API void WINAPI UnEmbedBrowserObject(HWND hwnd);
XWEBPAGE_API long WINAPI DisplayHTMLPage(HWND hwnd, LPTSTR webPageName);
XWEBPAGE_API long WINAPI DisplayHTMLStr(HWND hwnd, LPTSTR string);
XWEBPAGE_API void WINAPI DoPageAction(HWND hwnd, DWORD action);
XWEBPAGE_API void WINAPI ResizeBrowser(HWND hwnd, DWORD width, DWORD height);