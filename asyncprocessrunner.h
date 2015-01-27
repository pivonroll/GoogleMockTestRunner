#ifndef ASYNCPROCESSRUNNER_H
#define ASYNCPROCESSRUNNER_H

#include <QProcess>
#include <QFutureWatcher>

class AsyncProcessRunner : public QObject
{
    Q_OBJECT
public:
    explicit AsyncProcessRunner(QObject *parent = 0);
    ~AsyncProcessRunner();

    void start(const QString &canonicalExePath, const QStringList &arguments);

signals:
    void finished(QString result);
    void error();

private slots:
    void onProcessFinished();

private:
    QFutureWatcher<QString> m_watcher;
};

#endif // ASYNCPROCESSRUNNER_H
