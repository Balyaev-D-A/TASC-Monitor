#include "commondialog.h"
#include "ui_commondialog.h"

CommonDialog::CommonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommonDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

CommonDialog::~CommonDialog()
{
    delete ui;
}


void CommonDialog::setHWarning(float val)
{
    ui->dsbUWarning->setValue(val);
}

void CommonDialog::setLWarning(float val)
{
    ui->dsbLWarning->setValue(val);
}

void CommonDialog::setHEmergency(float val)
{
    ui->dsbUEmergency->setValue(val);
}

void CommonDialog::setLEmergency(float val)
{
    ui->dsbLEmergency->setValue(val);
}

void CommonDialog::setAutoUnShutUp(bool val)
{
    ui->cbAutoShutup->setChecked(val);
}

void CommonDialog::setInterval(uint val)
{
    ui->sbUpdate->setValue(val);
}

void CommonDialog::setSound(bool val)
{
    ui->cbSound->setChecked(val);
}

float CommonDialog::getHWarning()
{
    return ui->dsbUWarning->value();
}

float CommonDialog::getLWarning()
{
    return ui->dsbLWarning->value();
}

float CommonDialog::getHEmergency()
{
    return ui->dsbUEmergency->value();
}

float CommonDialog::getLEmergency()
{
    return ui->dsbLEmergency->value();
}

bool CommonDialog::getAutoUnShutUp()
{
    return ui->cbAutoShutup->isChecked();
}

uint CommonDialog::getInterval()
{
    return ui->sbUpdate->value();
}

bool CommonDialog::getSound()
{
    return ui->cbSound->isChecked();
}
