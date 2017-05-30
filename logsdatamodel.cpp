#include "logsdatamodel.h"
#include "logsformatconfiguration.h"

#include <QTextStream>
#include <QFile>
#include <QRegularExpression>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

namespace {
enum Columns {
    kTimeColumn,
    kTagColumn,
    kLogColumn,

    kColumntCount
};
}

LogsDataModel::~LogsDataModel()
{
}

LogsDataModel::LogsDataModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_data.reserve(0x100000);
}

bool LogsDataModel::LoadLogs(const QString &fileName, const LogsFormatConfiguration &config)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        LoadLogs(&file, config);
        return true;
    }
    return false;
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
    return kColumntCount;
}

QVariant LogsDataModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return {};
    }

    auto entry = m_data.at(index.row());
    switch (index.column()) {
    case kTimeColumn: {
        auto timeText = entry.time.toString("HH:mm:ss.zzz");
        return timeText;
    }
    case kTagColumn:
        return entry.tag;

    case kLogColumn:
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
        case kTimeColumn:
            return tr("Time");
        case kTagColumn:
            return tr("Tag");
        case kLogColumn:
            return tr("Log");
        }
    } else {
        if (section < m_data.size()) {
            return m_data.at(section).lineNumber;
        }
    }
    return {};
}

void LogsDataModel::clear()
{
    if (m_data.empty()) {
        return;
    }
    beginRemoveRows({}, 0, m_data.size() - 1);
    m_data.clear();
    endRemoveRows();
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
        auto changedIndex = this->index(kLogColumn, m_data.size() - 1);

        emit dataChanged(changedIndex, changedIndex, { Qt::DisplayRole });
    }
}
