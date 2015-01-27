#ifndef JTFTESTSCANNERWORKER_H
#define JTFTESTSCANNERWORKER_H

#include "common.h"

#include <QObject>
#include <QString>
#include <QFutureWatcher>

class JTFTestScannerWorker : public QObject
{
    Q_OBJECT

public:
    JTFTestScannerWorker();

    void start(const QString &canonicalJTFExePath);

private slots:
    void onTaskFinished();

signals:
    void finished(QString canonicalExePath, QList<JtfTest> result);

private:
    QFutureWatcher<QList<JtfTest> > m_watcher;
    QString m_canonicalJTFTestPath;
};

#endif // JTFTESTSCANNERWORKER_H
