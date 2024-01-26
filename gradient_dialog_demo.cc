/*
 * SPDX-FileCopyrightText: 2022 Nick Korotysh <nick.korotysh@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QApplication>
#include "gradient_dialog.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  GradientDialog dlg;
  QConicalGradient g(0.5, 0.5, 45.0);
  g.setStops({
    {0.00, {170,   0,   0}},  // #aa0000
    {0.20, {  0,  85, 255}},  // #0055ff
    {0.45, {  0, 170,   0}},  // #00aa00
    {0.65, {255, 255,   0}},  // #ffff00
    {1.00, {170,   0,   0}},  // #aa0000
  });
  dlg.setGradient(g);
  dlg.show();
  return app.exec();
}
