#ifndef LOGSDATAMODEL_H
#define LOGSDATAMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>

QT_FORWARD_DECLARE_CLASS(QIODevice)
QT_FORWARD_DECLARE_CLASS(QRegularExpression)

class LogsFormatConfiguration;

class LogsDataModel : public QAbstractTableModel
{
public:
    ~LogsDataModel();
    explicit LogsDataModel(QObject *parent = nullptr);

    bool LoadLogs(const QString &fileName, const LogsFormatConfiguration &config);
    void LoadLogs(QIODevice * device, const LogsFormatConfiguration &config);

    auto rowCount(const QModelIndex &parent) const -> int;
    auto columnCount(const QModelIndex &parent) const -> int;
    auto data(const QModelIndex &index, int role) const -> QVariant;
    auto headerData(int section, Qt::Orientation orientation, int role) const -> QVariant;

signals:

public slots:
    void clear();

private:
    void ProcessLine(const QString &line, int lineNumber, const LogsFormatConfiguration &config);

private:
    struct LogEntry {
        int lineNumber;
        QDateTime time;
        QString tag;
        QString text;
    };

    QVector<LogEntry> m_data;
};

#endif // LOGSDATAMODEL_H
