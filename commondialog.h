#ifndef COMMONDIALOG_H
#define COMMONDIALOG_H

#include <QDialog>

namespace Ui {
class CommonDialog;
}

class CommonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommonDialog(QWidget *parent = nullptr);
    ~CommonDialog();
    void setLWarning(float val);
    void setHWarning(float val);
    void setLEmergency(float val);
    void setHEmergency(float val);
    void setAutoUnShutUp(bool val);
    void setInterval(uint val);
    void setSound(bool val);

    float getLWarning();
    float getHWarning();
    float getLEmergency();
    float getHEmergency();
    bool getAutoUnShutUp();
    uint getInterval();
    bool getSound();

private:
    Ui::CommonDialog *ui;
};

#endif // COMMONDIALOG_H
