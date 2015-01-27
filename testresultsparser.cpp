#include "testresultsparser.h"

#include <QStringList>

const char FAILED_STRING[] = "[  FAILED  ]";
const char PASSED_STRING[] = "[  PASSED  ]";


TestErrorLine::TestErrorLine()
{

}

TestResultsParser::TestResultsParser()
{
}

TestResultsParser::~TestResultsParser()
{
}

TestResult TestResultsParser::parse(const QString &input) const
{
    TestResult result;
    result.m_rawOutput = input.trimmed();

    QStringList split = result.m_rawOutput.split("[==========]");

    if (split.size() == 3 && !split[2].isEmpty()) {
        QString resultsString = split[2];

        QStringList splittedResults = resultsString.split("\r\n");

        if (splittedResults.size() > 1)
            parseNumberOfPassedTests(splittedResults[1], result);

        if (splittedResults.size() > 2) {
            parseNumberOfFailedTests(splittedResults[2], result);

            QStringList failedTests = splittedResults;
            failedTests.removeFirst();
            failedTests.removeFirst();
            failedTests.removeFirst();

            if (failedTests.size() > 0)
                parseFailedTests(failedTests, result);
        }
    }

    result.m_isPassed = result.m_numberOfFailedTests == 0;

    return result;
}

void TestResultsParser::parseNumberOfPassedTests(const QString &input, TestResult &testResult) const
{
    int offset = QString(PASSED_STRING).size();
    QString temp = input.mid(offset).trimmed();
    QStringList split = temp.split(" ");

    if (split.size() >= 1) {
        bool ok;
        int numberOfPassedTests = split[0].toInt(&ok);

        if (!ok)
            testResult.m_numberOfPassedTests = 0;
        else
            testResult.m_numberOfPassedTests = numberOfPassedTests;
    }
}

void TestResultsParser::parseNumberOfFailedTests(const QString &input, TestResult &testResult) const
{
    int offset = QString(FAILED_STRING).size();
    QString temp = input.mid(offset).trimmed();
    QStringList split = temp.split(" ");

    if (split.size() >= 1) {
        bool ok;
        int numberOfFailedTests = split[0].toInt(&ok);

        if (!ok)
            testResult.m_numberOfFailedTests = 0;
        else
            testResult.m_numberOfFailedTests = numberOfFailedTests;
    }
}

void TestResultsParser::parseFailedTests(const QStringList &inputList, TestResult &testResult) const
{
    foreach(const QString &input, inputList) {

        if (input.startsWith(FAILED_STRING)) {
            int offset = QString(FAILED_STRING).size();
            QString temp = input.mid(offset).trimmed();

            if (!temp.isEmpty())
                testResult.m_failedTests.append(temp);
        }
    }
}


TestResult::TestResult()
    : m_isPassed(false),
      m_numberOfFailedTests(0),
      m_numberOfPassedTests(0)
{
}

QString TestResult::toString() const
{
    QString output;
    if (m_isPassed)
        output.append("==============  TESTS PASSED ==============\n");
    else
        output.append("==============  TESTS FAILED ==============\n");

    output.append("\n");

    output.append(QString("Number of PASSED tests : %1 \n").arg(m_numberOfPassedTests));
    output.append(QString("Number of FAILED tests : %1 \n").arg(m_numberOfFailedTests));

    output.append("\n");

    if (m_numberOfFailedTests > 0) {
        output.append("List of failed tests:\n");

        foreach (const QString &failedTest, m_failedTests)
            output.append(QString("\t%1\n").arg(failedTest));
    }

    return output;
}
