#ifndef TESTRESULTSPARSER_H
#define TESTRESULTSPARSER_H

#include <QStringList>

struct TestErrorLine
{
    TestErrorLine();

    QStringList m_errorLines;
    QString m_testName;
};

struct TestResult
{
    TestResult();

    QString toString() const;
    QString errorLinesToString() const;

    TestErrorLine *testErrorLine(const QString &testName) const;

    bool m_isPassed;
    int m_numberOfPassedTests;
    int m_numberOfFailedTests;

    QString m_formatedOutput;
    QString m_rawOutput;
    QStringList m_failedTests;

    QList<TestErrorLine *> m_errorLines;
};

class TestResultsParser
{
public:
    TestResultsParser();
    ~TestResultsParser();

    TestResult parse(const QString &input) const;

private:
    void parseNumberOfPassedTests(const QString &input, TestResult &testResult) const;
    void parseNumberOfFailedTests(const QString &input, TestResult &testResult) const;
    void parseFailedTests(const QStringList &inputList, TestResult &testResult) const;
    void cleanErrorStrings(QStringList *errorString) const;
};

#endif // TESTRESULTSPARSER_H
