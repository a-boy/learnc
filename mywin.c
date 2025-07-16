#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <math.h>

// 链接必要的库
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comdlg32.lib")

// 自定义消息
#define WM_USER_INIT_CONTROLS (WM_USER + 1)
#define IDC_MAIN_EDIT 1001
#define IDC_STATUS_BAR 1002
#define IDC_PROGRESS_BAR 1003
#define IDC_TREE_VIEW 1004
#define IDC_LIST_VIEW 1005
#define IDC_TAB_CONTROL 1006

// 全局变量
HINSTANCE hInst;
HWND hMainWnd, hEdit, hStatusBar, hProgressBar, hTreeView, hListView, hTabControl;
HMENU hMenu;
HBRUSH hBrushBackground;
HBITMAP hBitmap;
HFONT hFont;

// 函数声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
VOID InitCommonControlsEx();
VOID CreateControls(HWND hWnd);
VOID CreateMenu(HWND hWnd);
VOID CreateStatusBar(HWND hWnd);
VOID CreateTreeView(HWND hWnd);
VOID CreateListView(HWND hWnd);
VOID CreateTabControl(HWND hWnd);
VOID UpdateStatusText(LPCTSTR text);
VOID DrawGraph(HDC hdc, int width, int height);
VOID LoadBitmapResource();

// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;
    
    // 初始化通用控件
    InitCommonControlsEx();
    
    // 注册窗口类
    WNDCLASSEX wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    hBrushBackground = CreateSolidBrush(RGB(240, 240, 240));
    wcex.hbrBackground = hBrushBackground;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "ComplexWindowClass";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, "窗口注册失败!", "错误", MB_ICONERROR);
        return 1;
    }
    
    // 创建窗口
    hMainWnd = CreateWindowEx(
        0,
        "ComplexWindowClass",
        "复杂Windows窗体示例",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
        NULL, NULL, hInstance, NULL
    );
    
    if (!hMainWnd) {
        MessageBox(NULL, "窗口创建失败!", "错误", MB_ICONERROR);
        return 1;
    }
    
    // 显示并更新窗口
    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);
    
    // 发送自定义消息初始化控件
    PostMessage(hMainWnd, WM_USER_INIT_CONTROLS, 0, 0);
    
    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // 清理资源
    DeleteObject(hBrushBackground);
    if (hBitmap) DeleteObject(hBitmap);
    if (hFont) DeleteObject(hFont);
    
    return (int)msg.wParam;
}

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_USER_INIT_CONTROLS:
            CreateMenu(hWnd);
            CreateControls(hWnd);
            CreateStatusBar(hWnd);
            LoadBitmapResource();
            UpdateStatusText("窗体初始化完成");
            break;
            
        case WM_CREATE:
            // 创建字体
            hFont = CreateFont(
                14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, "Microsoft YaHei"
            );
            break;
            
        case WM_SIZE:
            // 调整控件大小
            if (hStatusBar) {
                SendMessage(hStatusBar, WM_SIZE, 0, 0);
            }
            if (hEdit) {
                RECT rcClient, rcStatus;
                GetClientRect(hWnd, &rcClient);
                if (hStatusBar) {
                    SendMessage(hStatusBar, SB_GETRECT, 0, (LPARAM)&rcStatus);
                    rcClient.bottom -= (rcStatus.bottom - rcStatus.top);
                }
                MoveWindow(hEdit, 10, 10, rcClient.right - 20, 100, TRUE);
            }
            if (hProgressBar) {
                RECT rc;
                GetClientRect(hWnd, &rc);
                MoveWindow(hProgressBar, 10, 120, rc.right - 20, 20, TRUE);
            }
            if (hTreeView) {
                RECT rc;
                GetClientRect(hWnd, &rc);
                MoveWindow(hTreeView, 10, 150, 200, rc.bottom - 170, TRUE);
            }
            if (hListView) {
                RECT rc;
                GetClientRect(hWnd, &rc);
                MoveWindow(hListView, 220, 150, rc.right - 240, (rc.bottom - 170) / 2, TRUE);
            }
            if (hTabControl) {
                RECT rc;
                GetClientRect(hWnd, &rc);
                MoveWindow(hTabControl, 220, 150 + (rc.bottom - 170) / 2 + 10, 
                          rc.right - 240, (rc.bottom - 170) / 2 - 10, TRUE);
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            // 获取客户区大小
            RECT rc;
            GetClientRect(hWnd, &rc);
            
            // 绘制背景
            FillRect(hdc, &rc, hBrushBackground);
            
            // 绘制位图
            if (hBitmap) {
                HDC hdcMem = CreateCompatibleDC(hdc);
                BITMAP bm;
                GetObject(hBitmap, sizeof(bm), &bm);
                SelectObject(hdcMem, hBitmap);
                BitBlt(hdc, 10, 10, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
                DeleteDC(hdcMem);
            }
            
            // 绘制图形
            DrawGraph(hdc, rc.right, rc.bottom);
            
            // 绘制文本
            SetTextColor(hdc, RGB(0, 0, 0));
            SetBkMode(hdc, TRANSPARENT);
            RECT textRect = {220, 10, rc.right - 10, 100};
            DrawText(hdc, "这是一个复杂的Windows窗体示例程序，展示了各种控件的使用方法和消息处理机制。", 
                    -1, &textRect, DT_WORDBREAK);
            
            EndPaint(hWnd, &ps);
            break;
        }
            
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDM_FILE_NEW:
                    MessageBox(hWnd, "新建文件菜单项被点击", "信息", MB_ICONINFORMATION);
                    UpdateStatusText("新建文件");
                    break;
                    
                case IDM_FILE_OPEN:
                    MessageBox(hWnd, "打开文件菜单项被点击", "信息", MB_ICONINFORMATION);
                    UpdateStatusText("打开文件");
                    break;
                    
                case IDM_FILE_SAVE:
                    MessageBox(hWnd, "保存文件菜单项被点击", "信息", MB_ICONINFORMATION);
                    UpdateStatusText("保存文件");
                    break;
                    
                case IDM_HELP_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
                    break;
            }
            break;
            
        case WM_NOTIFY:
            switch (((LPNMHDR)lParam)->code) {
                case NM_CLICK:
                    if (((LPNMHDR)lParam)->hwndFrom == hTreeView) {
                        TVITEM item;
                        HTREEITEM hItem = TreeView_GetSelection(hTreeView);
                        if (hItem) {
                            item.hItem = hItem;
                            item.mask = TVIF_TEXT;
                            TCHAR buffer[256];
                            item.pszText = buffer;
                            item.cchTextMax = 256;
                            TreeView_GetItem(hTreeView, &item);
                            UpdateStatusText(buffer);
                        }
                    }
                    break;
                    
                case NM_DBLCLK:
                    if (((LPNMHDR)lParam)->hwndFrom == hListView) {
                        int iItem = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
                        if (iItem != -1) {
                            TCHAR buffer[256];
                            ListView_GetItemText(hListView, iItem, 0, buffer, 256);
                            UpdateStatusText(buffer);
                        }
                    }
                    break;
                    
                case TCN_SELCHANGE:
                    if (((LPNMHDR)lParam)->hwndFrom == hTabControl) {
                        int iSel = TabCtrl_GetCurSel(hTabControl);
                        TCHAR buffer[256];
                        TabCtrl_GetItemText(hTabControl, iSel, buffer, 256);
                        UpdateStatusText(buffer);
                    }
                    break;
            }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 关于对话框过程
INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;
            
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}

// 初始化通用控件
VOID InitCommonControlsEx() {
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_TREEVIEW_CLASSES | ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icc);
}

// 创建控件
VOID CreateControls(HWND hWnd) {
    // 创建编辑框
    hEdit = CreateWindow(
        "EDIT", "这是一个多行编辑控件",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
        10, 10, 400, 100, hWnd, (HMENU)IDC_MAIN_EDIT, hInst, NULL
    );
    SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // 创建进度条
    hProgressBar = CreateWindow(
        PROGRESS_CLASS, NULL,
        WS_VISIBLE | WS_CHILD,
        10, 120, 400, 20, hWnd, (HMENU)IDC_PROGRESS_BAR, hInst, NULL
    );
    SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    SendMessage(hProgressBar, PBM_SETPOS, 50, 0);
    
    // 创建树视图
    CreateTreeView(hWnd);
    
    // 创建列表视图
    CreateListView(hWnd);
    
    // 创建标签控件
    CreateTabControl(hWnd);
}

