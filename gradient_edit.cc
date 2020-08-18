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

#include "gradient_edit.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

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

GradientEdit::GradientEdit(const QGradient& gradient, QWidget* parent)
  : QWidget(parent)
  , m_background(create_background())
  , m_gradient(gradient)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  m_points.append({0.25, 0.75});
  m_points.append({0.75, 0.25});

  m_gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
  updateGradient();
}

QSize GradientEdit::sizeHint() const
{
  return minimumSizeHint();
}

QSize GradientEdit::minimumSizeHint() const
{
  return QSize(120, 120);
}

void GradientEdit::setGradient(const QGradient& gradient)
{
  m_gradient = gradient;
  m_gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
  updatePoints();
  update();
  emit gradientChanged(m_gradient);
}

void GradientEdit::setGradientType(QGradient::Type type)
{
  QGradient gradient;

  switch (type) {
    case QGradient::LinearGradient:
      gradient = QLinearGradient();
      break;

    case QGradient::RadialGradient:
      gradient = QRadialGradient();
      break;

    case QGradient::ConicalGradient:
      gradient = QConicalGradient();
      break;

    default:
      break;
  }

  gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
  gradient.setSpread(m_gradient.spread());
  gradient.setStops(m_gradient.stops());
  m_gradient = gradient;
  updateGradient();
  update();
  emit gradientChanged(m_gradient);
}

void GradientEdit::setGradientSpread(QGradient::Spread spread)
{
  m_gradient.setSpread(spread);
  update();
  emit gradientChanged(m_gradient);
}

void GradientEdit::setGradientStops(const QGradientStops& stops)
{
  m_gradient.setStops(stops);
  update();
  emit gradientChanged(m_gradient);
}

void GradientEdit::mousePressEvent(QMouseEvent* event)
{
  for (int i = 0; i < m_points.size(); ++i) {
    QPointF p(m_points[i].x() * width(), m_points[i].y() * height());
    QLineF l(p, event->pos());
    if (l.length() < 5)
      m_selected_point = i;
  }
  event->accept();
}

void GradientEdit::mouseMoveEvent(QMouseEvent* event)
{
  if (m_selected_point != -1) {
    QPointF& p = m_points[m_selected_point];
    p.rx() = static_cast<qreal>(event->x()) / width();
    p.ry() = static_cast<qreal>(event->y()) / height();
    updateGradient();
    update();
    emit gradientChanged(m_gradient);
  }
  event->accept();
}

void GradientEdit::mouseReleaseEvent(QMouseEvent* event)
{
  m_selected_point = -1;
  event->accept();
}

void GradientEdit::paintEvent(QPaintEvent* event)
{
  QPainter p(this);

  p.setPen(palette().color(QPalette::Dark));
  p.setBrush(m_background);
  p.setCompositionMode(QPainter::CompositionMode_Source);

  p.drawRect(rect());

  p.setBrush(m_gradient);
  p.setCompositionMode(QPainter::CompositionMode_SourceOver);

  p.drawRect(rect());

  p.setPen(Qt::gray);
  p.setBrush(QColor(255, 255, 255, 180));

  p.setRenderHint(QPainter::Antialiasing);

  for (auto& i : qAsConst(m_points))
    p.drawEllipse(QPointF(i.x() * width(), i.y() * height()), 5, 5);

  event->accept();
}

void GradientEdit::updatePoints()
{
  m_points.clear();

  switch (m_gradient.type()) {
    case QGradient::LinearGradient: {
      const QLinearGradient& lg = static_cast<QLinearGradient&>(m_gradient);
      m_points.append(lg.start());
      m_points.append(lg.finalStop());
      break;
    }

    case QGradient::RadialGradient: {
      const QRadialGradient& rg = static_cast<QRadialGradient&>(m_gradient);
      m_points.append(rg.center());
      m_points.append(rg.focalPoint());
      break;
    }

    case QGradient::ConicalGradient: {
      const QConicalGradient& cg = static_cast<QConicalGradient&>(m_gradient);
      m_points.append(cg.center());
      m_points.append(cg.center());
      m_points.last().rx() += 0.1 * cos(cg.angle() * M_PI / 180);
      m_points.last().ry() -= 0.1 * sin(cg.angle() * M_PI / 180);
      break;
    }

    default:
      m_points.append({0.25, 0.75});
      m_points.append({0.75, 0.25});
      break;
  }
}

void GradientEdit::updateGradient()
{
  switch (m_gradient.type()) {
    case QGradient::LinearGradient: {
      QLinearGradient& lg = static_cast<QLinearGradient&>(m_gradient);
      lg.setStart(m_points.at(0));
      lg.setFinalStop(m_points.at(1));
      break;
    }

    case QGradient::RadialGradient: {
      QRadialGradient& rg = static_cast<QRadialGradient&>(m_gradient);
      rg.setCenter(m_points.at(0));
      rg.setFocalPoint(m_points.at(1));
      break;
    }

    case QGradient::ConicalGradient: {
      QConicalGradient& cg = static_cast<QConicalGradient&>(m_gradient);
      cg.setCenter(m_points.at(0));
      QLineF l(m_points.at(0), m_points.at(1));
      cg.setAngle(l.angle());
      break;
    }

    default:
      break;
  }
}
