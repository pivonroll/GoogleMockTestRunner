#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "testresultsparser.h"
#include "jtftestrunworker.h"
#include "addenvvartotrack.h"
#include "listofenvironmentvarstotrack.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QProcessEnvironment>

const char ALL_TESTS_STRING[] = "All";
const char EXECUTABLE_SETTINGS[] = "ExecutableSettings";
const char EXECUTABLE_SETTINGS_LIST[] = "ExecutableSettingsList";
const char ENVIRONMENT_VARIBLES_TO_TRACK[] = "EnvironmentVariablesToTrack";
const char ENVIRONMENT_VARIBLES[] = "EnvironmentVariables";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::setOrganizationName("My");
    QApplication::setOrganizationDomain("@none_important.who.cares");
    QApplication::setApplicationName("Gmock Test Runner");
    setWindowTitle("Gmock Test Runner");

    initialize();

    m_envVarToTrack = new QAction(tr("Add Environment Variable to Track"), this);
    ui->menuFile->addAction(m_envVarToTrack);

    m_listOfEnvVarsToTrack = new QAction(tr("List of Environment Variables to Track"), this);
    ui->menuFile->addAction(m_listOfEnvVarsToTrack);

    m_saveAction = new QAction(tr("Save"), this);
    ui->menuFile->addAction(m_saveAction);

    createConnections();
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAddJTFExeButtonClicked()
{
    if (ui->m_exeLineEdit->text().isEmpty())
        return;

    QFileInfo info(ui->m_exeLineEdit->text());

    if (!info.exists()) {
        showExecutableNotFoundMessage(ui->m_exeLineEdit->text());
        return;
    }

    ui->m_selectJTFExeComboBox->addItem(ui->m_exeLineEdit->text());
}

void MainWindow::onBrowseButtonClicked()
{
    QString canonicalPath = QFileDialog::getOpenFileName(this, tr("Select a JTF test executable"), QString(), QString("*.exe"));

    if (canonicalPath.isEmpty())
        return;

    ui->m_exeLineEdit->setText(canonicalPath);
}

void MainWindow::onRecheckForAvailableTestsClicked()
{
    if (ui->m_selectJTFExeComboBox->currentText().isEmpty())
        return;

    startScanForAvailableJTFTests(ui->m_selectJTFExeComboBox->currentText());
}

void MainWindow::onRunButtonClicked()
{
    if (ui->m_selectJTFExeComboBox->currentText().isEmpty() || ui->m_jtfTestChooseComboBox->currentText().isEmpty())
        return;

    runJTFTest(ui->m_selectJTFExeComboBox->currentText(), ui->m_jtfTestChooseComboBox->currentText());
}

void MainWindow::onExecutableChanged(QString canonicalExePath)
{
    if (canonicalExePath.isEmpty())
        return;

    startScanForAvailableJTFTests(canonicalExePath);
}

void MainWindow::onJTFTestScanCompleted(QString canonicalExePath, QList<JtfTest> tests)
{
    disableWidgets(false);

    if (ui->m_selectJTFExeComboBox->currentText() == canonicalExePath) {
        ui->m_jtfTestChooseComboBox->clear();
        fillJTFComboBox(tests);
    }
}

void MainWindow::onExeLineEditTextChanged(QString text)
{
    ui->m_addJTFExePushButton->setEnabled(!text.isEmpty());
}

void MainWindow::onMenuSaveClicked()
{
    saveSettings();
}

void MainWindow::onRemovePushButtonClicked()
{
    if (ui->m_selectJTFExeComboBox->count() > 0) {
        ui->m_selectJTFExeComboBox->removeItem(ui->m_selectJTFExeComboBox->currentIndex());

        if (ui->m_selectJTFExeComboBox->count() == 0)
            disableWidgets(true);
    }
}

void MainWindow::onAddEnvVarToTrack()
{
    AddEnvVarToTrack *dialog = new AddEnvVarToTrack(this);

    if (dialog->exec() == QDialog::Accepted) {
        QString varToTrack = dialog->varibleToTrack();
        if (!varToTrack.isEmpty())
            m_environmentVariablesToTrack.insert(varToTrack, readEnvVar(varToTrack));

    }

    dialog->deleteLater();
}

