#include "jtftestrunner.h"

#include <QProcess>

JTFTestRunner::JTFTestRunner(const QString &canonicalExePath, const QStringList &arguments)
    : m_canonicalExecutablePath(canonicalExePath),
      m_ExeArguments(arguments)
{
}


QString JTFTestRunner::runProcess()
{
    QProcess jtfProcess;
    jtfProcess.setProcessChannelMode(QProcess::MergedChannels);
    jtfProcess.start(m_canonicalExecutablePath, m_ExeArguments);

    if (jtfProcess.waitForFinished())
        return jtfProcess.readAll();

    return QString();
}
