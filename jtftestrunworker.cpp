#include "jtftestrunworker.h"

#include "jtftestrunner.h"

#include <QProcess>
#include <QtConcurrent/QtConcurrent>

JTFTestRunWorker::JTFTestRunWorker(const QString &canonicalExePath, const QStringList &arguments, QObject *parent)
    : QObject(parent),
      m_canonicalExePath(canonicalExePath),
      m_ExeArguments(arguments)
{
    connect(&m_watcher, SIGNAL(finished()), this, SLOT(onTaskFinished()));
}

JTFTestRunWorker::~JTFTestRunWorker()
{
}

void JTFTestRunWorker::start()
{
    JTFTestRunner runner(m_canonicalExePath, m_ExeArguments);
    QFuture<QString> result = QtConcurrent::run(runner, &JTFTestRunner::runProcess);
    m_watcher.setFuture(result);
}

QString JTFTestRunWorker::canonicalExePath() const
{
    return m_canonicalExePath;
}

QStringList JTFTestRunWorker::arguments() const
{
    return m_ExeArguments;
}

void JTFTestRunWorker::onTaskFinished()
{
    QString result = m_watcher.result();

    if (result.isEmpty()) {
        emit error();
        return;
    }

    emit finished(result);
}
