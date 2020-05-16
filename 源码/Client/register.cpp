#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    setWindow();
}

Register::~Register()
{
    delete ui;
}

void Register::setWindow()
{
    setWindowIcon(QIcon("../icons/M_icon.png"));
    setWindowTitle("Koala 注册系统");
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    // setAttribute(Qt::WA_TranslucentBackground);

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(), 8, 8);
    setMask(bmp);

    ui -> icon -> setPixmap(QPixmap("../Icons/M_icon.png"));
    ui -> icon -> setScaledContents(true);
    ui -> background -> setPixmap(QPixmap("../images/R_bg.jpg"));

    int wide = width();
    QToolButton *minButton = new QToolButton(this);
    QToolButton *closeButton= new QToolButton(this);
    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    minButton -> setIcon(minPix);
    closeButton -> setIcon(closePix);
    minButton->setGeometry(wide-65,7,20,20);
    closeButton->setGeometry(wide-35,7,20,20);
    minButton->setToolTip(tr("最小化"));
    closeButton->setToolTip(tr("关闭"));

    connect(closeButton, &QPushButton::pressed, this, &Register::windowclosed);
    connect(minButton, &QPushButton::pressed, this, &Register::windowmin);

    // 自定义注册按钮
    QString btnStyle1 =
            "QPushButton{\
                color: rgb(255, 255, 0);\
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                border:1px;\
                border-radius:5px; /*border-radius控制圆角大小*/\
                padding:2px 4px;  \
            }\
            QPushButton:hover{\
                color: rgb(255, 255, 255); \
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                border:1px;  \
                border-radius:5px; /*border-radius控制圆角大小*/\
                padding:2px 4px; \
            }\
            QPushButton:pressed{    \
                color: rgb(255, 255, 0); \
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,200), stop:0.3 rgb(0,0,230), stop:1 rgb(0,0,255));  \
                border:1px;  \
                border-radius:5px; /*border-radius控制圆角大小*/\
                padding:2px 4px; \
            }";
    ui -> username -> setStyleSheet("background:transparent;border-left-width:0;"
                            "border-right-width:0;border-top-width:0;border-bottom:#666 2px solid;");
    ui -> password -> setStyleSheet("background:transparent;border-left-width:0;"
                                "border-right-width:0;border-top-width:0;border-bottom:#666 2px solid;");
    ui -> password1 -> setStyleSheet("background:transparent;border-left-width:0;"
                                "border-right-width:0;border-top-width:0;border-bottom:#666 2px solid;");
    ui -> sure -> setStyleSheet(btnStyle1);
    ui -> password -> setEchoMode(QLineEdit::Password);
    ui -> password1 -> setEchoMode(QLineEdit::Password);

    ui -> username -> setToolTip(tr("为自己起一个炫酷而独特的用户名吧"));
    ui -> password1 -> setToolTip(tr("字母开头且不能小于6位哟！"));
    ui -> password -> setToolTip(tr("与上面的密码保持一致哟~"));

    ui -> frame -> hide();
    ui -> bg -> setPixmap(QPixmap("../images/white.png"));
    ui -> bg -> setScaledContents(true);

    QIcon icon;
    icon.addFile(tr("../images/h1.jpg"));
    ui -> headpic -> setIconSize(QSize(100, 100));
    ui -> headpic -> setIcon(icon);

    icon.addFile(tr("../images/h0.jpg"));
    ui -> h0 -> setIconSize(QSize(60, 60));
    ui -> h0 -> setIcon(icon);

    icon.addFile(tr("../images/h1.jpg"));
    ui -> h1 -> setIconSize(QSize(60, 60));
    ui -> h1 -> setIcon(icon);

    icon.addFile(tr("../images/h2.jpg"));
    ui -> h2 -> setIconSize(QSize(60, 60));
    ui -> h2 -> setIcon(icon);

    icon.addFile(tr("../images/h3.jpg"));
    ui -> h3 -> setIconSize(QSize(60, 60));
    ui -> h3 -> setIcon(icon);

    icon.addFile(tr("../images/h4.jpg"));
    ui -> h4 -> setIconSize(QSize(60, 60));
    ui -> h4 -> setIcon(icon);

    icon.addFile(tr("../images/h5.jpg"));
    ui -> h5 -> setIconSize(QSize(60, 60));
    ui -> h5 -> setIcon(icon);

    icon.addFile(tr("../images/h6.jpg"));
    ui -> h6 -> setIconSize(QSize(60, 60));
    ui -> h6 -> setIcon(icon);

    icon.addFile(tr("../images/h7.jpg"));
    ui -> h7 -> setIconSize(QSize(60, 60));
    ui -> h7 -> setIcon(icon);

    icon.addFile(tr("../images/h8.jpg"));
    ui -> h8 -> setIconSize(QSize(60, 60));
    ui -> h8 -> setIcon(icon);

}
void Register::windowclosed()
{
    this -> close();
}
void Register::windowmin()
{
    this->showMinimized();
}
void Register::mousePressEvent(QMouseEvent* mEvent)
{
    if(mEvent->button()==Qt::LeftButton)
    {
        dragPosition = mEvent->globalPos()-frameGeometry().topLeft();
        mEvent->accept();
    }
}
void Register::mouseMoveEvent(QMouseEvent* mEvent)
{
    if(mEvent->buttons()& Qt::LeftButton)
    {
        move(mEvent->globalPos() - dragPosition);
        mEvent->accept();
    }
}

