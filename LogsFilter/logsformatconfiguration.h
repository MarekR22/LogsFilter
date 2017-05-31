#ifndef LOGSFORMATCONFIGURATION_H
#define LOGSFORMATCONFIGURATION_H

#include <QRegularExpression>
#include <QVector>

QT_FORWARD_DECLARE_CLASS(QIODevice)

class ColumnConfiguration
{
public:
    ColumnConfiguration(const QJsonObject& object);

    QString Title() const {
        return m_title;
    }

    int captureIndex() const {
        return m_capture;
    }

private:
    QString m_title;
    int m_capture;
};

class TimeColumnConfiguration : public ColumnConfiguration
{
public:
    TimeColumnConfiguration(const QJsonObject& object);

    QString TimeFormat() const {
        return m_dateFormat;
    }

private:
    QString m_dateFormat;
};

class LogsFormatConfiguration
{
public:
    ~LogsFormatConfiguration();
    LogsFormatConfiguration();

    bool loadFromJsonFileWithName(const QString &fileName);
    bool loadFromJsonDevice(QIODevice     *dev);
    bool loadFromJson(const QByteArray    &data);
    bool loadFromJson(const QJsonDocument &jsonDoc);
    bool loadFromJson(const QJsonObject   &jsonObject);

    QRegularExpression mainRegExp() const
    {
        return m_mainRegExp;
    }

private:
    QRegularExpression m_mainRegExp;
    QVector<ColumnConfiguration> m_Columns;
};

#endif // LOGSFORMATCONFIGURATION_H
