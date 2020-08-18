/*
  QGradient QVariant integration (including serialization)
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

#include "gradient_variant.h"

QDataStream& operator<<(QDataStream& out, const QGradient& gradient)
{
  out << static_cast<quint8>(gradient.type());
  out << static_cast<quint8>(gradient.spread());
  out << static_cast<quint8>(gradient.coordinateMode());
  out << gradient.stops();

  switch (gradient.type()) {
    case QGradient::LinearGradient: {
      const auto& g = static_cast<const QLinearGradient&>(gradient);
      out << g.start() << g.finalStop();
      break;
    }

    case QGradient::RadialGradient: {
      const auto& g = static_cast<const QRadialGradient&>(gradient);
      out << g.center() << g.radius() << g.focalPoint();
      break;
    }

    case QGradient::ConicalGradient: {
      const auto& g = static_cast<const QConicalGradient&>(gradient);
      out << g.center() << g.angle();
      break;
    }

    default:
      break;
  }

  return out;
}

QDataStream& operator>>(QDataStream& in, QGradient& gradient)
{
  quint8 type, spread, cm;
  QGradientStops stops;
  in >> type >> spread >> cm >> stops;

  switch (static_cast<QGradient::Type>(type)) {
    case QGradient::LinearGradient: {
      QPointF start, stop;
      in >> start >> stop;
      gradient = QLinearGradient(start, stop);
      break;
    }

    case QGradient::RadialGradient: {
      QPointF center, fp;
      qreal radius;
      in >> center >> radius >> fp;
      gradient = QRadialGradient(center, radius, fp);
      break;
    }

    case QGradient::ConicalGradient: {
      QPointF center;
      int angle;
      in >> center >> angle;
      gradient = QConicalGradient(center, angle);
      break;
    }

    default:
      break;
  }

  gradient.setSpread(static_cast<QGradient::Spread>(spread));
  gradient.setCoordinateMode(static_cast<QGradient::CoordinateMode>(cm));
  gradient.setStops(stops);

  return in;
}
