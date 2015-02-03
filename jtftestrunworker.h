#ifndef JTFTESTRUNWORKER_H
#define JTFTESTRUNWORKER_H

#include <QObject>
#include <QStringList>
#include <QFutureWatcher>

class JTFTestRunWorker : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(JTFTestRunWorker)
public:
    explicit JTFTestRunWorker(const QString &canonicalExePath, const QStringList &arguments, QObject *parent = 0);
    ~JTFTestRunWorker();

    void start();

    QString canonicalExePath() const;
    QStringList arguments() const;

signals:
    void finished(const QString result);
    void error();

private slots:
    void onTaskFinished();

private:
    QFutureWatcher<QString> m_watcher;
    QString m_canonicalExePath;
    QStringList m_ExeArguments;
};

#endif // JTFTESTRUNWORKER_H
