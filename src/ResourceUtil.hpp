#pragma once
#include <QString>

class ResourceUtil
{
public:
    static 	QString getResourceContent(QString resource, bool *ok = nullptr);
};

