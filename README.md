Gradient Picker Dialog
======================

This project includes the Qt gradient picker library itself and a demo application demonstrating how this dialog looks.

Provided dialog was written as a missing part that complements `QColorDialog` for one personal project that unfortunately did not come to life, and just shared as a likely useful component for some other graphics-related project.

Building
--------

Provided code is compatible with both Qt5 and Qt6, but with an accent to Qt6 (some compatibility stuff was added to support Qt5 build due to interface changes). It doesn't require any private Qt interfaces, depending on only the Qt Widgets library.
C++14 is mentioned as a requirement, but actually, it can be compiled with C++11, so this requirement can be relaxed if required. C++14 is enforced just because some Qt headers may rely (or just take advantage) on it.

The build is supported and was tested on major systems: Windows, Linux, and macOS.

CMake is used as a build system, so build it as any usual CMake-based project:

```bash
cmake -B build -D CMAKE_BUILD_TYPE=Release .
cmake --build build
```

Adjust the commands given above depending on the environment (like choosing CMake generator or passing a path to Qt for example) or your preferences/needs (debug/release build for example).

By default, both targets are compiled. As result, you will get a static library containing dialog implementation and executable. If you want to build only the library, just specify the target to `cmake build` command:

```bash
cmake --build build --target gradient_dialog
```

But actually, it costs nothing to build a demo application too, because it is only one source file containing just a few lines (~15) of code.

Usage
-----

The primary header you are interested in is `gradient_dialog.h`, it contains all declarations required to start using it. A dialog can be created in the "classic way" as any other Qt dialog (see demo application for example), easy to use static function is also provided for convenience (like other dialogs do). Examples below.

1. the "classic way" - creating and configuring an object:

   ```cpp
   #include "gradient_dialog.h"

   // somewhere in some event handler (actionOpen in this example)
   void MainWindow::on_actionOpen_triggered()
   {
       // create the dialog itself
       GradientDialog* dlg = new GradientDialog(this);
       // create some initial gradient
       QConicalGradient g(0.5, 0.5, 45.0);
       g.setStops({
           {0.00, {170,   0,   0}},  // #aa0000
           {0.20, {  0,  85, 255}},  // #0055ff
           {0.45, {  0, 170,   0}},  // #00aa00
           {0.65, {255, 255,   0}},  // #ffff00
           {1.00, {170,   0,   0}},  // #aa0000
       });
       // init dialog with gradient
       dlg->setGradient(g);
       // show the dialog
       dlg->open();
   }
   ```

2. convenient way using the static function:

   ```cpp
   #include "gradient_dialog.h"

   // somewhere in some event handler (actionOpen in this example)
   void MainWindow::on_actionOpen_triggered()
   {
       // unfortunately there is no such thing like "invalid gradient"
       // so we need some kind of indicator is dialog was accepted
       bool ok = false;
       // create some initial gradient
       QConicalGradient g(0.5, 0.5, 45.0);
       g.setStops({
           {0.00, {170,   0,   0}},  // #aa0000
           {0.20, {  0,  85, 255}},  // #0055ff
           {0.45, {  0, 170,   0}},  // #00aa00
           {0.65, {255, 255,   0}},  // #ffff00
           {1.00, {170,   0,   0}},  // #aa0000
       });
       // create and show gradient dialog
       QGradient res = GradientDialog::getGradient(&ok, g, this);
       if (ok) {
           // dialog was closed with "OK",
           // it is safe to use 'res', it has valid value
       }
   }
   ```

The first option is more flexible. It is asynchronous and allows to subscribe to `gradientChanged()` signal and track gradient changes on the fly (for example for a live preview of customizing component). The second one is for the sake of simplicity (note the similar syntax compared to `QColorDialog::getColor()`) and it is synchronous (i.e. blocking call).

Final notes
-----------

Returned `QGradient` object has a specific option set

```cpp
m_gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
```

so, to use it in your code you may need to change this option or adjust your code. Read more about this option in Qt documentation.
