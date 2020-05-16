#include "signin.h"
#include "ui_signin.h"

Signin::Signin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signin)
{
    ui->setupUi(this);
    setWindow();
    initRFile();
    netLink();
}

Signin::~Signin()
{
    // 主动和对方断开链接
    tSocket -> disconnectFromHost();
    tSocket -> close();
    delete ui;
}

void Signin::setWindow()
{
    setWindowIcon(QIcon("../icons/M_icon.png"));
    // 设置窗口置顶
    setWindowFlags(Qt::WindowStaysOnTopHint);
    // 设置窗体无边框、背景透明、圆角窗口
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),5,5);
    setMask(bmp);

    // Title
    QPalette pe_title;
    pe_title.setColor(QPalette::WindowText,Qt::white);
    ui -> title -> setPalette(pe_title);

    // Background
    ui -> bg -> setPixmap(QPixmap("../images/bg.jpg"));
    ui -> bg -> setScaledContents(true);
    ui -> icon -> setPixmap(QPixmap("../icons/M_icon.png"));
    ui -> icon -> setScaledContents(true);
    ui -> username_icon -> setPixmap(QPixmap("../icons/username.png"));
    ui -> username_icon -> setScaledContents(true);
    ui -> password_icon -> setPixmap(QPixmap("../icons/password.png"));
    ui -> password_icon -> setScaledContents(true);
    ui -> password -> setEchoMode(QLineEdit::Password);
    ui -> username -> setStyleSheet("background:transparent;border-width:0;border-style:outset");
    ui -> password -> setStyleSheet("background:transparent;border-width:0;border-style:outset");

    // 自定义按钮样式
    const QString closeStyle = "\
                QPushButton{\
                    color: rgb(255, 0, 0);\
                    border:1px;\
                }\
                QPushButton:hover{\
                    color: rgb(38, 133, 227);\
                }\
                QPushButton:pressed{\
                    color: rgb(255, 255, 255);\
                }";
    ui -> close -> setStyleSheet(closeStyle);
    const QString btnStyle = "\
                QPushButton{\
                    color: rgb(255, 255, 255);\
                    border:1px;\
                }\
                QPushButton:hover{\
                    color: rgb(0, 0, 255);\
                }\
                QPushButton:pressed{\
                    color: rgb(255, 0, 0);\
                }";
    ui -> min -> setStyleSheet(btnStyle);
    ui -> feedback -> setStyleSheet(btnStyle);
    const QString outStyle = "\
                QPushButton{\
                    color: rgb(166, 166, 166);\
                    border:1px;\
                }\
                QPushButton:hover{\
                    color: rgb(25, 25, 25);\
                }\
                QPushButton:pressed{\
                    color: rgb(144, 144, 144);\
                }";
    ui -> signup -> setStyleSheet(outStyle);
    ui -> findpassword -> setStyleSheet(outStyle);
    const QString signinStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background:url(../images/signin.jpg); \
                    border:1px;  \
                    border-radius:5px; /*border-radius控制圆角大小*/\
                    padding:2px 4px; \
                }\
                QPushButton:hover{\
                    color: rgb(255, 255, 0);\
                    background:url(../images/signin.jpg); \
                    border:1px;\
                    border-radius:5px; /*border-radius控制圆角大小*/\
                    padding:2px 4px;  \
                }\
                QPushButton:pressed{    \
                    color: rgb(0, 255, 0); \
                    background:url(../images/signin.jpg); \
                    border:1px;  \
                    border-radius:5px; /*border-radius控制圆角大小*/\
                    padding:2px 4px; \
                }";
    ui -> signin -> setStyleSheet(signinStyle);

    // 为按钮设置提示信息
    ui -> min -> setToolTip(tr("最小化"));
    ui -> close -> setToolTip(tr("关闭"));
    ui -> username -> setToolTip(tr("账号"));
    ui -> password -> setToolTip(tr("密码"));

    // 头像框设计
    const QString circleLable = "border-radius: 50px; border:2px ridge gray; background:url(../Icons/signin_head.jpg)";
    ui -> head -> setStyleSheet(circleLable);

}