void MainWindow::onListEnvVarToTrack()
{
    ListOfEnvironmentVarsToTrack *dialog = new ListOfEnvironmentVarsToTrack(this);

    QMapIterator<QString, QVariant> it(m_environmentVariablesToTrack);

    while (it.hasNext()) {
        it.next();
        dialog->addRow(it.key(), it.value().toString());
    }

    dialog->exec(); // blocks
    dialog->deleteLater();
}

void MainWindow::onJTFTestCompleted(const QString result)
{
    TestResultsParser parser;

    TestResult testResult = parser.parse(result);
    ui->m_outputTextEdit->append(testResult.toString());
    ui->m_rawOutputTextEdit->setText(result);
    ui->m_errorLinesTextEdit->append(testResult.errorLinesToString());

    QString styleSheet;

    if (testResult.m_isPassed)
        styleSheet.append("QFrame \n { \n border-image: url(:/icons/Green); \n }");
    else
        styleSheet.append("QFrame \n { \n border-image: url(:/icons/Red); \n}");

    ui->m_testIndicatorIconFrame->setStyleSheet(styleSheet);
    ui->m_runPushButton->setEnabled(true);
}

void MainWindow::onJTFTestError()
{
    ui->m_outputTextEdit->append(QString("Error while executing!"));
    ui->m_runPushButton->setEnabled(true);
}

void MainWindow::initialize()
{
    ui->m_exeLineEdit->clear();
    ui->m_addJTFExePushButton->setDisabled(true);
    disableWidgets(true);
}

void MainWindow::loadSettings()
{
    QSettings settings;
    QStringList executableList;
    QStringList executablesNotFound;
    QStringList executablesFound;

    settings.beginGroup(ENVIRONMENT_VARIBLES_TO_TRACK);
    m_environmentVariablesToTrack = settings.value(ENVIRONMENT_VARIBLES).toMap();
    settings.endGroup();

    settings.beginGroup(EXECUTABLE_SETTINGS);
    executableList = settings.value(EXECUTABLE_SETTINGS_LIST).toStringList();
    settings.endGroup();

    ui->m_selectJTFExeComboBox->disconnect();

    foreach (const QString &exe, executableList) {
        QFileInfo info(exe);

        if (!info.exists())
            executablesNotFound << exe;
        else {
            ui->m_selectJTFExeComboBox->addItem(exe);
            executablesFound << exe;
        }
    }

    connect(ui->m_selectJTFExeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onExecutableChanged(QString)));

    if (executablesNotFound.size() > 0) {
        if (showExecutableNotFoundMessage(executablesNotFound.join("\n"), true) == QMessageBox::Yes) {
            settings.beginGroup(EXECUTABLE_SETTINGS);
            settings.setValue(EXECUTABLE_SETTINGS_LIST, QVariant(executablesFound));
            settings.endGroup();
        }
    }

    if (ui->m_selectJTFExeComboBox->count() > 0) {
        disableWidgets(false);
        ui->m_selectJTFExeComboBox->setCurrentIndex(0);

        startScanForAvailableJTFTests(ui->m_selectJTFExeComboBox->currentText());
    }
}

void MainWindow::saveSettings()
{
    QSettings settings;

    settings.beginGroup(ENVIRONMENT_VARIBLES_TO_TRACK);
    settings.setValue(ENVIRONMENT_VARIBLES, QVariant(m_environmentVariablesToTrack));
    settings.endGroup();

    QStringList executableList;

    for (int i = 0;  i < ui->m_selectJTFExeComboBox->count(); ++i)
        executableList << ui->m_selectJTFExeComboBox->itemText(i);

    settings.beginGroup(EXECUTABLE_SETTINGS);
    settings.setValue(EXECUTABLE_SETTINGS_LIST, QVariant(executableList));
    settings.endGroup();
}

