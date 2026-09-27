/*
 * win32_controls_demo.c
 * A demonstration of common Win32 GUI controls in C.
 * This example is self-contained and can be compiled with TCC or GCC (MinGW).
 *
 * To compile with TCC:
 *   tcc win32_controls_demo.c -o win32_controls_demo.exe
 *
 * To compile with GCC (MinGW):
 *   gcc win32_controls_demo.c -o win32_controls_demo.exe -lcomctl32 -lgdi32 -luser32
 */

#include <windows.h>
#include <commctrl.h>

// Link with the common controls library
#pragma comment(lib, "comctl32.lib")

// Define control identifiers
#define IDC_STATIC_LABEL    1001
#define IDC_TEXT_INPUT      1002
#define IDC_CLICK_BUTTON    1003
#define IDC_CHECKBOX        1004
#define IDC_RADIO_A         1005
#define IDC_RADIO_B         1006
#define IDC_LISTBOX         1007
#define IDC_COMBOBOX        1008
#define IDC_PROGRESS_BAR    1009
#define IDC_GROUPBOX        1010

// Global variables
const char g_szClassName[] = "myWin32ControlsClass";
HWND hTextInput, hButton, hProgressBar;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreateControls(HWND);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize common controls.
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES | ICC_PROGRESS_CLASS;
    InitCommonControlsEx(&icex);

    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    // Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Win32 Controls Demonstration",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 480, 360,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

// Step 4: The Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            CreateControls(hwnd);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDC_CLICK_BUTTON: {
                    char buffer[256];
                    GetWindowText(hTextInput, buffer, 256);
                    MessageBox(hwnd, buffer, "Input Text", MB_OK);
                    // Update progress bar on button click
                    SendMessage(hProgressBar, PBM_STEPIT, 0, 0);
                    break;
                }
                case IDC_CHECKBOX: {
                    BOOL checked = IsDlgButtonChecked(hwnd, IDC_CHECKBOX);
                    if (checked) {
                        MessageBox(hwnd, "Checkbox is checked!", "Checkbox", MB_OK);
                    } else {
                        MessageBox(hwnd, "Checkbox is unchecked!", "Checkbox", MB_OK);
                    }
                    break;
                }
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void CreateControls(HWND hwnd) {
    // Create a Static Label
    CreateWindow("STATIC", "Enter Text:",
                 WS_VISIBLE | WS_CHILD,
                 10, 10, 100, 20,
                 hwnd, (HMENU)IDC_STATIC_LABEL, NULL, NULL);

    // Create an Edit Control (Text Input)
    hTextInput = CreateWindow("EDIT", "",
                              WS_VISIBLE | WS_CHILD | WS_BORDER,
                              120, 10, 200, 20,
                              hwnd, (HMENU)IDC_TEXT_INPUT, NULL, NULL);

    // Create a Button
    hButton = CreateWindow("BUTTON", "Click Me",
                           WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                           330, 10, 100, 25,
                           hwnd, (HMENU)IDC_CLICK_BUTTON, NULL, NULL);

    // Create a Checkbox
    CreateWindow("BUTTON", "Check this box",
                 WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                 10, 40, 150, 20,
                 hwnd, (HMENU)IDC_CHECKBOX, NULL, NULL);

    // Create a Group Box for Radio Buttons
    CreateWindow("BUTTON", "Options",
                 WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                 10, 70, 150, 80,
                 hwnd, (HMENU)IDC_GROUPBOX, NULL, NULL);

    // Create Radio Buttons
    CreateWindow("BUTTON", "Option A",
                 WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                 20, 90, 100, 20,
                 hwnd, (HMENU)IDC_RADIO_A, NULL, NULL);
    CreateWindow("BUTTON", "Option B",
                 WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                 20, 120, 100, 20,
                 hwnd, (HMENU)IDC_RADIO_B, NULL, NULL);
    // Set the first radio button as selected by default
    CheckRadioButton(hwnd, IDC_RADIO_A, IDC_RADIO_B, IDC_RADIO_A);


    // Create a List Box
    HWND hListBox = CreateWindow("LISTBOX", "",
                                 WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                                 180, 40, 120, 110,
                                 hwnd, (HMENU)IDC_LISTBOX, NULL, NULL);
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)"Apple");
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)"Banana");
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)"Orange");

    // Create a Combo Box
    HWND hComboBox = CreateWindow("COMBOBOX", "",
                                  CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD,
                                  310, 40, 120, 100,
                                  hwnd, (HMENU)IDC_COMBOBOX, NULL, NULL);
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Item 1");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Item 2");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Item 3");
    SendMessage(hComboBox, CB_SETCURSEL, 0, 0); // Select first item

    // Create a Progress Bar
    hProgressBar = CreateWindow(PROGRESS_CLASS, "",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                10, 160, 420, 25,
                                hwnd, (HMENU)IDC_PROGRESS_BAR, NULL, NULL);
    SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    SendMessage(hProgressBar, PBM_SETSTEP, (WPARAM)10, 0);
    SendMessage(hProgressBar, PBM_SETPOS, 0, 0);
}