void Register::on_sure_clicked()
{
    QRegExp regUser("");
    QRegExp regPass("^[a-zA-Z]\\w{5,17}$");

    ui -> tips -> setText("请稍候 ...");
    QString username = ui -> username -> text();
    QString passwd = ui -> password -> text();
    QString passwd1 = ui -> password1 -> text();

    if (username == "" || passwd == "" || passwd1 == "")
        ui -> tips -> setText("填写信息不完整~");
    else if(passwd != passwd1)
        ui -> tips -> setText("密码和确定密码不匹配~");
    // else if (regUser.indexIn(username) != 0 || regPass.indexIn(passwd) != 0)
    else if (regPass.indexIn(passwd) != 0)
        ui -> tips -> setText("信息不符合规范，请重新填写~");
    else
    {
        netLink();

        QString sendData = username + "|" + passwd + "|" + headp;
        tSocket -> write(sendData.toLatin1());
        QByteArray error = "0";
        connect(tSocket, &QTcpSocket::readyRead,
                                       [=]()
                                          {
                                              QByteArray tip = tSocket -> readAll();
                                              if(tip == error)
                                              {
                                                  ui -> tips -> setStyleSheet("color:red;");
                                                  ui -> tips -> setText("注册失败，请检查网络或者用户名被占用！");
                                                  // 主动和对方断开链接
                                                  tSocket -> disconnectFromHost();
                                                  tSocket -> close();
                                              }
                                              else
                                              {
                                                  // 主动和对方断开链接
                                                  tSocket -> disconnectFromHost();
                                                  tSocket -> close();
                                                  ui -> tips -> setStyleSheet("color:green;");
                                                  QString str = "注册成功，请牢记您的用户名和密码";
                                                  ui -> tips -> setText(str);
                                              }
                                          });
    }
}

void Register::on_headpic_clicked()
{
    /* QImage image;
    OpenFile = QFileDialog::getOpenFileName(this,
                                            "please choose an image file",
                                            "../images",
                                            "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)"
                                            );

    QFileInfo OpenFileInfo;
    OpenFileInfo = QFileInfo(OpenFile);
    OpenFilePath = OpenFileInfo.filePath(); */

    if(n % 2 == 0)
    {
        ui -> frame -> show();
        n++;
    }
    else
    {
        ui -> frame -> hide();
        n++;
    }
}

void Register::netLink()
{
    // 网络部分
    tSocket = nullptr;
    tSocket = new QTcpSocket(this);
    // 主动和服务器建立连接
    tSocket -> connectToHost(QHostAddress(QString("xxx.xx.xx.xxx")), 9001);
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

void Register::on_h0_clicked()
{
    QIcon icon;
    icon.addFile(tr("../images/h0.jpg"));
    ui -> headpic -> setIconSize(QSize(100, 100));
    ui -> headpic -> setIcon(icon);
    headp = "0";
    on_headpic_clicked();
}

void Register::on_h1_clicked()
{
    QIcon icon;
    icon.addFile(tr("../images/h1.jpg"));
    ui -> headpic -> setIconSize(QSize(100, 100));
    ui -> headpic -> setIcon(icon);
    headp = "1";
    on_headpic_clicked();
}

void Register::on_h2_clicked()
{
    QIcon icon;
    icon.addFile(tr("../images/h2.jpg"));
    ui -> headpic -> setIconSize(QSize(100, 100));
    ui -> headpic -> setIcon(icon);
    headp = "2";
    on_headpic_clicked();
}

void Register::on_h3_clicked()
{
    QIcon icon;
    icon.addFile(tr("../images/h3.jpg"));
    ui -> headpic -> setIconSize(QSize(100, 100));
    ui -> headpic -> setIcon(icon);
    headp = "3";
    on_headpic_clicked();
}

void Register::on_h4_clicked()
{
    QIcon icon;
    icon.addFile(tr("../images/h4.jpg"));
    ui -> headpic -> setIconSize(QSize(100, 100));
    ui -> headpic -> setIcon(icon);
    headp = "4";
    on_headpic_clicked();
}

void Register::on_h5_clicked()
{
    QIcon icon;
    icon.addFile(tr("../images/h5.jpg"));
    ui -> headpic -> setIconSize(QSize(100, 100));
    ui -> headpic -> setIcon(icon);
    headp = "5";
    on_headpic_clicked();
}

void Register::on_h6_clicked()
{
    QIcon icon;
    icon.addFile(tr("../images/h6.jpg"));
    ui -> headpic -> setIconSize(QSize(100, 100));
    ui -> headpic -> setIcon(icon);
    headp = "6";
    on_headpic_clicked();
}

void Register::on_h7_clicked()
{
    QIcon icon;
    icon.addFile(tr("../images/h7.jpg"));
    ui -> headpic -> setIconSize(QSize(100, 100));
    ui -> headpic -> setIcon(icon);
    headp = "7";
    on_headpic_clicked();
}

void Register::on_h8_clicked()
{
    QIcon icon;
    icon.addFile(tr("../images/h8.jpg"));
    ui -> headpic -> setIconSize(QSize(100, 100));
    ui -> headpic -> setIcon(icon);
    headp = "8";
    on_headpic_clicked();
}
