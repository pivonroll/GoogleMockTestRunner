#ifndef PROCESSRUNNER_H
#define PROCESSRUNNER_H

#include <QStringList>

class ProcessRunner
{
public:
    ProcessRunner(const QString &canonicalPath, const QStringList &arguments);
    ~ProcessRunner();

    QString start();

private:
    QString m_canonicalPath;
    QStringList m_arguments;
};

#endif // PROCESSRUNNER_H
