#ifndef SIGNIN_H
#define SIGNIN_H

#include "head.h"
#include "register.h"

namespace Ui {
class Signin;
}

class Signin : public QWidget
{
    Q_OBJECT

public:
    explicit Signin(QWidget *parent = nullptr);
    ~Signin();

    void setWindow();
    // void paintEvent(QPaintEvent *event);
    void TikTok();
    void mousePressEvent(QMouseEvent* mEvent);
    void mouseMoveEvent(QMouseEvent* mEvent);
    void initRFile();
    void signinWFile();
    void netLink();

signals:
    //定义子窗口向主窗口发送数据的信号
    void send_to_main(QString send_msg);

private slots:
    void on_close_clicked();

    void on_min_clicked();

    void on_signin_clicked();

    void on_signup_clicked();

private:
    Ui::Signin *ui;
    QPoint dragPosition;
    QTcpSocket *tSocket;
    bool insuccess = false;
    bool netlink;
    QString username;
    Register _register;
};

#endif // SIGNIN_H
