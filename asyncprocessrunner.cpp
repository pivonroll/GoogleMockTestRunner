#include "asyncprocessrunner.h"

#include "processrunner.h"

#include <QtConcurrent/QtConcurrent>
#include <QFuture>

AsyncProcessRunner::AsyncProcessRunner(QObject *parent) : QObject(parent)
{
    connect(&m_watcher, SIGNAL(finished()),
            this, SLOT(onProcessFinished()));
}

AsyncProcessRunner::~AsyncProcessRunner()
{
}

void AsyncProcessRunner::onProcessFinished()
{
    emit finished(m_watcher.result());
}

void AsyncProcessRunner::start(const QString &canonicalExePath, const QStringList &arguments)
{
    ProcessRunner runner(canonicalExePath, arguments);
    QFuture<QString> result = QtConcurrent::run(runner, &ProcessRunner::start);
    m_watcher.setFuture(result);
}