/* void Signin::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter paintLine(ui -> bg);
    paintLine.setPen(Qt::red);
    paintLine.drawLine(120, 290, 420, 290);
    paintLine.drawLine(120, 330, 420, 330);

}*/

void Signin::mousePressEvent(QMouseEvent* mEvent)
{
    if(mEvent->button()==Qt::LeftButton)
    {
        dragPosition = mEvent->globalPos()-frameGeometry().topLeft();
        mEvent->accept();
    }
}

void Signin::mouseMoveEvent(QMouseEvent* mEvent)
{
    if(mEvent->buttons()& Qt::LeftButton)
    {
        move(mEvent->globalPos() - dragPosition);
        mEvent->accept();
    }
}

void Signin::TikTok()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"geometry");
    animation->setDuration(500);
    animation->setKeyValueAt(0,QRect(QPoint(this->frameGeometry().x()-3,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.1,QRect(QPoint(this->frameGeometry().x()+6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.2,QRect(QPoint(this->frameGeometry().x()-6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.3,QRect(QPoint(this->frameGeometry().x()+6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.4,QRect(QPoint(this->frameGeometry().x()-6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.5,QRect(QPoint(this->frameGeometry().x()+6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.6,QRect(QPoint(this->frameGeometry().x()-6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.7,QRect(QPoint(this->frameGeometry().x()+6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.8,QRect(QPoint(this->frameGeometry().x()-6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.9,QRect(QPoint(this->frameGeometry().x()+6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(1,QRect(QPoint(this->frameGeometry().x()-3,this->frameGeometry().y()),this->size()));
    animation->start();
}

// 关闭窗口
void Signin::on_close_clicked()
{
    // 淡出效果
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation -> setDuration(1000);
    animation -> setStartValue(1);
    animation -> setEndValue(0);
    animation -> start();
    connect(animation, SIGNAL(finished()), this, SLOT(close()));
}

void Signin::on_min_clicked()
{
    this -> showMinimized();
}

// 初始化读取本地账号
void Signin::initRFile()
{
    QFile file("../source/an");
    file.open(QIODevice::WriteOnly | QIODevice::ReadOnly);
    QByteArray t = file.readAll();
    ui -> username -> setText(QString(t));
    file.close();
}

// 账号记录
void Signin::signinWFile()
{
    QFile file("../source/an");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QString username = ui -> username -> text();
    file.write(username.toUtf8());
    file.close();
}

// 登录按钮事件
void Signin::on_signin_clicked()
{
    signinWFile();
    if(netlink == false)
        netLink();
    QString an = ui -> username -> text();
    QString password = ui -> password -> text();
    if (netlink == false || an == nullptr || password == nullptr)
        TikTok();
    else
    {
        QString sendData = an + "|" + password;
        tSocket -> write(sendData.toLatin1());
        QByteArray error = "0";
        connect(tSocket, &QTcpSocket::readyRead,
                                [=]()
                                   {
                                       QByteArray tip = tSocket -> readAll();
                                       if(tip == error)
                                       {
                                           TikTok();
                                       }
                                       else
                                       {
                                           username.prepend(tip);
                                           emit send_to_main(username);
                                           on_close_clicked();
                                       }
                                   });
    }
}

void Signin::on_signup_clicked()
{
    _register.show();
}

// 建立网络连接
void Signin::netLink()
{
    // 网络部分
    tSocket = nullptr;
    tSocket = new QTcpSocket(this);
    // 主动和服务器建立连接
    tSocket -> connectToHost(QHostAddress(QString("xxx.xx.xx.xxx")), 9000);
    connect(tSocket, &QTcpSocket::connected,
           [=]()
               {
                   netlink = true;
               });
    connect(tSocket, &QTcpSocket::disconnected,
           [=]()
               {
                   netlink = false;
               });
}
