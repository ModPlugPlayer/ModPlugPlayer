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
