#include <QApplication>
#include "settings.h"
#include "connectiondialog.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
    s = new QSettings(QApplication::applicationDirPath() + "\\config.ini", QSettings::IniFormat, this);
    readSettings();
}

Settings::~Settings()
{
    saveSettings();
}
void Settings::readSettings()
{
    s->beginGroup("Connection");
    settings.port = s->value("port", "COM1").toString();
    s->endGroup();

    s->beginGroup("Common");
    settings.geometry = s->value("geometry").toByteArray();
    settings.state = s->value("state").toByteArray();
    settings.h_warning = s->value("h_warning", 110.0).toFloat();
    settings.l_warning = s->value("l_warning", 90.0).toFloat();
    settings.h_emergency = s->value("h_emergency", 120.0).toFloat();
    settings.l_emergency = s->value("l_emergency", 80.0).toFloat();
    settings.auto_unshutup = s->value("auto_unshutup", true).toBool();
    settings.interval = s->value("interval", 1).toInt();
    settings.trend = s->value("trend", true).toBool();
    settings.sound = s->value("sound", true).toBool();
    settings.ontop = s->value("ontop", false).toBool();
    s->endGroup();
}

void Settings::saveSettings()
{
    s->beginGroup("Connection");
    s->setValue("port", settings.port);
    s->endGroup();

    s->beginGroup("Common");
    s->setValue("geometry", settings.geometry);
    s->setValue("state", settings.state);
    s->setValue("h_warning", settings.h_warning);
    s->setValue("l_warning", settings.l_warning);
    s->setValue("h_emergency", settings.h_emergency);
    s->setValue("l_emergency", settings.l_emergency);
    s->setValue("auto_unshutup", settings.auto_unshutup);
    s->setValue("interval", settings.interval);
    s->setValue("trend", settings.trend);
    s->setValue("sound", settings.sound);
    s->setValue("ontop", settings.ontop);
    s->endGroup();

    s->sync();
}

void Settings::setPort(QString port)
{
    settings.port = port;
}

void Settings::setGeometry(QByteArray geometry)
{
    settings.geometry = geometry;
}

void Settings::setState(QByteArray state)
{
    settings.state = state;
}

void Settings::setHWarning(float val)
{
    settings.h_warning = val;
}

void Settings::setLWarning(float val)
{
    settings.l_warning = val;
}

void Settings::setHEmergency(float val)
{
    settings.h_emergency = val;
}

void Settings::setLEmergency(float val)
{
    settings.l_emergency = val;
}

void Settings::setAutoUnShutUp(bool val)
{
    settings.auto_unshutup = val;
}

void Settings::setInterval(uint val)
{
    settings.interval = val;
}

void Settings::setTrend(bool val)
{
    settings.trend = val;
}

void Settings::setSound(bool val)
{
    settings.sound = val;
}

void Settings::setOnTop(bool val)
{
    settings.ontop = val;
}

QString Settings::getPort()
{
    return settings.port;
}

QByteArray Settings::getGeometry()
{
    return settings.geometry;
}

QByteArray Settings::getState()
{
    return settings.state;
}

float Settings::getHWarning()
{
    return settings.h_warning;
}

float Settings::getLWarning()
{
    return settings.l_warning;
}

float Settings::getHEmergency()
{
    return settings.h_emergency;
}

float Settings::getLEmergency()
{
    return settings.l_emergency;
}

bool Settings::getAutoUnShutUp()
{
    return settings.auto_unshutup;
}

uint Settings::getInterval()
{
    return settings.interval;
}

bool Settings::getTrend()
{
    return settings.trend;
}

bool Settings::getSound()
{
    return settings.sound;
}

bool Settings::getOnTop()
{
    return settings.ontop;
}
