#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>


namespace Ui {
class ErrorDialog;
}

class ErrorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorDialog(QWidget *parent = nullptr);
    ~ErrorDialog();
    void addError(QString error);

private:
    Ui::ErrorDialog *ui;
};

#endif // ERRORDIALOG_H
