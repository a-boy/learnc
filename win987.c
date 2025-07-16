#include <windows.h>
#include <commctrl.h> // For common controls like ListView, ProgressBar, StatusBar
#include <stdio.h>    // For sprintf

// --- Control IDs ---
#define IDC_MAIN_EDIT          101
#define IDC_MAIN_BUTTON_SHOW   102
#define IDC_MAIN_BUTTON_CLEAR  103
#define IDC_MAIN_CHECKBOX      104
#define IDC_MAIN_RADIO1        105
#define IDC_MAIN_RADIO2        106
#define IDC_MAIN_COMBOBOX      107
#define IDC_MAIN_LISTBOX       108
#define IDC_MAIN_PROGRESS      109
#define IDC_MAIN_LISTVIEW      110
#define IDC_MAIN_STATUSBAR     111
#define IDC_MAIN_BUTTON_ADDLISTVIEW 112
#define IDC_MAIN_BUTTON_PROGRESS_INC 113

// --- Menu IDs ---
#define ID_FILE_EXIT           201
#define ID_HELP_ABOUT          202

// --- Global Variables ---
HINSTANCE hInst;
HWND hEdit, hButtonShow, hButtonClear, hCheckbox, hRadio1, hRadio2, hComboBox, hListBox;
HWND hProgress, hListView, hStatusBar, hButtonAddListView, hButtonProgressInc;
HWND hGroupRadio, hGroupOther;

// --- Forward Declarations ---
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddControls(HWND hwnd);
void AddMenus(HWND hwnd);
void InitializeListView(HWND hListView);
void AddItemToListView(HWND hListView, const char* col1, const char* col2, const char* col3);

// --- WinMain ---
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance; // Store instance handle in our global variable

    // Register the window class.
    const char CLASS_NAME[] = "ComplexWindowClass";

    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WindowProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL; // Will add menu manually
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Initialize common controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_PROGRESS_CLASS | ICC_BAR_CLASSES; // For ListView, ProgressBar, StatusBar
    InitCommonControlsEx(&icex);


    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "Complex C GUI Example",        // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,

        NULL,       // Parent window
        NULL,       // Menu (will be added in WM_CREATE or via AddMenus)
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Run the message loop.
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// --- Window Procedure ---
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            AddMenus(hwnd);
            AddControls(hwnd);
            SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100)); // Progress Bar Range 0-100
            SendMessage(hProgress, PBM_SETPOS, 20, 0); // Initial position
            SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)"Ready");
            return 0;

        case WM_COMMAND:
            {
                int wmId = LOWORD(wParam);
                int wmEvent = HIWORD(wParam);

                switch (wmId) {
                    case ID_FILE_EXIT:
                        DestroyWindow(hwnd);
                        break;
                    case ID_HELP_ABOUT:
                        MessageBox(hwnd, "A complex GUI example using C and WinAPI.", "About", MB_OK | MB_ICONINFORMATION);
                        break;
                    case IDC_MAIN_BUTTON_SHOW:
                        {
                            char buffer[256];
                            GetWindowText(hEdit, buffer, 256);
                            char message[512];
                            sprintf(message, "Edit Text: %s\nCheckbox is: %s\nRadio: %s",
                                    buffer,
                                    SendMessage(hCheckbox, BM_GETCHECK, 0, 0) == BST_CHECKED ? "Checked" : "Unchecked",
                                    SendMessage(hRadio1, BM_GETCHECK, 0, 0) == BST_CHECKED ? "Option 1" : "Option 2");
                            MessageBox(hwnd, message, "Info", MB_OK);
                        }
                        break;
                    case IDC_MAIN_BUTTON_CLEAR:
                        SetWindowText(hEdit, "");
                        SendMessage(hCheckbox, BM_SETCHECK, BST_UNCHECKED, 0);
                        SendMessage(hRadio1, BM_SETCHECK, BST_CHECKED, 0); // Default radio
                        SendMessage(hRadio2, BM_SETCHECK, BST_UNCHECKED, 0);
                        SendMessage(hComboBox, CB_SETCURSEL, 0, 0); // Select first item
                        SendMessage(hListBox, LB_SETCURSEL, -1, 0); // Deselect all
                        break;
                    case IDC_MAIN_BUTTON_ADDLISTVIEW:
                        {
                            static int itemCount = 0;
                            char itemNum[10];
                            char val1[20], val2[20];
                            sprintf(itemNum, "Item %d", ++itemCount);
                            sprintf(val1, "Value %d-A", itemCount);
                            sprintf(val2, "Value %d-B", itemCount);
                            AddItemToListView(hListView, itemNum, val1, val2);
                        }
                        break;
                    case IDC_MAIN_BUTTON_PROGRESS_INC:
                        {
                           int currentPos = SendMessage(hProgress, PBM_GETPOS, 0, 0);
                           currentPos += 10;
                           if (currentPos > 100) currentPos = 0; // Reset
                           SendMessage(hProgress, PBM_SETPOS, currentPos, 0);
                           char statusText[50];
                           sprintf(statusText, "Progress: %d%%", currentPos);
                           SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)statusText);
                        }
                        break;

                    case IDC_MAIN_COMBOBOX:
                        if (wmEvent == CBN_SELCHANGE) {
                            int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
                            char buffer[256];
                            SendMessage(hComboBox, CB_GETLBTEXT, index, (LPARAM)buffer);
                            char message[300];
                            sprintf(message, "ComboBox: Selected '%s'", buffer);
                            SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)message);
                        }
                        break;
                    case IDC_MAIN_LISTBOX:
                         if (wmEvent == LBN_SELCHANGE) {
                            int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                            if (index != LB_ERR) {
                                char buffer[256];
                                SendMessage(hListBox, LB_GETTEXT, index, (LPARAM)buffer);
                                char message[300];
                                sprintf(message, "ListBox: Selected '%s'", buffer);
                                SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)message);
                            }
                        }
                        break;
                }
            }
            return 0;

        case WM_NOTIFY:
            {
                LPNMHDR lpnmh = (LPNMHDR)lParam;
                if (lpnmh->hwndFrom == hListView && lpnmh->code == LVN_ITEMCHANGED) {
                    LPNMLISTVIEW pnmv = (LPNMLISTVIEW)lParam;
                    if ((pnmv->uChanged & LVIF_STATE) && (pnmv->uNewState & LVIS_SELECTED)) {
                        char buffer[256];
                        ListView_GetItemText(hListView, pnmv->iItem, 0, buffer, sizeof(buffer));
                        char message[300];
                        sprintf(message, "ListView: Selected '%s'", buffer);
                        SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)message);
                    }
                }
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_SIZE: // Handle window resize for status bar
            SendMessage(hStatusBar, WM_SIZE, 0, 0);
            // You could also resize/reposition other controls here if needed
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0; // Should not reach here
}