// 创建菜单
VOID CreateMenu(HWND hWnd) {
    hMenu = CreateMenu();
    
    // 创建文件菜单
    HMENU hFileMenu = CreateMenu();
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW, "新建(&N)");
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_OPEN, "打开(&O)...");
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE, "保存(&S)");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_EXIT, "退出(&X)");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "文件(&F)");
    
    // 创建编辑菜单
    HMENU hEditMenu = CreateMenu();
    AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_CUT, "剪切(&T)");
    AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_COPY, "复制(&C)");
    AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_PASTE, "粘贴(&P)");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, "编辑(&E)");
    
    // 创建帮助菜单
    HMENU hHelpMenu = CreateMenu();
    AppendMenu(hHelpMenu, MF_STRING, IDM_HELP_ABOUT, "关于(&A)...");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, "帮助(&H)");
    
    // 设置菜单
    SetMenu(hWnd, hMenu);
}

// 创建状态栏
VOID CreateStatusBar(HWND hWnd) {
    hStatusBar = CreateWindow(
        STATUSCLASSNAME, NULL,
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
        0, 0, 0, 0, hWnd, (HMENU)IDC_STATUS_BAR, hInst, NULL
    );
    
    // 设置状态栏分区
    int parts[3] = {200, 400, -1};
    SendMessage(hStatusBar, SB_SETPARTS, 3, (LPARAM)parts);
    SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)"就绪");
    SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM)"第1行");
    SendMessage(hStatusBar, SB_SETTEXT, 2, (LPARAM)"第1列");
}

