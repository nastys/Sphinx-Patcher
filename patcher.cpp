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

#include <patcher.h>

void writePatch(QByteArray &array, const unsigned int address_pal, QVector<unsigned char> bytes_pal, const unsigned int address_ntsc, QVector<unsigned char> bytes_ntsc, bool isPal)
{
    if(isPal) for(int i=0; i<bytes_pal.size(); i++) array[address_pal+i]=bytes_pal.at(i);
    else for(int i=0; i<bytes_ntsc.size(); i++) array[address_ntsc+i]=bytes_ntsc.at(i);
}

QVector<unsigned char> ratioToBytes(QString text, float defaultVal, float diff)
{
    float newRatio;
    bool ok=false;
    text.replace(',', '.');
    if(text.contains('.')) newRatio = text.toFloat(&ok);
    if(!ok)
    {
        text.replace('/', ':');
        if(text.contains(':'))
        {
            QStringList op = text.split(':');
            bool ok1, ok2;
            newRatio = op.at(0).toFloat(&ok1) / op.at(1).toFloat(&ok2);
            ok=ok1&&ok2;
        }
        if(!ok) newRatio = defaultVal;
    }
    newRatio-=diff;
    unsigned char* newRatioArray = reinterpret_cast<unsigned char*>(&newRatio);
    QVector<unsigned char> newRatioQVector;
    if(QSysInfo::ByteOrder==QSysInfo::BigEndian)
        for(unsigned char i=0; i<4; i--) newRatioQVector.append(newRatioArray[3-i]);
    else
        for(unsigned char i=0; i<4; i++) newRatioQVector.append(newRatioArray[i]);

    return newRatioQVector;
}