// --- Add Menus ---
void AddMenus(HWND hwnd) {
    HMENU hMenubar = CreateMenu();
    HMENU hMenuFile = CreateMenu();
    HMENU hMenuHelp = CreateMenu();

    AppendMenu(hMenuFile, MF_STRING, ID_FILE_EXIT, "&Exit");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenuFile, "&File");

    AppendMenu(hMenuHelp, MF_STRING, ID_HELP_ABOUT, "&About");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenuHelp, "&Help");

    SetMenu(hwnd, hMenubar);
}

// --- Add Controls ---
void AddControls(HWND hwnd) {
    // --- Left Column ---
    CreateWindow("STATIC", "Enter Text:", WS_VISIBLE | WS_CHILD,
                 10, 10, 100, 20, hwnd, NULL, hInst, NULL);
    hEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                         120, 10, 150, 20, hwnd, (HMENU)IDC_MAIN_EDIT, hInst, NULL);

    hButtonShow = CreateWindow("BUTTON", "Show Info", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                               10, 40, 100, 25, hwnd, (HMENU)IDC_MAIN_BUTTON_SHOW, hInst, NULL);
    hButtonClear = CreateWindow("BUTTON", "Clear All", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                120, 40, 100, 25, hwnd, (HMENU)IDC_MAIN_BUTTON_CLEAR, hInst, NULL);

    hCheckbox = CreateWindow("BUTTON", "Enable Feature", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                             10, 75, 150, 20, hwnd, (HMENU)IDC_MAIN_CHECKBOX, hInst, NULL);
    SendMessage(hCheckbox, BM_SETCHECK, BST_CHECKED, 0); // Default checked

    // Radio Button Group
    hGroupRadio = CreateWindow("BUTTON", "Options Group", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                               10, 105, 200, 80, hwnd, NULL, hInst, NULL);
    hRadio1 = CreateWindow("BUTTON", "Option 1", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,
                           20, 125, 100, 20, hwnd, (HMENU)IDC_MAIN_RADIO1, hInst, NULL);
    hRadio2 = CreateWindow("BUTTON", "Option 2", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                           20, 150, 100, 20, hwnd, (HMENU)IDC_MAIN_RADIO2, hInst, NULL);
    SendMessage(hRadio1, BM_SETCHECK, BST_CHECKED, 0); // Default selected

    // --- Middle Column ---
    CreateWindow("STATIC", "Select Item (ComboBox):", WS_VISIBLE | WS_CHILD,
                 230, 10, 150, 20, hwnd, NULL, hInst, NULL);
    hComboBox = CreateWindow("COMBOBOX", "", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                             230, 30, 150, 100, hwnd, (HMENU)IDC_MAIN_COMBOBOX, hInst, NULL);
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Apple");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Banana");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Cherry");
    SendMessage(hComboBox, CB_SETCURSEL, 0, 0); // Select first item

    CreateWindow("STATIC", "Select Item (ListBox):", WS_VISIBLE | WS_CHILD,
                 230, 70, 150, 20, hwnd, NULL, hInst, NULL);
    hListBox = CreateWindow("LISTBOX", "", LBS_STANDARD | WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
                            230, 90, 150, 95, hwnd, (HMENU)IDC_MAIN_LISTBOX, hInst, NULL);
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)"Dog");
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)"Cat");
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)"Bird");
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)"Fish");

    // --- Right Column / Bottom Area ---
    hGroupOther = CreateWindow("BUTTON", "More Controls", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                               10, 195, 600, 180, hwnd, NULL, hInst, NULL);

    CreateWindow("STATIC", "Progress:", WS_VISIBLE | WS_CHILD,
                 20, 215, 100, 20, hGroupOther, NULL, hInst, NULL);
    hProgress = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
                               120, 215, 200, 20, hGroupOther, (HMENU)IDC_MAIN_PROGRESS, hInst, NULL);
    hButtonProgressInc = CreateWindow("BUTTON", "Inc Progress", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                               330, 215, 100, 25, hGroupOther, (HMENU)IDC_MAIN_BUTTON_PROGRESS_INC, hInst, NULL);


    hListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "",
                               WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
                               20, 245, 450, 100, hGroupOther, (HMENU)IDC_MAIN_LISTVIEW, hInst, NULL);
    InitializeListView(hListView);
    // Add some initial items
    AddItemToListView(hListView, "Row 1 Col 1", "Row 1 Col 2", "Row 1 Col 3");
    AddItemToListView(hListView, "Row 2 Col 1", "Row 2 Col 2", "Row 2 Col 3");

    hButtonAddListView = CreateWindow("BUTTON", "Add to ListView", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                      480, 245, 110, 25, hGroupOther, (HMENU)IDC_MAIN_BUTTON_ADDLISTVIEW, hInst, NULL);

    // Status Bar
    hStatusBar = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
                                0, 0, 0, 0, hwnd, (HMENU)IDC_MAIN_STATUSBAR, hInst, NULL);
    int statwidths[] = { 200, -1 }; // First part 200px, second takes remaining space
    SendMessage(hStatusBar, SB_SETPARTS, sizeof(statwidths) / sizeof(int), (LPARAM)statwidths);
}

