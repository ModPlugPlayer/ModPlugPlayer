/*
VersionController class definitions of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "VersionController.hpp"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

Version VersionController::getVersionInfo() {
    Version version;

    QFile file(":/JSON/VersionInfo.json");  // qrc üzerinden erişim
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open versionInfo.json";
        return version;
    }

    QByteArray jsonData = file.readAll();
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return version;
    }

    QJsonObject jsonObject = jsonDocument.object();

    version.major = jsonObject.value("major").toInt(0);
    version.minor = jsonObject.value("minor").toInt(0);
    version.release = jsonObject.value("release").toInt(0);
    version.patch = jsonObject.value("patch").toInt(0);
    version.stageRevision = jsonObject.value("stageRevision").toInt(0);

    QString stage = jsonObject.value("stage").toString("StableRelease");

    if (stage == "PreAlpha") version.stage = Stage::PreAlpha;
    else if (stage == "Alpha") version.stage = Stage::Alpha;
    else if (stage == "Beta") version.stage = Stage::Beta;
    else if (stage == "ReleaseCandidate") version.stage = Stage::ReleaseCandidate;
    else version.stage = Stage::StableRelease;

    return version;
}
