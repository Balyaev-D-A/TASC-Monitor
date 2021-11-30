#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

typedef struct {
    QString port;
    QByteArray geometry;
    QByteArray state;
    float l_warning;
    float h_warning;
    float l_emergency;
    float h_emergency;
    bool auto_unshutup;
    uint interval;
    bool trend;
    bool sound;
    bool ontop;
    bool widgetstate;
} tSettings;

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    ~Settings();
    QString getPort();
    QByteArray getGeometry();
    QByteArray  getState();
    float getLWarning();
    float getHWarning();
    float getLEmergency();
    float getHEmergency();
    bool getAutoUnShutUp();
    uint getInterval();
    bool getTrend();
    bool getSound();
    bool getOnTop();
    bool getWidgetState();

    void setPort(QString port);
    void setGeometry(QByteArray geometry);
    void setState(QByteArray state);
    void setLWarning(float val);
    void setHWarning(float val);
    void setLEmergency(float val);
    void setHEmergency(float val);
    void setAutoUnShutUp(bool val);
    void setInterval(uint val);
    void setTrend(bool val);
    void setSound(bool val);
    void setOnTop(bool val);
    void setWidgetState(bool val);

    void readSettings();
    void saveSettings();

public slots:

private:
    QSettings *s;
    tSettings settings;


};

#endif // SETTINGS_H
