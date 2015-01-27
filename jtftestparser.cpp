#include "jtftestparser.h"

JTFTestParser::JTFTestParser()
{
}

JTFTestParser::~JTFTestParser()
{

}

QList<JtfTest> JTFTestParser::parse(const QString &input)
{
    QList<JtfTest> jtfTests;

    JtfTest test;

    QString temp = input.trimmed();

    QStringList split = temp.split("\r\n");

    foreach (QString line, split) {
        if (!line.isEmpty()) {
            if (isTestName(line)) {
                line.remove(line.size() - 1, 1);
                test = JtfTest();
                test.m_testName = line;
                jtfTests.append(test);
            }
            else
                test.m_tests.append(line);
        }
    }

    return jtfTests;
}

bool JTFTestParser::isTestName(const QString &input) const
{
    if (input.trimmed().endsWith('.'))
        return true;

    return false;
}
