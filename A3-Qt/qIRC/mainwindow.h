#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void displayMessage(QString);
    ~MainWindow();

private slots:
    void on_chatPushButton_clicked();

    void on_messageLineEdit_returnPressed();

    void openServerConnectionDialog();

private:
    Ui::MainWindow *ui;
    void sendMessage();
    Client* client;
};

#endif // MAINWINDOW_H
