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

#ifndef GRADIENT_EDIT_H
#define GRADIENT_EDIT_H

#include <QWidget>

class GradientEdit : public QWidget
{
  Q_OBJECT

public:
  explicit GradientEdit(QWidget* parent = nullptr)
    : GradientEdit(QLinearGradient(), parent) {}
  explicit GradientEdit(const QGradient& gradient, QWidget* parent = nullptr);

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

  bool hasHeightForWidth() const override { return true; }
  int heightForWidth(int w) const override { return w; }

  inline const QGradient& gradient() const noexcept { return m_gradient; }

public slots:
  void setGradient(const QGradient& gradient);

  void setGradientType(QGradient::Type type);
  void setGradientSpread(QGradient::Spread spread);
  void setGradientStops(const QGradientStops& stops);

signals:
  void gradientChanged(const QGradient& gradient);

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  void paintEvent(QPaintEvent* event) override;

private:
  QBrush m_background;
  QGradient m_gradient;

private:
  void updatePoints();
  void updateGradient();

private:
  QVector<QPointF> m_points;
  int m_selected_point = -1;
};

#endif // GRADIENT_EDIT_H