// 创建树视图
VOID CreateTreeView(HWND hWnd) {
    hTreeView = CreateWindow(
        WC_TREEVIEW, NULL,
        WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
        10, 150, 200, 200, hWnd, (HMENU)IDC_TREE_VIEW, hInst, NULL
    );
    
    // 设置字体
    SendMessage(hTreeView, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // 添加树节点
    TVINSERTSTRUCT tvInsert;
    HTREEITEM hRoot, hChild;
    
    // 根节点
    tvInsert.hParent = TVI_ROOT;
    tvInsert.hInsertAfter = TVI_LAST;
    tvInsert.item.mask = TVIF_TEXT;
    tvInsert.item.pszText = (LPSTR)"项目";
    hRoot = TreeView_InsertItem(hTreeView, &tvInsert);
    
    // 子节点1
    tvInsert.hParent = hRoot;
    tvInsert.item.pszText = (LPSTR)"文件";
    hChild = TreeView_InsertItem(hTreeView, &tvInsert);
    
    // 子节点1的子节点
    tvInsert.hParent = hChild;
    tvInsert.item.pszText = (LPSTR)"文档.docx";
    TreeView_InsertItem(hTreeView, &tvInsert);
    
    tvInsert.hParent = hChild;
    tvInsert.item.pszText = (LPSTR)"演示.pptx";
    TreeView_InsertItem(hTreeView, &tvInsert);
    
    // 子节点2
    tvInsert.hParent = hRoot;
    tvInsert.item.pszText = (LPSTR)"图片";
    hChild = TreeView_InsertItem(hTreeView, &tvInsert);
    
    // 子节点2的子节点
    tvInsert.hParent = hChild;
    tvInsert.item.pszText = (LPSTR)"风景.jpg";
    TreeView_InsertItem(hTreeView, &tvInsert);
    
    tvInsert.hParent = hChild;
    tvInsert.item.pszText = (LPSTR)"人物.png";
    TreeView_InsertItem(hTreeView, &tvInsert);
    
    // 展开根节点
    TreeView_Expand(hTreeView, hRoot, TVE_EXPAND);
}

// 创建列表视图
VOID CreateListView(HWND hWnd) {
    hListView = CreateWindow(
        WC_LISTVIEW, NULL,
        WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS,
        220, 150, 400, 150, hWnd, (HMENU)IDC_LIST_VIEW, hInst, NULL
    );
    
    // 设置字体
    SendMessage(hListView, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // 设置列表视图样式
    ListView_SetExtendedListViewStyle(hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    
    // 添加列
    LVCOLUMN column;
    column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt = LVCFMT_LEFT;
    
    column.pszText = (LPSTR)"名称";
    column.cx = 150;
    ListView_InsertColumn(hListView, 0, &column);
    
    column.pszText = (LPSTR)"类型";
    column.cx = 100;
    ListView_InsertColumn(hListView, 1, &column);
    
    column.pszText = (LPSTR)"大小";
    column.cx = 100;
    ListView_InsertColumn(hListView, 2, &column);
    
    // 添加项目
    LVITEM item;
    item.mask = LVIF_TEXT;
    
    // 项目1
    item.iItem = 0;
    item.iSubItem = 0;
    item.pszText = (LPSTR)"文件1.txt";
    ListView_InsertItem(hListView, &item);
    ListView_SetItemText(hListView, 0, 1, (LPSTR)"文本文件");
    ListView_SetItemText(hListView, 0, 2, (LPSTR)"1024 字节");
    
    // 项目2
    item.iItem = 1;
    item.pszText = (LPSTR)"图片1.jpg";
    ListView_InsertItem(hListView, &item);
    ListView_SetItemText(hListView, 1, 1, (LPSTR)"JPEG图像");
    ListView_SetItemText(hListView, 1, 2, (LPSTR)"456 KB");
    
    // 项目3
    item.iItem = 2;
    item.pszText = (LPSTR)"文档.docx";
    ListView_InsertItem(hListView, &item);
    ListView_SetItemText(hListView, 2, 1, (LPSTR)"Word文档");
    ListView_SetItemText(hListView, 2, 2, (LPSTR)"2.4 MB");
}

// 创建标签控件
VOID CreateTabControl(HWND hWnd) {
    hTabControl = CreateWindow(
        WC_TABCONTROL, NULL,
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        220, 310, 400, 150, hWnd, (HMENU)IDC_TAB_CONTROL, hInst, NULL
    );
    
    // 设置字体
    SendMessage(hTabControl, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // 添加标签页
    TCITEM tie;
    tie.mask = TCIF_TEXT;
    
    tie.pszText = (LPSTR)"常规";
    TabCtrl_InsertItem(hTabControl, 0, &tie);
    
    tie.pszText = (LPSTR)"选项";
    TabCtrl_InsertItem(hTabControl, 1, &tie);
    
    tie.pszText = (LPSTR)"高级";
    TabCtrl_InsertItem(hTabControl, 2, &tie);
    
    // 设置当前选中的标签页
    TabCtrl_SetCurSel(hTabControl, 0);
}

// 更新状态栏文本
VOID UpdateStatusText(LPCTSTR text) {
    SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)text);
}

// 绘制图形
VOID DrawGraph(HDC hdc, int width, int height) {
    // 创建画笔和画刷
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    
    // 绘制一些基本图形
    Rectangle(hdc, width - 200, 10, width - 100, 60);
    Ellipse(hdc, width - 180, 80, width - 120, 130);
    Polygon(hdc, 
        (POINT[]){
            {width - 160, 150},
            {width - 130, 200},
            {width - 190, 200}
        },
        3
    );
    
    // 绘制正弦曲线
    HPEN hSinePen = CreatePen(PS_SOLID, 1, RGB(0, 128, 0));
    SelectObject(hdc, hSinePen);
    
    const int points = 100;
    POINT sinePoints[points];
    
    for (int i = 0; i < points; i++) {
        float x = (float)i / points * 200;
        float y = sin(x / 20) * 30;
        sinePoints[i].x = width - 200 + i * 2;
        sinePoints[i].y = 250 + (int)y;
    }
    
    Polyline(hdc, sinePoints, points);
    
    // 清理资源
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
    DeleteObject(hSinePen);
}

// 加载位图资源
VOID LoadBitmapResource() {
    // 这里使用Windows自带的系统位图，实际应用中可以使用LoadImage加载自定义位图
    hBitmap = LoadBitmap(NULL, IDB_HAND);
}