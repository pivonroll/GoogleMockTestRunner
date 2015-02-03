#ifndef LISTOFENVIRONMENTVARSTOTRACK_H
#define LISTOFENVIRONMENTVARSTOTRACK_H

#include <QDialog>

namespace Ui {
class ListOfEnvironmentVarsToTrack;
}

class ListOfEnvironmentVarsToTrack : public QDialog
{
    Q_OBJECT

public:
    explicit ListOfEnvironmentVarsToTrack(QWidget *parent = 0);
    ~ListOfEnvironmentVarsToTrack();

    void addRow(const QString &variableName, const QString &varibaleValue);

private:
    Ui::ListOfEnvironmentVarsToTrack *ui;
};

#endif // LISTOFENVIRONMENTVARSTOTRACK_H
