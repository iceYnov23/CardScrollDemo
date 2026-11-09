# Qt-FlowCardWall

A responsive card wall demo built with **Qt 5.14.2 (Widgets) + qmake**, no QML.
Cards keep a fixed **1 : 1.2** aspect ratio, flow horizontally, wrap automatically,
and scale smoothly as the window resizes.

## Features

- Custom `FlowLayout` (based on the official Qt example) for horizontal flow + auto wrap
- Fixed 1:1.2 card aspect ratio, width driven by viewport width and column count
- Smooth proportional scaling on window resize, with a stable column count
- Robust handling of the **wrap / scrollbar boundary condition**
  (no misalignment when a card is added exactly at the edge of the viewport)
- Safe relayout via `QTimer::singleShot` — avoids re-entrant `processEvents()`
  inside `resizeEvent`
- Clean separation: `.ui` for the shell, `CardWidget` for the custom card paint,
  `FlowLayout` for the layout algorithm

## Project structure

```
Qt-FlowCardWall/
├── CardScrollDemo.pro     # qmake project
├── main.cpp
├── mainwindow.h / .cpp / .ui
├── cardwidget.h / .cpp    # custom card (rounded rect, title + wrapped desc)
└── flowlayout.h / .cpp    # official Qt FlowLayout, adapted
```

## Build

Tested with:

- Qt **5.14.2** (msvc2017_64)
- Visual Studio 2022 + **Qt Visual Studio Tools**
- qmake

Steps:

1. Install the **Qt Visual Studio Tools** extension.
2. `Qt VS Tools → Qt Versions` → add `C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin\qmake.exe`.
3. `Qt VS Tools → Open Qt Project File (.pro)` → select `CardScrollDemo.pro`.
4. If linking fails (`LNK2019` / MSVCRT mismatch), set the project's
   **Platform Toolset** to `Visual Studio 2019 (v142)`.

## How it works

| Concern                   | Implementation                                               |
| ------------------------- | ------------------------------------------------------------ |
| Card aspect ratio         | `CardWidget::sizeHint()` returns `(w, round(w * 1.2))`       |
| Horizontal flow + wrap    | Custom `FlowLayout` with `hasHeightForWidth() == true`       |
| Column count / card width | `MainWindow::doRelayoutIteration()` derives columns from viewport width |
| Boundary correctness      | `FlowLayout::sizeHint()` returns `heightForWidth(geometry().width())` |
| Re-entrancy safety        | `scheduleRelayout()` uses `QTimer::singleShot(0, ...)` instead of `processEvents()` |

## License

MIT
