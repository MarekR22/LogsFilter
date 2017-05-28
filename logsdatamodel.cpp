#include "logsdatamodel.h"
#include "logsformatconfiguration.h"

#include <QTextStream>
#include <QFile>
#include <QRegularExpression>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

LogsDataModel::~LogsDataModel()
{
}

LogsDataModel::LogsDataModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_data.reserve(0x100000);
}

void LogsDataModel::LoadLogs(const QString &fileName, const LogsFormatConfiguration &config)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        LoadLogs(&file, config);
    }
}

void LogsDataModel::LoadLogs(QIODevice *device, const LogsFormatConfiguration &config)
{
    QTextStream stream(device);

    int lineNumber = 0;
    while(!stream.atEnd()) {
        auto line = stream.readLine();
        ProcessLine(line, ++lineNumber, config);
    }
}

int LogsDataModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_data.count();
}

int LogsDataModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return 3;
}

QVariant LogsDataModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return {};
    }

    auto entry = m_data.at(index.row());
    switch (index.column()) {
    case 0: {
        auto timeText = entry.time.toString("HH:mm:ss.zzz");
        return timeText;
    }
    case 1:
        return entry.tag;
    case 2:
        return entry.text;
    }
    return {};
}

QVariant LogsDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return {};
    }

    if (orientation != Qt::Vertical) {
        switch (section) {
        case 0:
            return tr("Time");
        case 1:
            return tr("Tag");
        case 2:
            return tr("Log");
        }
    } else {
        if (section < m_data.size()) {
            return m_data.at(section).lineNumber;
        }
    }
    return {};
}

void LogsDataModel::ProcessLine(const QString &line, int lineNumber, const LogsFormatConfiguration &config)
{
    if (line.isEmpty()) {
        return;
    }
    auto result = config.entryFirstLineRegExp().match(line);

    if (result.hasMatch()) {
        LogEntry entry {
            lineNumber,
            QDateTime::fromString(result.captured(config.timeCaptureIndex()), config.timeFormat()),
            result.captured(config.tagsCaptureIndex()),
            result.captured(config.textCaptureIndex())
        };
        auto newRow = m_data.size();
        beginInsertRows({}, newRow, newRow);
        m_data.append(std::move(entry));
        endInsertRows();
    } else {
        if (m_data.empty()) {
            beginInsertRows({}, 0, 0);
            m_data.append({ lineNumber, {}, {}, {} });
            endInsertRows();
        }
        m_data.back().text.append('\n').append(line);
        auto changedIndex = this->index(2, m_data.size() - 1);
        dataChanged(changedIndex, changedIndex, { Qt::DisplayRole });
    }
}
