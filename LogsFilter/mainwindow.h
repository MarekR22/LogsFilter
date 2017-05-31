#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logsformatconfiguration.h"

namespace Ui {
class MainWindow;
}

class LogsDataModel;

QT_FORWARD_DECLARE_CLASS(QFileDialog)
QT_FORWARD_DECLARE_CLASS(QSortFilterProxyModel)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~MainWindow();
    explicit MainWindow(QWidget *parent = 0);

private slots:

    void on_actionLoad_Config_triggered();

    void on_actionAppendLogs_triggered();
    
    void updateProxyFilter();

    void on_actionCaseSensitiveFilter_toggled(bool arg1);

    void on_actionRegExpFilter_toggled(bool arg1);

    void on_actionChangeLogsTimeZone_triggered();

private:
    static void removeRegularExpresionEscape(QString &filterText);

private:
    Ui::MainWindow *ui;
    LogsDataModel *m_logModel;
    QSortFilterProxyModel *m_proxy;
    LogsFormatConfiguration m_logConfig;
};

#endif // MAINWINDOW_H
