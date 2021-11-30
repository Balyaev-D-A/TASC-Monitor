#include <QDate>
#include <QTime>

#include "errordialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

void ErrorDialog::addError(QString error)
{
    QString err = "[" + QDate::currentDate().toString() + "] (" + QTime::currentTime().toString() + ") | " + error;
    ui->errorList->insertItem(0, err);
}
