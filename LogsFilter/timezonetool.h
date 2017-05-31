#ifndef TIMEZONETOOL_H
#define TIMEZONETOOL_H

#include <QTimeZone>

class TimeZoneTool
{
public:
    TimeZoneTool();

    static QTimeZone timeZoneFromString(const QString &s);
    static QTimeZone timeZoneFromTimeShift(double hoursShift);
};

#endif // TIMEZONETOOL_H
