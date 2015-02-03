#include "addenvvartotrack.h"
#include "ui_addenvvartotrack.h"

AddEnvVarToTrack::AddEnvVarToTrack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEnvVarToTrack)
{
    ui->setupUi(this);
}

AddEnvVarToTrack::~AddEnvVarToTrack()
{
    delete ui;
}

QString AddEnvVarToTrack::varibleToTrack() const
{
    return ui->m_varToTrackLineEdit->text();
}
