#include "jtftestscannerworker.h"

#include "jtftestscanner.h"

#include <QtConcurrent/QtConcurrent>

JTFTestScannerWorker::JTFTestScannerWorker()
{
    connect(&m_watcher, SIGNAL(finished()), this, SLOT(onTaskFinished()));
}

void JTFTestScannerWorker::start(const QString &canonicalJTFExePath)
{
    m_canonicalJTFTestPath = canonicalJTFExePath;
    JTFTestScanner scanner(canonicalJTFExePath);
    QFuture<QList<JtfTest> > result = QtConcurrent::run(scanner, &JTFTestScanner::scanForAvailableTests);
    m_watcher.setFuture(result);
}

void JTFTestScannerWorker::onTaskFinished()
{
    QList<JtfTest> result = m_watcher.result();
    emit finished(m_canonicalJTFTestPath, result);
}
