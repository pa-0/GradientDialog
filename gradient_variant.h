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

#ifndef GRADIENT_VARIANT_H
#define GRADIENT_VARIANT_H

#include <QGradient>
#include <QVariant>

QDataStream& operator<<(QDataStream& out, const QGradient& gradient);
QDataStream& operator>>(QDataStream& in, QGradient& gradient);

Q_DECLARE_METATYPE(QGradient)

Q_DECLARE_METATYPE(QLinearGradient)
Q_DECLARE_METATYPE(QRadialGradient)
Q_DECLARE_METATYPE(QConicalGradient)

#endif // GRADIENT_VARIANT_H
