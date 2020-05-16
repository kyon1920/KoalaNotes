#ifndef REGISTER_H
#define REGISTER_H

#include "head.h"

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

    void setWindow();
    void windowclosed();
    void windowmin();
    void mousePressEvent(QMouseEvent* mEvent);
    void mouseMoveEvent(QMouseEvent* mEvent);
    void netLink();

private slots:
    void on_sure_clicked();

    void on_headpic_clicked();

    void on_h0_clicked();

    void on_h1_clicked();

    void on_h2_clicked();

    void on_h5_clicked();

    void on_h3_clicked();

    void on_h4_clicked();

    void on_h6_clicked();

    void on_h7_clicked();

    void on_h8_clicked();

private:
    Ui::Register *ui;
    QPoint dragPosition;
    QString OpenFile, OpenFilePath;
    QTcpSocket *tSocket;
    bool netlink;
    int n = 0;
    int selecthead = 0;
    QString headp = "1";
};

#endif // REGISTER_H
