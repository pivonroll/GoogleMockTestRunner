#include "jtftestscanner.h"

#include "jtftestparser.h"

#include <QProcess>

JTFTestScanner::JTFTestScanner(const QString &processCanonicalPath)
    : m_processCanonicalPath(processCanonicalPath)
{
}

JTFTestScanner::~JTFTestScanner()
{
}

QList<JtfTest> JTFTestScanner::scanForAvailableTests()
{
    QProcess jtfTestExeProcess;
    jtfTestExeProcess.setProcessChannelMode(QProcess::MergedChannels);
    jtfTestExeProcess.start(m_processCanonicalPath, QStringList() << "--gtest_list_tests");

    if (jtfTestExeProcess.waitForFinished()) {
        QString output(jtfTestExeProcess.readAll());
        JTFTestParser parser;
        return parser.parse(output);
    }

    return QList<JtfTest>();
}
