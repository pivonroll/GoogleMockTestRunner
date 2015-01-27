#ifndef JTFTESTPARSER_H
#define JTFTESTPARSER_H

#include "common.h"

#include <QList>

class JTFTestParser
{
public:
    JTFTestParser();
    ~JTFTestParser();

    QList<JtfTest> parse(const QString &input);

private:
    bool isTestName(const QString &input) const;
};

#endif // JTFTESTPARSER_H
