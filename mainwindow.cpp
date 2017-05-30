#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logsdatamodel.h"

#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QErrorMessage>
#include <QSettings>
#include <QDebug>
#include <QRegExp>

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_logModel = new LogsDataModel(this);
    m_proxy = new QSortFilterProxyModel(this);
    m_proxy->setSourceModel(m_logModel);
    m_proxy->setDynamicSortFilter(false);
    m_proxy->setFilterKeyColumn(2);
    ui->tableView->setModel(m_proxy);

    connect(ui->actionClearLogs, &QAction::triggered,
            m_logModel, &LogsDataModel::clear);

    connect(ui->filterLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::updateProxyFilter);

    on_actionCaseSensitiveFilter_toggled(ui->actionCaseSensitiveFilter->isChecked());
}

void MainWindow::on_actionLoad_Config_triggered()
{
    static const QString lastDirKey("lastConfigDirectoryKey");

    QFileDialog dialog(this, tr("Load config"));

    QSettings settings;
    dialog.setDirectory(settings.value(lastDirKey).toString());

    if (dialog.exec()) {
        settings.setValue(lastDirKey, dialog.directory().absolutePath());

        auto files = dialog.selectedFiles();
        auto newConfig = LogsFormatConfiguration {};
        if (newConfig.loadFromJsonFileWithName(files.first())) {
            m_logConfig = newConfig;
        } else {
            QErrorMessage::qtHandler()->showMessage(tr("Failed to load config file: %1").arg(files.first()));
        }
    }
}

void MainWindow::on_actionAppendLogs_triggered()
{
    static const QString lastDirKey("lastLogsDirectoryKey");

    QFileDialog dialog(this, tr("Load logs"));

    QSettings settings;
    dialog.setDirectory(settings.value(lastDirKey).toString());

    if (dialog.exec()) {
        auto files = dialog.selectedFiles();
        for (const auto fileName : files) {
            if (!m_logModel->LoadLogs(fileName, m_logConfig)) {
                QErrorMessage::qtHandler()->showMessage(tr("Failed to open file: %1").arg(fileName));
            }
        }

        settings.setValue(lastDirKey, dialog.directory().absolutePath());
    }
}

void MainWindow::updateProxyFilter()
{
    auto filterText = ui->filterLineEdit->text();

    if (ui->actionRegExpFilter->isChecked()) {
        QRegExp re(filterText);
        if (re.isValid()) {
            m_proxy->setFilterRegExp(re);
        }
    } else {
        m_proxy->setFilterFixedString(filterText);
    }
}

void MainWindow::on_actionCaseSensitiveFilter_toggled(bool caseSensitive)
{
    m_proxy->setFilterCaseSensitivity(caseSensitive?Qt::CaseSensitive:Qt::CaseInsensitive);
}

void MainWindow::removeRegularExpresionEscape(QString &filterText)
{
    static QRegExp removeEscape(R"(\\([\\*.()?|]))");
    filterText.replace(removeEscape, "\\1");
}

void MainWindow::on_actionRegExpFilter_toggled(bool useRE)
{
    auto filterText = ui->filterLineEdit->text();
    if (useRE) {
        ui->filterLineEdit->setText(QRegExp::escape(filterText));
    } else {
        removeRegularExpresionEscape(filterText);
        ui->filterLineEdit->setText(filterText);
    }
}
