#ifndef LOGSFORMATCONFIGURATION_H
#define LOGSFORMATCONFIGURATION_H

#include<QRegularExpression>

QT_FORWARD_DECLARE_CLASS(QIODevice)

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

    QRegularExpression entryFirstLineRegExp() const
    {
        return m_entryFirstLineRE;
    }

    int timeCaptureIndex() const
    {
        return m_timeCaptureIndex;
    }

    int tagsCaptureIndex() const
    {
        return m_tagsCaptureIndex;
    }

    int textCaptureIndex() const
    {
        return m_textCaptureIndex;
    }

    QString timeFormat() const
    {
        return m_timeFormat;
    }

private:
    QRegularExpression m_entryFirstLineRE;
    int m_timeCaptureIndex;
    int m_tagsCaptureIndex;
    int m_textCaptureIndex;
    QString m_timeFormat;
};

#endif // LOGSFORMATCONFIGURATION_H