// --- ListView Functions ---
void InitializeListView(HWND hLV) {
    LVCOLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    // Column 1
    lvc.iSubItem = 0;
    lvc.pszText = "Column 1 Header";
    lvc.cx = 150;
    lvc.fmt = LVCFMT_LEFT;
    ListView_InsertColumn(hLV, 0, &lvc);

    // Column 2
    lvc.iSubItem = 1;
    lvc.pszText = "Column 2 Header";
    lvc.cx = 150;
    ListView_InsertColumn(hLV, 1, &lvc);

    // Column 3
    lvc.iSubItem = 2;
    lvc.pszText = "Column 3 Header";
    lvc.cx = 120;
    ListView_InsertColumn(hLV, 2, &lvc);

    // Enable full row select and grid lines (optional visual styles)
    ListView_SetExtendedListViewStyle(hLV, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void AddItemToListView(HWND hLV, const char* col1, const char* col2, const char* col3) {
    LVITEM lvi;
    lvi.mask = LVIF_TEXT;
    lvi.iIt-em = ListView_GetItemCount(hLV); // Insert at the end
    lvi.iSubItem = 0;
    lvi.pszText = (LPSTR)col1; // Cast because pszText is LPTSTR
    ListView_InsertItem(hLV, &lvi);

    ListView_SetItemText(hLV, lvi.iItem, 1, (LPSTR)col2);
    ListView_SetItemText(hLV, lvi.iItem, 2, (LPSTR)col3);
}