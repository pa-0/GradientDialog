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

#include "gradient_stops_edit.h"

#include <QColorDialog>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define position localPos
#endif

inline bool cmp_pos(const QGradientStop& a, const QGradientStop& b)
{
  return a.first < b.first;
}

static QPixmap create_background()
{
  QPixmap pxm(8, 8);
  pxm.fill(Qt::lightGray);

  QPainter p(&pxm);
  QRect qp(0, 0, pxm.width() / 2, pxm.height() / 2);
  p.fillRect(qp, Qt::gray);
  p.fillRect(qp.translated(qp.width(), qp.height()), Qt::gray);

  return pxm;
}

GradientStopsEdit::GradientStopsEdit(const QGradientStops& stops, QWidget* parent)
  : QWidget(parent)
  , m_background(create_background())
  , m_stops(stops)
  , m_gradient(100, 1, QImage::Format_ARGB32)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  if (m_stops.isEmpty()) {
    m_stops.append({0, Qt::black});
    m_stops.append({0.25, Qt::yellow});
    m_stops.append({0.75, Qt::red});
    m_stops.append({1, Qt::white});
  }

  updateImage();
}

QSize GradientStopsEdit::sizeHint() const
{
  return minimumSizeHint();
}

QSize GradientStopsEdit::minimumSizeHint() const
{
  return QSize(160, 20);
}

void GradientStopsEdit::setStops(const QGradientStops& stops)
{
  m_stops = stops;
  updateImage();
  update();
  emit stopsChanged(m_stops);
}

void GradientStopsEdit::mousePressEvent(QMouseEvent* event)
{
  int stop_index = findStopIndex(event->position().x());
  // do not allow to select first and last stops
  if (stop_index != 0 && stop_index != m_stops.size() - 1)
    m_stop_index = stop_index;

  // create new stop on left button click
  if (stop_index == -1 && event->button() == Qt::LeftButton) {
    addStop(event->position().x() / width());
    update();
  }

  event->accept();
}

void GradientStopsEdit::mouseMoveEvent(QMouseEvent* event)
{
  if ((event->buttons() & Qt::LeftButton) && (m_stop_index != -1) &&
      (4 < event->position().x()) && (event->position().x() < width() - 4)) {
    m_stops[m_stop_index].first = event->position().x() / width();
    updateImage();
    update();
    emit stopsChanged(m_stops);
  }

  event->accept();
}

void GradientStopsEdit::mouseReleaseEvent(QMouseEvent* event)
{
  if (m_stop_index != -1) {
    if (event->button() == Qt::LeftButton)
      std::sort(m_stops.begin(), m_stops.end(), cmp_pos);

    if (event->button() == Qt::RightButton)
      removeStop(m_stop_index);

    m_stop_index = -1;
    update();
  }

  event->accept();
}

void GradientStopsEdit::mouseDoubleClickEvent(QMouseEvent* event)
{
  int idx = findStopIndex(event->position().x());
  if (idx != -1) {
    QColor color = QColorDialog::getColor(
                     m_stops[idx].second,
                     window(), QString(),
                     QColorDialog::ShowAlphaChannel);

    if (color.isValid()) {
      m_stops[idx].second = color;
      updateImage();
      update();
      emit stopsChanged(m_stops);
    }
  }

  event->accept();
}

void GradientStopsEdit::paintEvent(QPaintEvent* event)
{
  QPainter p(this);

  QLinearGradient g(0, 0, width(), 0);
  g.setStops(m_stops);

  p.setPen(Qt::NoPen);
  p.setBrush(m_background);

  p.drawRect(rect().adjusted(0, 2, 0, -2));

  p.setPen(palette().color(QPalette::Dark));
  p.setBrush(g);

  p.drawRect(rect().adjusted(0, 2, 0, -2));

  p.setPen(Qt::gray);
  p.setBrush(QColor(255, 255, 255, 180));

  for (auto& s : qAsConst(m_stops))
    p.drawRect(QRect(-4, 0, 8, height()).translated(s.first * width(), 0));

  event->accept();
}

int GradientStopsEdit::findStopIndex(qreal xpos) const
{
  for (int i = 0; i < m_stops.size(); ++i) {
    qreal x = m_stops[i].first * width();
    constexpr const int dx = 4;
    if (x - dx < xpos && xpos < x + dx)
      return i;
  }

  return -1;
}

void GradientStopsEdit::addStop(qreal pos)
{
  m_stops.append({pos, pickColor(pos)});
  std::sort(m_stops.begin(), m_stops.end(), cmp_pos);
  updateImage();
  emit stopsChanged(m_stops);
}

void GradientStopsEdit::removeStop(int idx)
{
  m_stops.removeAt(idx);
  updateImage();
  emit stopsChanged(m_stops);
}

QColor GradientStopsEdit::pickColor(qreal pos) const
{
  return m_gradient.pixelColor(qRound(pos * m_gradient.width()), 0);
}

void GradientStopsEdit::updateImage()
{
  m_gradient.fill(Qt::transparent);
  QPainter p(&m_gradient);
  QLinearGradient g(0, 0, m_gradient.width(), 0);
  g.setStops(m_stops);
  p.setBrush(g);
  p.setPen(Qt::NoPen);
  p.drawRect(m_gradient.rect());
}
