#include "listofenvironmentvarstotrack.h"
#include "ui_listofenvironmentvarstotrack.h"

#include <QHeaderView>

ListOfEnvironmentVarsToTrack::ListOfEnvironmentVarsToTrack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListOfEnvironmentVarsToTrack)
{
    ui->setupUi(this);
    ui->m_listEnvVarsToTrackTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

ListOfEnvironmentVarsToTrack::~ListOfEnvironmentVarsToTrack()
{
    delete ui;
}

void ListOfEnvironmentVarsToTrack::addRow(const QString &variableName, const QString &varibaleValue)
{
    ui->m_listEnvVarsToTrackTableWidget->insertRow(ui->m_listEnvVarsToTrackTableWidget->rowCount());
    QTableWidgetItem *item = new QTableWidgetItem(variableName);
    ui->m_listEnvVarsToTrackTableWidget->setItem(ui->m_listEnvVarsToTrackTableWidget->rowCount() - 1,
                                                 0,
                                                 item);

    item = new QTableWidgetItem(varibaleValue);
    ui->m_listEnvVarsToTrackTableWidget->setItem(ui->m_listEnvVarsToTrackTableWidget->rowCount() - 1,
                                                 1,
                                                 item);

}
