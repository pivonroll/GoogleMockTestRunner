#ifndef ADDENVVARTOTRACK_H
#define ADDENVVARTOTRACK_H

#include <QDialog>

namespace Ui {
class AddEnvVarToTrack;
}

class AddEnvVarToTrack : public QDialog
{
    Q_OBJECT

public:
    explicit AddEnvVarToTrack(QWidget *parent = 0);
    ~AddEnvVarToTrack();

    QString varibleToTrack() const;

private:
    Ui::AddEnvVarToTrack *ui;
};

#endif // ADDENVVARTOTRACK_H
