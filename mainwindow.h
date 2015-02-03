#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "jtftestscannerworker.h"

#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onAddJTFExeButtonClicked();
    void onBrowseButtonClicked();
    void onRecheckForAvailableTestsClicked();
    void onRunButtonClicked();
    void onExecutableChanged(QString canonicalExePath);
    void onJTFTestScanCompleted(QString canonicalExePath, QList<JtfTest> tests);
    void onExeLineEditTextChanged(QString text);
    void onMenuSaveClicked();
    void onRemovePushButtonClicked();
    void onAddEnvVarToTrack();
    void onListEnvVarToTrack();

    void onJTFTestCompleted(const QString result);
    void onJTFTestError();

private:
    void initialize();
    void loadSettings();
    void saveSettings();
    void createConnections();
    void runJTFTest(const QString &canonicalExePath, const QString &test);

    void startScanForAvailableJTFTests(const QString &canonicalJTFTestExePath);
    void fillJTFComboBox(QList<JtfTest> tests);

    void disableWidgets(bool disable);
    QMessageBox::StandardButton showExecutableNotFoundMessage(const QString &canonicalExecutablePath,
                                                              bool offerRemoveOption = false);
    void checkForEnvironmentVariables();
    QString readEnvVar(const QString &envVar) const;

    Ui::MainWindow *ui;

    QMap<QString, QVariant> m_environmentVariablesToTrack;

    JTFTestScannerWorker m_jtftestScanner;
    QAction *m_saveAction;
    QAction *m_envVarToTrack;
    QAction *m_listOfEnvVarsToTrack;
};

#endif // MAINWINDOW_H
