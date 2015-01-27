#ifndef JTFTESTSCANNER_H
#define JTFTESTSCANNER_H

#include "common.h"
#include <QString>

class JTFTestScanner
{
public:
    JTFTestScanner(const QString &processCanonicalPath);
    ~JTFTestScanner();

    QList<JtfTest> scanForAvailableTests();

private:
    QString m_processCanonicalPath;
};

#endif // JTFTESTSCANNER_H
