#include "processrunner.h"
#include <QProcess>

ProcessRunner::ProcessRunner(const QString &canonicalPath, const QStringList &arguments)
    : m_canonicalPath(canonicalPath),
      m_arguments(arguments)
{

}

ProcessRunner::~ProcessRunner()
{
}

QString ProcessRunner::start()
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start(m_canonicalPath, m_arguments);

    if(process.waitForFinished())
        return process.readAll();

    return QString();
}

