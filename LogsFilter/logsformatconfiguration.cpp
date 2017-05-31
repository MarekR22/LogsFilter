#include "logsformatconfiguration.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QFile>

namespace {
    const QString kDefaultRE { R"re((\d+-\d+-\d+ \d+:\d{2}:\d{2}.\d{3}) \[(\w+)\]\s*(.*))re" };
    const QString kDefaultTimeFormat { "yyyy-MM-dd HH:mm:ss.zzz" };
    const int kDefaultTimeCaptureIndex = 1;
    const int kDefaultTagCaptureIndex = 2;
    const int kDefaultLogTextCaptureIndex = 3;
}

LogsFormatConfiguration::~LogsFormatConfiguration()
{

}

LogsFormatConfiguration::LogsFormatConfiguration()
    : m_mainRegExp { kDefaultRE }
{
}

bool LogsFormatConfiguration::loadFromJsonFileWithName(const QString &fileName)
{
    QFile file { fileName };
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Failed to open file: " << fileName;
        return false;
    }
    return loadFromJsonDevice(&file);
}

bool LogsFormatConfiguration::loadFromJsonDevice(QIODevice *dev)
{
    auto data = dev->readAll();
    if (data.isEmpty()) {
        qDebug() << "File is empty";
        return false;
    }
    return loadFromJson(data);
}

bool LogsFormatConfiguration::loadFromJson(const QByteArray &data)
{
    QJsonParseError error;
    QJsonDocument json = QJsonDocument::fromJson(data, &error);

    if (json.isNull()) {
        qDebug() << "Parsing of json data filed: " << error.errorString();
        return false;
    }
    return loadFromJson(json);
}

bool LogsFormatConfiguration::loadFromJson(const QJsonDocument &jsonDoc)
{
    if (!jsonDoc.isObject()) {
        qDebug() << "Root of json must be an object";
        return false;
    }
    return loadFromJson(jsonDoc.object());
}

bool LogsFormatConfiguration::loadFromJson(const QJsonObject &jsonObject)
{
    QJsonValue jsonMainRegExp = jsonObject.value("regExp");
    QJsonValue jsonColumns    = jsonObject.value("columns");

    if (!jsonMainRegExp.isString()) {
        qDebug() << "regExp string is missing";
        return false;
    }

    if (!jsonColumns.isObject()) {
        qDebug() << "columns object is missing";
        return false;
    }

    auto entryFirstLineRE = QRegularExpression { jsonMainRegExp.toString() };
    if (!entryFirstLineRE.isValid()) {
        qDebug() << "Invalid reguler expresion in regExp";
        return false;
    }

    auto columnsObject = jsonColumns.toObject();
    for (auto valueRef : columnsObject) {
        if (!valueRef.isObject()) {
            qDebug() << "columns object is missing";
            return false;
        }
    }

    return true;
}
