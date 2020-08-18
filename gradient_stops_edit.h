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

#ifndef GRADIENT_STOPS_EDIT_H
#define GRADIENT_STOPS_EDIT_H

#include <QWidget>

class GradientStopsEdit : public QWidget
{
  Q_OBJECT

public:
  explicit GradientStopsEdit(QWidget* parent = nullptr)
    : GradientStopsEdit(QGradientStops(), parent) {}
  explicit GradientStopsEdit(const QGradientStops& stops,
                             QWidget* parent = nullptr);

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

  inline const QGradientStops& stops() const noexcept { return m_stops; }

public slots:
  void setStops(const QGradientStops& stops);

signals:
  void stopsChanged(const QGradientStops& stops);

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseDoubleClickEvent(QMouseEvent* event) override;

  void paintEvent(QPaintEvent* event) override;

private:
  QBrush m_background;
  QGradientStops m_stops;

private:
  int findStopIndex(int xpos) const;

  void addStop(qreal pos);
  void removeStop(int idx);

  QColor pickColor(qreal pos) const;
  void updateImage();

private:
  int m_stop_index = -1;
  QImage m_gradient;
};

#endif // GRADIENT_STOPS_EDIT_H
