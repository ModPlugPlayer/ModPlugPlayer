/*
ResourceUtil class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "ResourceUtil.hpp"
#include <QFile>
#include <QIODevice>

QString ResourceUtil::
    getResourceContent(QString resource, bool *ok)
{
    QString data;

    QFile file(resource);
    if(!file.open(QIODevice::ReadOnly)) {
        if(ok != nullptr)
            *ok = false;
    }
    else
    {
        data = file.readAll();
        if(ok != nullptr)
           *ok = true;
    }

    file.close();
    return data;
}
