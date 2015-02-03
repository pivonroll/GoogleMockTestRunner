#ifndef JTFTESTRUNNER_H
#define JTFTESTRUNNER_H

#include <QObject>
#include <QStringList>

class JTFTestRunner
{
public:
    explicit JTFTestRunner(const QString &canonicalExePath, const QStringList &arguments);

    QString runProcess();

private:
    QString m_canonicalExecutablePath;
    QStringList m_ExeArguments;
};

#endif // JTFTESTRUNNER_H
