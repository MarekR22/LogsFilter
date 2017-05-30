#include "timezonetool.h"

TimeZoneTool::TimeZoneTool()
{
}

QTimeZone TimeZoneTool::timeZoneFromString(const QString &s)
{
    bool success = false;
    auto hoursDiff = s.toDouble(&success);
    if (success) {
        return timeZoneFromTimeShift(hoursDiff);
    }

    auto possibleZones = QTimeZone::availableTimeZoneIds();

    auto zoneNameArray = s.toLower().toUtf8();

    auto matchedIanald = std::find_if(possibleZones.begin(), possibleZones.end(),
                                      [zoneNameArray](const QByteArray &ianald) {
        auto asString = QString::fromUtf8(ianald);
        return asString.endsWith(zoneNameArray, Qt::CaseInsensitive);
    });

    if (matchedIanald != possibleZones.end()) {
        return QTimeZone { *matchedIanald };
    }
    return {};
}

QTimeZone TimeZoneTool::timeZoneFromTimeShift(double hoursShift)
{
    return QTimeZone { qRound(hoursShift * 60 * 60) };
}

