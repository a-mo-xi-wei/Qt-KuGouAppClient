/**
* @file ElaWinShadowHelper.h
 * @brief 定义 Windows 平台窗口阴影辅助函数
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAWINSHADOWHELPER_H
#define ELAWINSHADOWHELPER_H

#include <qglobal.h>

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windowsx.h>
#endif
#ifdef Q_OS_WIN

/**
 * @brief 为指定窗口设置阴影效果（Windows 平台）
 * @param hwnd 窗口句柄
 */
[[maybe_unused]] static inline void setShadow(HWND hwnd)
{
    const MARGINS shadow = {1, 0, 0, 0};
    typedef HRESULT(WINAPI * DwmExtendFrameIntoClientAreaPtr)(HWND hWnd, const MARGINS* pMarInset);
    HMODULE module = LoadLibraryW(L"dwmapi.dll");
    if (module)
    {
        DwmExtendFrameIntoClientAreaPtr dwm_extendframe_into_client_area_;
        dwm_extendframe_into_client_area_ = reinterpret_cast<DwmExtendFrameIntoClientAreaPtr>(GetProcAddress(module, "DwmExtendFrameIntoClientArea"));
        if (dwm_extendframe_into_client_area_)
        {
            dwm_extendframe_into_client_area_(hwnd, &shadow);
        }
    }
}
#endif

#endif // ELAWINSHADOWHELPER_H