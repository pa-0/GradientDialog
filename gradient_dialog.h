/*
  gradient picker dialog
  Copyright (C) 2020  Nick Korotysh <nick.korotysh@gmail.com>

  This file is part of Qt gradient picker library.

  Qt gradient picker library is free software: you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of the License,
  or (at your option) any later version.

  Qt gradient picker library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
  for more details.

  You should have received a copy of the GNU Lesser General Public License along
  with Qt gradient picker library. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <QDialog>

namespace Ui {
class GradientDialog;
}

class GradientDialog : public QDialog
{
  Q_OBJECT

public:
  static QGradient getGradient(bool* ok,
                               const QGradient& gradient = QLinearGradient(),
                               QWidget* parent = nullptr,
                               const QString& title = QString());

  explicit GradientDialog(QWidget* parent = nullptr)
    : GradientDialog(QLinearGradient(), parent) {}
  explicit GradientDialog(const QGradient& gradient,
                          QWidget* parent = nullptr);
  ~GradientDialog();

  QGradient gradient() const;

public slots:
  void setGradient(const QGradient& gradient);

signals:
  void gradientChanged(const QGradient& gradient);

private slots:
  void on_type_linear_rbtn_clicked();
  void on_type_radial_rbtn_clicked();
  void on_type_conical_rbtn_clicked();

  void on_spread_pad_rbtn_clicked();
  void on_spread_repeat_rbtn_clicked();
  void on_spread_reflect_rbtn_clicked();

private:
  Ui::GradientDialog* ui;
};
