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

    auto matchedIanald = std::find_if(possibleZones.begin(), possibleZones.end(),
                                      [s](const QByteArray &ianald) {
        auto rightLen = ianald.size() - ianald.lastIndexOf('/') - 1;
        auto asString = QString::fromUtf8(ianald.right(rightLen));
        asString.replace('_', ' ');
        return asString.endsWith(s, Qt::CaseInsensitive);
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