void MainWindow::createConnections()
{
    connect(ui->m_addJTFExePushButton, SIGNAL(clicked()), this, SLOT(onAddJTFExeButtonClicked()));
    connect(ui->m_browsePushButton, SIGNAL(clicked()), this, SLOT(onBrowseButtonClicked()));
    connect(ui->m_recheckForTestsPushButton, SIGNAL(clicked()), this, SLOT(onRecheckForAvailableTestsClicked()));
    connect(ui->m_runPushButton, SIGNAL(clicked()), this, SLOT(onRunButtonClicked()));
    connect(ui->m_selectJTFExeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onExecutableChanged(QString)));
    connect(ui->m_exeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onExeLineEditTextChanged(QString)));
    connect(&m_jtftestScanner, SIGNAL(finished(QString,QList<JtfTest>)), this, SLOT(onJTFTestScanCompleted(QString, QList<JtfTest>)));
    connect(m_saveAction, SIGNAL(triggered()), this, SLOT(onMenuSaveClicked()));
    connect(ui->m_removePushButton, SIGNAL(clicked()), this, SLOT(onRemovePushButtonClicked()));
    connect(m_envVarToTrack, SIGNAL(triggered()), this, SLOT(onAddEnvVarToTrack()));
    connect(m_listOfEnvVarsToTrack, SIGNAL(triggered()), this, SLOT(onListEnvVarToTrack()));
}

void MainWindow::runJTFTest(const QString &canonicalExePath, const QString &test)
{
    QFileInfo info(canonicalExePath);

    if (!info.exists()) {
        showExecutableNotFoundMessage(canonicalExePath);
        return;
    }

    ui->m_outputTextEdit->clear();
    ui->m_rawOutputTextEdit->clear();
    ui->m_errorLinesTextEdit->clear();
    ui->m_outputTextEdit->setEnabled(true);
    ui->m_rawOutputTextEdit->setEnabled(true);
    ui->m_errorLinesTextEdit->setEnabled(true);

    if (canonicalExePath.isEmpty() || test.isEmpty())
        return;

    QStringList arguments;

    if (test != QString(ALL_TESTS_STRING))
        arguments << QString("--gtest_filter=%1*").arg(test);

    ui->m_runPushButton->setEnabled(false);
    ui->m_outputTextEdit->append(QString("Executing: %1 %2\n").arg(canonicalExePath).arg(arguments.join("")));

    JTFTestRunWorker *worker = new JTFTestRunWorker(canonicalExePath, arguments);
    connect(worker, SIGNAL(error()), this, SLOT(onJTFTestError()));
    connect(worker, SIGNAL(finished(QString)), this, SLOT(onJTFTestCompleted(QString)));
    worker->start();
}

void MainWindow::startScanForAvailableJTFTests(const QString &canonicalJTFTestExePath)
{
    m_jtftestScanner.start(canonicalJTFTestExePath);
}

void MainWindow::fillJTFComboBox(QList<JtfTest> tests)
{
    ui->m_jtfTestChooseComboBox->clear();

    ui->m_jtfTestChooseComboBox->addItem(QString(ALL_TESTS_STRING));

    foreach (const JtfTest &test, tests) {
        if (!test.m_testName.isEmpty())
            ui->m_jtfTestChooseComboBox->addItem(test.m_testName);
    }

    ui->m_runPushButton->setEnabled(true);
}

void MainWindow::disableWidgets(bool disable)
{
    ui->m_removePushButton->setDisabled(disable);
    ui->m_jtfTestChooseComboBox->setDisabled(disable);
    ui->m_outputTextEdit->setDisabled(disable);
    ui->m_rawOutputTextEdit->setDisabled(disable);
    ui->m_errorLinesTextEdit->setDisabled(disable);
    ui->m_recheckForTestsPushButton->setDisabled(disable);
    ui->m_runPushButton->setDisabled(disable);
    ui->m_selectJTFExeComboBox->setDisabled(disable);
    ui->m_testIndicatorIconFrame->setDisabled(disable);
}

QMessageBox::StandardButton MainWindow::showExecutableNotFoundMessage(const QString &canonicalExecutablePath, bool offerRemoveOption)
{
    if (!offerRemoveOption)
        return QMessageBox::warning(this,
                                    tr("Error!"),
                                    tr("Executable(s) not found:\n") + canonicalExecutablePath);

    return static_cast<QMessageBox::StandardButton>(QMessageBox::warning(this,
                                tr("Error!"),
                                tr("Executable(s) not found:\n") + canonicalExecutablePath,
                                QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::Close));
}


void MainWindow::checkForEnvironmentVariable(const QString &envVar)
{
    if (!QProcessEnvironment::systemEnvironment().contains(envVar))
        QMessageBox::warning(this, tr("Check for system variable %1").arg(envVar), tr("System variable %1 is not set.").arg(envVar));
}

QString MainWindow::readEnvVar(const QString &envVar) const
{
    return QProcessEnvironment::systemEnvironment().value(envVar);
}
