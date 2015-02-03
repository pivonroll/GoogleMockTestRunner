#include "testresultsparser.h"

#include <QStringList>

const char GMOCK_SECTION_SPLIT[] = "[==========]";
const char FAILED_STRING[] = "[  FAILED  ]";
const char PASSED_STRING[] = "[  PASSED  ]";
const char GMOCK_WARNING_STRING[] = "GMOCK WARNING:";
const char GMOCK_ERROR_STRING[] = "error:";
const char STACK_TRACE_STRING[] = "Stack trace:";
const char CARRIAGE_RETURN_LINE_FEED[] = "\r\n";
const char RUN_STRING[] = "[ RUN      ]";

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

    QStringList split = result.m_rawOutput.split(GMOCK_SECTION_SPLIT);

    if (split.size() == 3 && !split[1].isEmpty()) {
        QString resultsString = split[1];
        QStringList testRunResults = resultsString.split(QLatin1String(RUN_STRING));
        testRunResults.pop_front();

        foreach (const QString &testRunResult, testRunResults) {
            QString testName = testRunResult.left(testRunResult.indexOf(QLatin1String(CARRIAGE_RETURN_LINE_FEED)));

            TestErrorLine *testErrorLine = result.testErrorLine(testName);

            if (!testErrorLine)
                testErrorLine = new TestErrorLine;

            testErrorLine->m_testName = testName;
            result.m_errorLines.append(testErrorLine);

            QStringList splittedWarnings = testRunResult.split(QLatin1String(GMOCK_WARNING_STRING));

            foreach (const QString &line, splittedWarnings) {
                if (line.contains(QLatin1String(GMOCK_ERROR_STRING))) {
                    QStringList splitted = line.split(QLatin1String(STACK_TRACE_STRING));

                    if  (splitted.size() == 2 && !splitted[1].isEmpty()) {
                        QStringList errorLines = splitted[1].trimmed().split(CARRIAGE_RETURN_LINE_FEED);

                        cleanErrorStrings(&errorLines);

                        if (errorLines.size() > 0) {
                            testErrorLine->m_errorLines = errorLines;
                        }
                    }
                }
            }
        }
    }

    if (split.size() == 3 && !split[2].isEmpty()) {
        QString resultsString = split[2];

        QStringList splittedResults = resultsString.split(CARRIAGE_RETURN_LINE_FEED);

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

void TestResultsParser::cleanErrorStrings(QStringList *errorString) const
{
    if (!errorString)
        return;

    int index = errorString->size() - 1;

    while (index < errorString->size() && index >= 0) {
        QString endLine = errorString->at(index);

        if (endLine.isEmpty() ||
                endLine.contains(QLatin1String(FAILED_STRING)) ||
                endLine.contains(QLatin1String("[----------]")))
            errorString->removeAt(index);
        --index;
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

QString TestResult::errorLinesToString() const
{
    QString output;
    output.append("=============== ERRORS ===============\n");

    foreach (const TestErrorLine *line, m_errorLines) {
        if (line) {
            output.append(">>>>>>>>>>>>>>>>>>>>>\n");
            output.append(QString("Test name: %1 \n\n").arg(line->m_testName));
            output.append(QString("%1\n\n").arg(line->m_errorLines.join(QLatin1Char('\n'))));
            output.append("<<<<<<<<<<<<<<<<<<<<<\n\n");
        }
    }

    return output;
}

TestErrorLine *TestResult::testErrorLine(const QString &testName) const
{
    for (int i = 0; i < m_errorLines.size(); ++i) {
        TestErrorLine *errorLine = m_errorLines[i];
        if (errorLine && errorLine->m_testName == testName)
            return errorLine;
    }

    return 0;
}
