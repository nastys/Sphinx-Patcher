/*
 * Copyright © 2021 nastys
 *
 * This file is part of Sphinx Patcher.
 * Sphinx Patcher is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sphinx Patcher is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sphinx Patcher.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PATCHER_H
#define PATCHER_H

#include <QFile>
#include <QVector>

void writePatch(QByteArray &array, const unsigned int address_pal, QVector<unsigned char> bytes_pal, const unsigned int address_ntsc, QVector<unsigned char> bytes_ntsc, bool isPal);
QVector<unsigned char> ratioToBytes(QString text, float defaultVal, float diff=0.0);

#endif // PATCHER_H
