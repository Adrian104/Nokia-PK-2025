#Requires AutoHotkey v2.0
#SingleInstance Force

; Helper script for those coding under WSL2 with WSLg, for example under Clion IDE installed under WSL2.
; It reorganizes UE windows that spawn in random locations in WSLg
; (WSLg doesn't respect app's requested screen position https://github.com/microsoft/wslg/issues/355 ).
; Press F1 to reorder windows based on their titles, and don't move the mouse while script is running (it will mess up the drag operation).
; This script assumes that the windows are named "Nokia 123 (Ubuntu)", "Nokia 124 (Ubuntu)", "Nokia 125 (Ubuntu)", modify as needed.
; Moving windows to fixed positions based on their titles programativally doesnt work correctly with WSLg,
; because the window UI is moved to the desired location, but all mouse events are still read from the window's position before the move.
; This script uses mouse drag to move the windows to the desired positions, because only this updates mouse events source location correctly,
; to be exactly above the window in the new position.


SetTitleMatchMode("2")

topleftX := 690
topleftY := 200

windowTitles := ["Nokia 123 (Ubuntu)", "Nokia 124 (Ubuntu)", "Nokia 125 (Ubuntu)"]
windowList := []

for idx, title in windowTitles {
    x := topleftX + (idx - 1) * 410
    y := topleftY
    windowList.Push({ title: title, x: x, y: y })
}

F1:: ReorderWindows()

ReorderWindows() {
    global windowList

    for win in windowList {
        try {
            hwnd := WinExist(win.title)
            if !hwnd
                continue

            WinGetPos(&curX, &curY, &w, &h, hwnd)
            if (curX == win.x && curY == win.y)
                continue  ; Already in correct position (allowing ±10px wiggle room)

            ; Save current mouse position accurately
            prevCM := CoordMode("Mouse", "Screen")
            MouseGetPos(&oldX, &oldY)
            CoordMode("Mouse", prevCM)

            ; Calculate title bar drag points
            clickX := curX + 50
            clickY := curY + 10
            dragX := win.x + 50
            dragY := win.y + 10

            ToolTip("Dragging " . win.title . "`nFrom (" . clickX . ", " . clickY . ") to (" . dragX . ", " . dragY .
                ")")

            ; Activate and bring the window to front
            WinActivate(hwnd)
            Sleep(300)

            ; Simulate drag
            DllCall("SetCursorPos", "Int", clickX, "Int", clickY)
            Sleep(100)
            Click()
            Sleep(100)
            Click("Down Left")
            prevCM := CoordMode("Mouse", "Screen")
            MouseMove(dragX, dragY, 100)
            CoordMode("Mouse", prevCM)
            Click("Up Left")
            Sleep(100)

            ; Restore mouse position
            DllCall("SetCursorPos", "Int", oldX, "Int", oldY)
            Sleep(100)
        } catch Error as err {
            ToolTip("❌ Error dragging: " . win.title . "`n" . err.Message)
        } finally {
            ToolTip()  ; Clear tooltip after each operation
        }
    }
}
