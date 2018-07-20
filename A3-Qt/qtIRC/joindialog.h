#ifndef JOINDIALOG_H
#define JOINDIALOG_H

#include <QWidget>

namespace Ui {
class JoinDialog;
}

class JoinDialog : public QWidget
{
    Q_OBJECT

public:
    explicit JoinDialog(QWidget *parent = 0);
    ~JoinDialog();

signals:
    void joinRequest(QString channelName);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::JoinDialog *ui;
};

#endif // JOINDIALOG_H
