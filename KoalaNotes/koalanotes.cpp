#include "koalanotes.h"
#include "ui_koalanotes.h"

KoalaNotes::KoalaNotes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::KoalaNotes)
{
    ui->setupUi(this);
    setWindows();
    initVar();
    receive_a_signal();
    bindPages();
    connect(ui -> p1_list, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked0(const QModelIndex &)));
    connect(ui -> p2_list, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked1(const QModelIndex &)));
    connect(ui -> p2_listnotes, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked2(const QModelIndex &)));
    connect(ui -> p3_follow, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked3(const QModelIndex &)));
    connect(ui -> p3_notes, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked4(const QModelIndex &)));
    connect(ui -> p3_share, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked5(const QModelIndex &)));
    connect(ui -> p3_shared, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked6(const QModelIndex &)));
}

KoalaNotes::~KoalaNotes()
{
    if(signin != nullptr)
    {
        delete signin;
        signin = nullptr;
    }
    if(signup != nullptr)
    {
        delete signup;
        signup = nullptr;
    }
    // 主动和对方断开链接
    if(netlink == true)
    {
        tSocket -> disconnectFromHost();
        tSocket -> close();
    }
    delete ui;
}

void KoalaNotes::receive_a_signal()
{
    if(signin != nullptr)
        connect(signin, SIGNAL(send_to_main(QString)), this, SLOT(receive_from_signin(QString)));
}

// 接收登录界面的消息
void KoalaNotes::receive_from_signin(QString recv_msg)
{
    int i;
    for (i = 0; i < recv_msg.size(); ++i)
    {
        if(recv_msg.at(i) == '|')
            break;
        mine.username += recv_msg.at(i);
    }

    ui -> M_username -> setText(mine.username);
    QPixmap head;
    mine.hdno = recv_msg.at(i + 1);
    int n = mine.hdno.toInt();
    switch(n)
    {
    case 0:
        head = QPixmap("../images/h0.jpg").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->M_head->setPixmap(head);
        break;
    case 1:
        head = QPixmap("../images/h1.jpg").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->M_head->setPixmap(head);
        break;
    case 2:
        head = QPixmap("../images/h2.jpg").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->M_head->setPixmap(head);
        break;
    case 3:
        head = QPixmap("../images/h3.jpg").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->M_head->setPixmap(head);
        break;
    case 4:
        head = QPixmap("../images/h4.jpg").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->M_head->setPixmap(head);
        break;
    case 5:
        head = QPixmap("../images/h5.jpg").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->M_head->setPixmap(head);
        break;
    case 6:
        head = QPixmap("../images/h6.jpg").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->M_head->setPixmap(head);
        break;
    case 7:
        head = QPixmap("../images/h7.jpg").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->M_head->setPixmap(head);
        break;
    case 8:
        head = QPixmap("../images/h8.jpg").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->M_head->setPixmap(head);
        break;
    }
    ui -> headopt -> hide();
    h_click = 0;
    login = true;

    if(netlink == false)
        netLink();
    readyRead();
    {
        QString sendData2 = "0|" + mine.username;
        QString sendData1 = QString::number(sendData2.size());
        int tail = 100 - (sendData1.size());
        for(int i = 0; i < tail; ++i)
            sendData1 += '*';

        tSocket -> write(sendData1.toLatin1());
        tSocket -> write(sendData2.toLatin1());
    }
    {
        QString sendData2 = "4|" + mine.username;
        QString sendData1 = QString::number(sendData2.size());
        int tail = 100 - (sendData1.size());
        for(int i = 0; i < tail; ++i)
            sendData1 += '*';

        tSocket -> write(sendData1.toLatin1());
        tSocket -> write(sendData2.toLatin1());
    }
}

void KoalaNotes::setWindows()
{
    setWindowIcon(QIcon("../icons/M_icon.png"));
    // 设置窗体无边框、背景透明、圆角窗口
    setWindowFlags(Qt::FramelessWindowHint);
    setHeadPicture();

    const QString closeStyle = "\
                    QPushButton{\
                        color: rgb(166, 166, 166);\
                        border:1px;\
                    }\
                    QPushButton:hover{\
                        background:rgb(255, 0, 0); \
                        color: rgb(255, 255, 255);\
                    }\
                    QPushButton:pressed{\
                        color: rgb(144, 144, 144);\
                    }";
    const QString minStyle = "\
                    QPushButton{\
                        color: rgb(166, 166, 166);\
                        border:1px;\
                    }\
                    QPushButton:hover{\
                        color: rgb(0, 0, 255);\
                    }\
                    QPushButton:pressed{\
                        background:rgb(0, 0, 255); \
                        color: rgb(255, 255, 255);\
                    }";
    ui -> M_close -> setStyleSheet(closeStyle);
    ui -> M_min -> setStyleSheet(minStyle);

    QIcon icon;
    ui -> M_top -> setFlat(true);
    icon.addFile(tr("../Icons/M_top.png"));
    ui -> M_top -> setIconSize(QSize(25, 25));
    ui -> M_top -> setIcon(icon);
    ui -> M_chbg -> setFlat(true);
    icon.addFile(tr("../Icons/M_chbg.png"));
    ui -> M_chbg -> setIconSize(QSize(25, 25));
    ui -> M_chbg -> setIcon(icon);
    ui -> M_icon -> setPixmap(QPixmap("../Icons/M_icon.png"));
    ui -> M_icon -> setScaledContents(true);

    ui -> W_bg -> setPixmap(QPixmap("../images/headopt.png"));
    ui -> W_bg -> setScaledContents(true);
    QString wStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;\
                    border-radius:2px; /*border-radius控制圆角大小*/\
                    padding:2px 4px;  \
                }\
                QPushButton:hover{\
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }\
                QPushButton:pressed{    \
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,200), stop:0.3 rgb(0,0,230), stop:1 rgb(0,0,255));  \
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }";
    ui -> W_login -> setStyleSheet(wStyle);
    ui -> W_logup -> setStyleSheet(wStyle);
    ui -> W_cancel -> setStyleSheet(wStyle);


    ui -> M_account -> setFlat(true);
    icon.addFile(tr("../Icons/M_account.png"));
    ui -> M_account -> setIconSize(QSize(25, 25));
    ui -> M_account -> setIcon(icon);

    ui -> M_syn -> setFlat(true);
    icon.addFile(tr("../Icons/M_syn.png"));
    ui -> M_syn -> setIconSize(QSize(25, 25));
    ui -> M_syn -> setIcon(icon);

    const QString asStyle = "\
                    QPushButton{\
                        color: rgb(166, 166, 166);\
                        border:1px;\
                    }\
                    QPushButton:hover{\
                        color: rgb(0, 0, 255);\
                    }\
                    QPushButton:pressed{\
                        color: rgb(255, 255, 255);\
                    }";
    ui -> M_account1 -> setStyleSheet(asStyle);
    ui -> M_syn1 -> setStyleSheet(asStyle);
    ui -> M_header -> setStyleSheet(asStyle);
    QString pbtnStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;\
                    border-radius:2px; \
                    padding:2px 4px;  \
                }\
                QPushButton:hover{\
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }\
                QPushButton:pressed{    \
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,200), stop:0.3 rgb(0,0,230), stop:1 rgb(0,0,255));  \
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }";
    ui -> M_zhuxiao -> setStyleSheet(pbtnStyle);
    ui -> M_zhuxiao -> hide();

    QString newStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;\
                    border-radius:10px; \
                    padding:2px 4px;  \
                }\
                QPushButton:hover{\
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;  \
                    border-radius:10px; \
                    padding:2px 4px; \
                }\
                QPushButton:pressed{    \
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,200), stop:0.3 rgb(0,0,230), stop:1 rgb(0,0,255));  \
                    border:1px;  \
                    border-radius:10px; \
                    padding:2px 4px; \
                }";
    icon.addFile(tr("../Icons/M_new.png"));
    ui -> M_new -> setIconSize(QSize(20, 20));
    ui -> M_new -> setIcon(icon);
    ui -> M_new -> setStyleSheet(newStyle);

    setButtonStyle();
    icon.addFile(tr("../Icons/M_mynotes.png"));
    ui -> M_mynotes -> setIconSize(QSize(25, 25));
    ui -> M_mynotes -> setIcon(icon);
    icon.addFile(tr("../Icons/M_follow.png"));
    ui -> M_follow -> setIconSize(QSize(25, 25));
    ui -> M_follow -> setIcon(icon);
    icon.addFile(tr("../Icons/M_prishare.png"));
    ui -> M_prishare -> setIconSize(QSize(25, 25));
    ui -> M_prishare -> setIcon(icon);
    icon.addFile(tr("../Icons/M_more.png"));
    ui -> M_more -> setIconSize(QSize(20, 20));
    ui -> M_more -> setIcon(icon);

    ui -> p1_p0 -> setChecked(true);
    ui -> p4_mail -> setPixmap(QPixmap("../Icons/mail.png"));
    ui -> p4_mail -> setScaledContents(true);
    ui -> P4_github -> setPixmap(QPixmap("../Icons/github.png"));
    ui -> P4_github -> setScaledContents(true);
    ui -> p4_github1 -> setOpenExternalLinks(true);
    ui -> p4_github1 -> setText("<style> a {text-decoration: none} </style> <a href=\"https://github.com/oubc\">https://github.com/oubc/KoalaNotes");

    QMovie *myMovie = new QMovie("../Icons/more.gif");
    ui -> p4_movie -> setMovie(myMovie);
    myMovie -> start();
    ui -> p4_movie -> setScaledContents(true);

    page0Content();
    setToolTips();
}

void KoalaNotes::initVar()
{
    login = false;
    ontop = false;
    netlink = false;
    signup = nullptr;
    signin = nullptr;
    h_click = 0;
    a_click = 0;
    s_click = 0;
    bg_click = 0;
    g_click = 0;
    c_click = 0;
    pModel = new QStandardItemModel();
    pModel1 = new QStandardItemModel();
    pModel2 = new QStandardItemModel();
    pModel3 = new QStandardItemModel();
    rollBar = "QScrollBar:vertical"
              "{"
              "width:8px;"
              "background:rgba(0,0,0,0%);"
              "margin:0px,0px,0px,0px;"
              "padding-top:9px;"
              "padding-bottom:9px;"
              "}"
              "QScrollBar::handle:vertical"
              "{"
              "width:8px;"
              "background:rgba(0,0,0,25%);"
              " border-radius:4px;"
              "min-height:20;"
              "}"
              "QScrollBar::handle:vertical:hover"
              "{"
              "width:8px;"
              "background:rgba(0,0,0,50%);"
              " border-radius:4px;"
              "min-height:20;"
              "}"
              "QScrollBar::add-line:vertical"
              "{"
              "height:9px;width:8px;"
              "border-image:url(:/images/a/3.png);"
              "subcontrol-position:bottom;"
              "}"
              "QScrollBar::sub-line:vertical"
              "{"
              "height:9px;width:8px;"
              "border-image:url(:/images/a/1.png);"
              "subcontrol-position:top;"
              "}"
              "QScrollBar::add-line:vertical:hover"
              "{"
              "height:9px;width:8px;"
              "border-image:url(:/images/a/4.png);"
              "subcontrol-position:bottom;"
              "}"
              "QScrollBar::sub-line:vertical:hover"
              "{"
              "height:9px;width:8px;"
              "border-image:url(:/images/a/2.png);"
              "subcontrol-position:top;"
              "}"
              "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
              "{"
              "background:rgba(0,0,0,10%);"
              "border-radius:4px;"
              "}";
}

// 设置提示信息
void KoalaNotes::setToolTips()
{
    ui -> M_min -> setToolTip(tr("最小化"));
    ui -> M_close -> setToolTip(tr("关闭"));
    ui -> M_top -> setToolTip(tr("置顶"));
}

void KoalaNotes::setHeadPicture()
{
    QPixmap pixBack("../images/headborder.png");
    ui->M_head->setMask(pixBack.mask());
    ui->M_headb->setStyleSheet("border-image:url(../images/headborder.png)");
    QPixmap head = QPixmap("../images/clicklogin.png").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->M_head->setPixmap(head);
}

void KoalaNotes::setButtonStyle()
{
    const QString btnStyle = "\
                    QPushButton{\
                        color: rgb(0, 0, 0);\
                        border:1px;\
                    }\
                    QPushButton:hover{\
                        color: rgb(0, 0, 255);\
                    }\
                    QPushButton:pressed{\
                        color: rgb(255, 255, 255);\
                    }";
    ui -> M_mynotes -> setStyleSheet(btnStyle);
    ui -> M_follow -> setStyleSheet(btnStyle);
    ui -> M_prishare -> setStyleSheet(btnStyle);
    ui -> M_more -> setStyleSheet(btnStyle);
    ui -> headopt -> hide();
    ui -> M_zhuxiao -> hide();
    h_click = 0;
}

void KoalaNotes::on_M_chbg_clicked()
{
    if(bg_click % 3 == 0)
    {
        ui -> M_bg -> setPixmap(QPixmap("../images/bg1.jpg"));
        bg_click++;
    }
    else if(bg_click % 3 == 1)
    {
        ui -> M_bg -> setPixmap(QPixmap("../images/bg0.jpg"));
        bg_click++;
    }
    else
    {
        ui -> M_bg -> setPixmap(QPixmap(""));
        bg_click++;
    }
}

void KoalaNotes::bindPages()
{
    ui -> listWidget -> setCurrentIndex(0);
    connect(ui -> M_new, &QPushButton::released,this,[=](){ui -> listWidget -> setCurrentIndex(1);});
    connect(ui -> M_mynotes, &QPushButton::released,this,[=](){ui -> listWidget -> setCurrentIndex(1);});
    connect(ui -> M_follow, &QPushButton::released,this,[=](){ui -> listWidget -> setCurrentIndex(2);});
    connect(ui -> M_prishare, &QPushButton::released,this,[=](){ui -> listWidget -> setCurrentIndex(3);});
    connect(ui -> M_more, &QPushButton::released,this,[=](){ui -> listWidget -> setCurrentIndex(4);});
}

void KoalaNotes::mousePressEvent(QMouseEvent* mEvent)
{
    if(mEvent->button()==Qt::LeftButton)
    {
        dragPosition = mEvent->globalPos()-frameGeometry().topLeft();
        mEvent->accept();
    }
}

void KoalaNotes::mouseMoveEvent(QMouseEvent* mEvent)
{
    if(mEvent->buttons()& Qt::LeftButton)
    {
        move(mEvent->globalPos() - dragPosition);
        mEvent->accept();
    }
}

void KoalaNotes::on_M_close_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation -> setDuration(1000);
    animation -> setStartValue(1);
    animation -> setEndValue(0);
    animation -> start();
    connect(animation, SIGNAL(finished()), this, SLOT(close()));
}

void KoalaNotes::on_M_min_clicked()
{
    this -> showMinimized();
}

void KoalaNotes::on_M_top_clicked()
{
    QIcon icon;
    if (ontop == false)
    {
        icon.addFile(tr("../Icons/M_top1.png"));
        ui -> M_top -> setIconSize(QSize(25, 25));
        ui -> M_top -> setIcon(icon);
        ontop = true;
        // 实现置顶
        this -> setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
        this -> show();
    }
    else
    {
        icon.addFile(tr("../Icons/M_top.png"));
        ui -> M_top -> setIconSize(QSize(25, 25));
        ui -> M_top -> setIcon(icon);
        ontop = false;
        // 取消置顶
        this -> setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
        this -> show();
    }
}

void KoalaNotes::on_M_headb_clicked() {}

void KoalaNotes::on_M_header_clicked()
{
    if(h_click %2 == 0)
    {
        if(login == false)
        {

            ui -> W_login -> show();
            ui -> W_logup -> show();
            ui -> W_cancel -> hide();
        }
        else
        {
            ui -> W_cancel -> show();
            ui -> W_login -> hide();
            ui -> W_logup -> hide();
        }
        ui -> headopt -> show();
    }
    else
        ui -> headopt -> hide();
    h_click++;
}

void KoalaNotes::on_M_new_clicked()
{
    newnotes = true;

    QDateTime dateTime(QDateTime::currentDateTime());
    QString qStr = dateTime.toString("MM-dd hh:mm:ss");

    pModel -> clear();
    MuItemData itemData;
    pItem = new QStandardItem;
    itemData.iconPath = "";
    itemData.title = "新文档";
    itemData.dates = qStr;
    pItem -> setData(QVariant::fromValue(itemData), Qt::UserRole + 1);
    pModel -> appendRow(pItem);
    MuItemDelegate *pItemDelegate = new MuItemDelegate(0, this);
    ui-> p1_list -> setItemDelegate(pItemDelegate);
    ui-> p1_list -> setModel(pModel);

    ui -> p1_title -> show();
    ui -> p1_title1 -> show();
    ui -> p1_text -> show();
    ui -> p1_public -> show();
    ui -> p1_private -> show();
    ui -> p1_title -> clear();
    ui -> p1_text -> clear();
    ui -> p1_modify -> hide();
    ui -> p1_p0 -> hide();
    ui -> p1_p1 -> hide();
    on_M_mynotes_clicked();
}

void KoalaNotes::on_M_mynotes_clicked()
{
    setButtonStyle();
    QString btnStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200,186,251), stop:0.3 rgb(200,186,251), stop:1 rgb(200,186,251));\
                    border:1px;\
                    padding:2px 4px;  \
                }";
    ui -> M_mynotes -> setStyleSheet(btnStyle);
    if(netlink == false)
    {
        ui -> page1 -> hide();
    }

    if(netlink == true)
    {
        ui -> p1_list ->show();
        QString sendData2 = "0|" + mine.username;
        QString sendData1 = QString::number(sendData2.size());
        int tail = 100 - (sendData1.size());
        for(int i = 0; i < tail; ++i)
            sendData1 += '*';

        tSocket -> write(sendData1.toLatin1());
        tSocket -> write(sendData2.toLatin1());
        page1Content();
    }
    else
        QMessageBox::information(this, tr("KoalaNotes"), QStringLiteral("未登陆或连接网络失败!"), QMessageBox::Ok);
}

void KoalaNotes::on_M_follow_clicked()
{
    setButtonStyle();
    QString btnStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200,186,251), stop:0.3 rgb(200,186,251), stop:1 rgb(200,186,251));\
                    border:1px;\
                    padding:2px 4px;  \
                }";
    ui -> M_follow -> setStyleSheet(btnStyle);

    if(netlink == false)
    {
        ui -> page2 -> hide();
    }

    if(netlink == true)
    {
        ui -> p2_list -> show();
        ui -> p2_listnotes -> hide();
        QString sendData2 = "4|" + mine.username;
        QString sendData1 = QString::number(sendData2.size());
        int tail = 100 - (sendData1.size());
        for(int i = 0; i < tail; ++i)
            sendData1 += '*';

        tSocket -> write(sendData1.toLatin1());
        tSocket -> write(sendData2.toLatin1());
        page2Content();

        ui -> p2_title -> hide();
        ui -> p2_title1 -> hide();
        ui -> p2_text -> hide();
        ui -> p2_good -> hide();
        ui -> p2_good1 -> hide();
        ui -> p2_comment -> hide();
        ui -> p2_widget -> hide();
        c_click = 0;
    }
    else
        QMessageBox::information(this, tr("KoalaNotes"), QStringLiteral("未登陆或连接网络失败!"), QMessageBox::Ok);
}

void KoalaNotes::on_M_prishare_clicked()
{
    setButtonStyle();
    QString btnStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200,186,251), stop:0.3 rgb(200,186,251), stop:1 rgb(200,186,251));\
                    border:1px;\
                    padding:2px 4px;  \
                }";
    ui -> M_prishare -> setStyleSheet(btnStyle);
    ui -> shared -> hide();
    ui -> p3_return1 -> hide();
    ui -> p3_title1 -> hide();
    ui -> p3_text1 -> hide();
    ui -> p3_return2 -> hide();
    ui -> p3_title2 -> hide();
    ui -> p3_text2 -> hide();
    ui -> p3_share -> show();
    ui -> p3_shared -> show();
    s_click = 0;

    if(netlink == false)
    {
        ui -> page3 -> hide();
    }

    if(netlink == true)
    {
        QString sendData2 = "9|" + mine.username;
        QString sendData1 = QString::number(sendData2.size());
        int tail = 100 - (sendData1.size());
        for(int i = 0; i < tail; ++i)
            sendData1 += '*';

        tSocket -> write(sendData1.toLatin1());
        tSocket -> write(sendData2.toLatin1());
        page3Content();
    }
    else
        QMessageBox::information(this, tr("KoalaNotes"), QStringLiteral("未登陆或连接网络失败!"), QMessageBox::Ok);
}

void KoalaNotes::on_M_more_clicked()
{
    setButtonStyle();
    QString btnStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200,186,251), stop:0.3 rgb(200,186,251), stop:1 rgb(200,186,251));\
                    border:1px;\
                    padding:2px 4px;  \
                }";
    ui -> M_more -> setStyleSheet(btnStyle);
}

void KoalaNotes::on_W_login_clicked()
{
    if(signin != nullptr)
    {
        delete signin;
        signin = new Signin;
        connect(signin, SIGNAL(send_to_main(QString)), this, SLOT(receive_from_signin(QString)));
    }
    else
    {
        signin = new Signin;
        connect(signin, SIGNAL(send_to_main(QString)), this, SLOT(receive_from_signin(QString)));
    }
    signin -> show();
}

void KoalaNotes::on_W_logup_clicked()
{
    if(signup != nullptr)
    {
        delete signup;
        signup = nullptr;
    }
    signup = new Register;
    signup -> show();
}

void KoalaNotes::on_W_cancel_clicked()
{
    mine.username = "";
    mine.hdno = "";
    login = false;
    h_click++;
    on_M_header_clicked();
    QPixmap head = QPixmap("../images/clicklogin.png").scaled(QSize(ui->M_head->width(),ui->M_head->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui -> M_head -> setPixmap(head);
    ui -> M_username -> setText("");
    if(netlink == true)
    {
        tSocket -> disconnectFromHost();
        tSocket -> close();
    }
}

void KoalaNotes::page0Content()
{
    ui -> p0_text -> setStyleSheet("QTextBrowser{border-width:0;border-style:outset}");
    QDate date(QDate::currentDate());
    int month = date.month();
    int day = date.day();

    ui -> p0_greet -> setStyleSheet("color:blue;");
    QTime time(QTime::currentTime());
    int hour = time.hour();
    if(hour >= 0 && hour < 12)
        ui -> p0_greet -> setText("早安！");
    else if(hour >= 12 && hour <= 17)
        ui -> p0_greet -> setText("午安！");
    else
        ui -> p0_greet -> setText("晚上好呀");

    ui -> p0_day -> setText(QString::number(day));
    ui -> p0_day -> setStyleSheet("color:blue;");
    ui -> p0_month -> setText(QString::number(month));
    ui -> p0_month -> setStyleSheet("color:blue;");

    QTime timerand;
    timerand = QTime::currentTime();
    qsrand(timerand.msec() + timerand.second() * 1000);
    int n = qrand() % 9;
    switch(n)
    {
    case 0:
        ui -> p0_picture -> setPixmap(QPixmap("../images/img0.jpg"));
        break;
    case 1:
        ui -> p0_picture -> setPixmap(QPixmap("../images/img1.jpg"));
        break;
    case 2:
        ui -> p0_picture -> setPixmap(QPixmap("../images/img2.jpg"));
        break;
    case 3:
        ui -> p0_picture -> setPixmap(QPixmap("../images/img3.jpg"));
        break;
    case 4:
        ui -> p0_picture -> setPixmap(QPixmap("../images/img4.jpg"));
        break;
    case 5:
        ui -> p0_picture -> setPixmap(QPixmap("../images/img5.jpg"));
        break;
    case 6:
        ui -> p0_picture -> setPixmap(QPixmap("../images/img6.jpg"));
        break;
    case 7:
        ui -> p0_picture -> setPixmap(QPixmap("../images/img7.jpg"));
        break;
    case 8:
        ui -> p0_picture -> setPixmap(QPixmap("../images/img8.jpg"));
        break;
    }

    QString str[10]{
        "人生就是一次又一次的选择，每选择一次，就遗憾一次，放弃一次。",
        "生活很丧，请你务必快乐！",
        "你要学会前进，人群川流不息，在身边像晃动的电影胶片，你怀揣自己的颜色，往一心要到的地方。",
        "趟出这片孤寂，就趟过生长；遇见风起水浪，就遇过虚妄；忍住顷刻回望，就忍过恓惶；去过远方，所以不再相信远方。",
        "一万年太久，只争朝夕。",
        "要是一个人，能充满信心地朝他理想的方向去做，下定决心过他所想过的生活，他就一定会得到意外的成功。",
        "常以为别人在注意你，或希望别人注意你的人，会生活的比较烦恼。",
        "科学给青年以营养，给老人以慰藉；她让幸福的生活锦上添花，她在你不幸的时刻保护着你。",
        "从青年人的角度看，生活是一个无穷无尽的遥远未来；从老年人的角度看，生活宛如一个非常短暂的往昔。",
        "即使在最寂寞的时候，即使泪水使我看不清世界的样子，我依然在聆听。",
    };
    timerand = QTime::currentTime();
    qsrand(timerand.msec() + timerand.second() * 1000);
    n = qrand() % 10;
    ui -> p0_text -> setText(str[n]);
}

void KoalaNotes::page1Content()
{
    ui -> p1_search -> setPlaceholderText("按标题搜索");
    ui -> p1_search1 -> setFlat(true);
    QIcon icon;
    icon.addFile(tr("../Icons/p1_search.png"));
    ui -> p1_search1 -> setIconSize(QSize(30, 30));
    ui -> p1_search1 -> setIcon(icon);

    ui -> p1_title -> setPlaceholderText("在这里输入标题~");
    ui -> p1_list -> setFrameShape(QListWidget::NoFrame);
    ui -> p1_list -> setStyleSheet(rollBar);
    ui -> p1_text -> setStyleSheet(rollBar);
    QString p1btnStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;\
                    border-radius:2px; \
                    padding:2px 4px;  \
                }\
                QPushButton:hover{\
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }\
                QPushButton:pressed{    \
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,200), stop:0.3 rgb(0,0,230), stop:1 rgb(0,0,255));  \
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }";
    ui -> p1_public -> setStyleSheet(p1btnStyle);
    ui -> p1_private -> setStyleSheet(p1btnStyle);
    ui -> p1_modify -> setStyleSheet(p1btnStyle);

    if(newnotes != true)
    {
        ui -> p1_title -> hide();
        ui -> p1_title1 -> hide();
        ui -> p1_text -> hide();
        ui -> p1_public -> hide();
        ui -> p1_private -> hide();
        ui -> p1_modify -> hide();
        ui -> p1_p0 -> hide();
        ui -> p1_p1 -> hide();
        pModel -> clear();
    }
    newnotes = false;
    MuItemData itemData;
    for(int i = 0; i < listnotes.size(); ++i)
    {
        pItem = new QStandardItem;
        itemData.iconPath = "";
        itemData.title = listnotes.at(i).title;
        itemData.dates = listnotes.at(i).dates;

        pItem -> setData(QVariant::fromValue(itemData), Qt::UserRole + 1);
        pModel -> appendRow(pItem);
    }

    MuItemDelegate *pItemDelegate = new MuItemDelegate(0, this);
    ui-> p1_list -> setItemDelegate(pItemDelegate);
    ui-> p1_list -> setModel(pModel);
}

void KoalaNotes::page2Content()
{
    QString p1btnStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;\
                    border-radius:2px; \
                    padding:2px 4px;  \
                }\
                QPushButton:hover{\
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }\
                QPushButton:pressed{    \
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,200), stop:0.3 rgb(0,0,230), stop:1 rgb(0,0,255));  \
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }";
    ui -> p2_addfollow -> setStyleSheet(p1btnStyle);
    ui -> p2_addsure -> setStyleSheet(p1btnStyle);
    ui -> p2_title -> setReadOnly(true);
    ui -> p2_text -> setReadOnly(true);
    ui -> addfollow -> hide();
    ui -> p2_list -> setStyleSheet(rollBar);
    ui -> p2_listnotes -> setStyleSheet(rollBar);
    ui -> p2_text -> setStyleSheet(rollBar);

    ui -> p2_good -> setFlat(true);
    QIcon icon;
    icon.addFile(tr("../Icons/"
                    ""
                    ""
                    ".png"));
    ui -> p2_good -> setIconSize(QSize(40, 40));
    ui -> p2_good -> setIcon(icon);

    QString p2btnStyle =
                "QPushButton{\
                    color: rgb(0, 0, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200,200,200), stop:0.3 rgb(200,200,200), stop:1 rgb(200,200,200));\
                    border:1px;\
                    border-radius:2px; \
                    padding:2px 4px;  \
                }\
                QPushButton:hover{\
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200,200,200), stop:0.3 rgb(200,200,200), stop:1 rgb(200,200,200));\
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }\
                QPushButton:pressed{    \
                    color: rgb(0, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200,200,200), stop:0.3 rgb(200,200,200), stop:1 rgb(200,200,200));  \
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }";

    ui -> p2_comment -> setStyleSheet(p2btnStyle);
    icon.addFile(tr("../Icons/p2_comment.png"));
    ui -> p2_comment -> setIconSize(QSize(20, 20));
    ui -> p2_comment -> setIcon(icon);

    ui -> p2_bg -> setPixmap(QPixmap("../images/comment_bg.jpg"));

    ui -> comment_sure -> setStyleSheet(p2btnStyle);
    ui -> comment_text -> setStyleSheet(rollBar);
    ui -> comment_edit -> setStyleSheet(rollBar);

    a_click = 0;
    ui -> p2_addbg -> setPixmap(QPixmap("../images/addbg.jpg"));

    pModel1 -> clear();
    MuItemData itemData1;
    for(int i = 0; i < listfo.size(); ++i)
    {
        pItem1 = new QStandardItem;
        QString path = "../images/h" + listfo.at(i).hdno + ".jpg";
        itemData1.iconPath = path;
        itemData1.title = listfo.at(i).username;
        itemData1.dates = "点击查看ta的发布吧~";

        pItem1 -> setData(QVariant::fromValue(itemData1), Qt::UserRole + 1);
        pModel1 -> appendRow(pItem1);
    }

    MuItemDelegate *pItemDelegate1 = new MuItemDelegate(40, this);
    ui-> p2_list -> setItemDelegate(pItemDelegate1);
    ui-> p2_list -> setModel(pModel1);
}

void KoalaNotes::page2ContentNotes()
{
    pModel1 -> clear();
    MuItemData itemData1;

    pItem1 = new QStandardItem;
    itemData1.iconPath = "../Icons/return.png";
    itemData1.title = "返回关注列表";
    itemData1.dates = "点击返回关注列表";

    pItem1 -> setData(QVariant::fromValue(itemData1), Qt::UserRole + 1);
    pModel1 -> appendRow(pItem1);

    for(int i = 0; i < listfonotes.size(); ++i)
    {
        pItem1 = new QStandardItem;
        itemData1.iconPath = "";
        itemData1.title = listfonotes.at(i).title;
        itemData1.dates = listfonotes.at(i).dates;

        pItem1 -> setData(QVariant::fromValue(itemData1), Qt::UserRole + 1);
        pModel1 -> appendRow(pItem1);
    }

    MuItemDelegate *pItemDelegate1 = new MuItemDelegate(40, this);
    ui-> p2_listnotes -> setItemDelegate(pItemDelegate1);
    ui-> p2_listnotes -> setModel(pModel1);
}

void KoalaNotes::page3Content()
{
    QString p3btnStyle =
                "QPushButton{\
                    color: rgb(255, 255, 255); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;\
                    border-radius:2px; \
                    padding:2px 4px;  \
                }\
                QPushButton:hover{\
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }\
                QPushButton:pressed{    \
                    color: rgb(255, 255, 0); \
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,200), stop:0.3 rgb(0,0,230), stop:1 rgb(0,0,255));  \
                    border:1px;  \
                    border-radius:2px; \
                    padding:2px 4px; \
                }";
    ui -> p3_wantshare -> setStyleSheet(p3btnStyle);
    ui -> p3_share -> setStyleSheet(rollBar);
    ui -> p3_shared -> setStyleSheet(rollBar);
    ui -> p3_follow -> setStyleSheet(rollBar);
    ui -> p3_notes -> setStyleSheet(rollBar);
    ui -> p3_text1 -> setStyleSheet(rollBar);
    ui -> p3_text2 -> setStyleSheet(rollBar);
    ui -> shared_bg -> setPixmap(QPixmap("../images/shared_bg.jpg"));
    ui -> p3_notes -> hide();
    ui -> p3_text1 -> setReadOnly(true);
    ui -> p3_text2 -> setReadOnly(true);

    QIcon icon;
    icon.addFile(tr("../Icons/return1.png"));
    ui -> p3_return1 -> setFlat(true);
    ui -> p3_return1 -> setIconSize(QSize(35, 35));
    ui -> p3_return1 -> setIcon(icon);

    ui -> p3_return2 -> setFlat(true);
    ui -> p3_return2 -> setIconSize(QSize(35, 35));
    ui -> p3_return2 -> setIcon(icon);

    pModel -> clear();
    MuItemData itemData;
    for(int i = 0; i < share.size(); ++i)
    {
        if(share.at(i).myshare == "1" && share.at(i).username != mine.username)
        {
            pItem = new QStandardItem;
            itemData.iconPath = "../images/h" + share.at(i).hdno + ".jpg";
            itemData.title = share.at(i).username + ": " + share.at(i).title;
            itemData.dates = share.at(i).text;

            pItem -> setData(QVariant::fromValue(itemData), Qt::UserRole + 1);
            pModel -> appendRow(pItem);
        }
    }

    MuItemDelegate *pItemDelegate = new MuItemDelegate(40, this);
    ui-> p3_share -> setItemDelegate(pItemDelegate);
    ui-> p3_share -> setModel(pModel);

    pModel1 -> clear();
    MuItemData itemData1;
    for(int i = 0; i < share.size(); ++i)
    {
        if(share.at(i).myshare == "0" && share.at(i).username != mine.username)
        {
            pItem1 = new QStandardItem;
            itemData1.iconPath = "../images/h" + share.at(i).hdno + ".jpg";
            itemData1.title = share.at(i).username + ": " + share.at(i).title;
            itemData1.dates = share.at(i).text;

            pItem1 -> setData(QVariant::fromValue(itemData1), Qt::UserRole + 1);
            pModel1 -> appendRow(pItem1);
        }
    }

    MuItemDelegate *pItemDelegate1 = new MuItemDelegate(40, this);
    ui-> p3_shared -> setItemDelegate(pItemDelegate1);
    ui-> p3_shared -> setModel(pModel1);

    pModel2 -> clear();
    MuItemData itemData2;
    for(int i = 0; i < listfo.size(); ++i)
    {
        pItem2 = new QStandardItem;
        itemData2.iconPath = "../images/h" + listfo.at(i).hdno + ".jpg";
        itemData2.title = listfo.at(i).username;
        itemData2.dates = "点击与ta共享吧~";

        pItem2 -> setData(QVariant::fromValue(itemData2), Qt::UserRole + 1);
        pModel2 -> appendRow(pItem2);
    }

    MuItemDelegate *pItemDelegate2 = new MuItemDelegate(40, this);
    ui-> p3_follow -> setItemDelegate(pItemDelegate2);
    ui-> p3_follow -> setModel(pModel2);

    pModel3 -> clear();
    MuItemData itemData3;
    for(int i = 0; i < listnotes.size(); ++i)
    {
        pItem3 = new QStandardItem;
        itemData3.iconPath = "";
        itemData3.title = listnotes.at(i).title;
        itemData3.dates = listnotes.at(i).dates;

        pItem3 -> setData(QVariant::fromValue(itemData3), Qt::UserRole + 1);
        pModel3 -> appendRow(pItem3);
    }

    MuItemDelegate *pItemDelegate3 = new MuItemDelegate(0, this);
    ui-> p3_notes -> setItemDelegate(pItemDelegate3);
    ui-> p3_notes -> setModel(pModel3);
}

void KoalaNotes::page4Content()
{}

void KoalaNotes::itemClicked0(const QModelIndex &index)
{
    ui -> p1_title -> show();
    ui -> p1_title1 -> show();
    ui -> p1_text -> show();
    ui -> p1_public -> hide();
    ui -> p1_private -> hide();
    ui -> p1_modify -> show();
    ui -> p1_p0 -> show();
    ui -> p1_p1 -> show();
    ui -> M_zhuxiao -> hide();

    QVariant var = index.data(Qt::UserRole+1);
    MuItemData itemData = var.value<MuItemData>();

    QString p1_title = itemData.title;
    QString dates = itemData.dates;

    raw_title = itemData.title;
    raw_date = itemData.dates;

    ui -> p1_title -> setText(p1_title);


    QString sendData2 = "1|" + mine.username + "|" + p1_title + "|" + dates;
    QString sendData1 = QString::number(sendData2.toUtf8().size());
    int tail = 100 - (sendData1.size());
    for(int i = 0; i < tail; ++i)
        sendData1 += '*';

    tSocket -> write(sendData1.toLatin1());
    tSocket -> write(sendData2.toUtf8());
}

void KoalaNotes::itemClicked1(const QModelIndex &index)
{
    ui -> addfollow -> hide();
    a_click = 0;

    ui -> p2_list -> hide();
    ui -> p2_listnotes -> show();
    ui -> M_zhuxiao -> hide();

    QVariant var = index.data(Qt::UserRole+1);
    MuItemData itemData = var.value<MuItemData>();

    QString p2_title = itemData.title;
    clickUsername = p2_title;

    QString dates = itemData.dates;

    QString sendData2 = "5|" + p2_title;
    QString sendData1 = QString::number(sendData2.toUtf8().size());
    int tail = 100 - (sendData1.size());
    for(int i = 0; i < tail; ++i)
        sendData1 += '*';

    tSocket -> write(sendData1.toLatin1());
    tSocket -> write(sendData2.toUtf8());
    page2ContentNotes();
}

void KoalaNotes::itemClicked2(const QModelIndex &index)
{
    ui -> addfollow -> hide();
    ui -> M_zhuxiao -> hide();
    a_click = 0;

    QIcon icon;
    icon.addFile(tr("../Icons/p2_good.png"));
    ui -> p2_good -> setIconSize(QSize(40, 40));
    ui -> p2_good -> setIcon(icon);

    QVariant var = index.data(Qt::UserRole+1);
    MuItemData itemData = var.value<MuItemData>();

    QString p2_title = itemData.title;
    QString dates = itemData.dates;

    if(p2_title == "返回关注列表")
    {
        ui -> p2_title -> clear();
        ui -> p2_text -> clear();
        on_M_follow_clicked();
    }
    else
    {
        ui -> p2_title -> show();
        ui -> p2_title1 -> show();
        ui -> p2_text -> show();
        ui -> p2_good -> show();
        ui -> p2_good1 -> show();
        ui -> p2_comment -> show();
        QString no1 = clickUsername + "：欢迎大家来评论~";
        ui -> comment_text -> setText(no1);
        ui -> comment_text -> append("Koala123：真心不错的文章");

        ui -> p2_title -> setText(p2_title);
        QString sendData2 = "6|" + clickUsername + "|" + p2_title + "|" + dates;
        QString sendData1 = QString::number(sendData2.toUtf8().size());
        int tail = 100 - (sendData1.size());
        for(int i = 0; i < tail; ++i)
            sendData1 += '*';

        tSocket -> write(sendData1.toLatin1());
        tSocket -> write(sendData2.toUtf8());
    }
}

void KoalaNotes::itemClicked3(const QModelIndex &index)
{
    QVariant var = index.data(Qt::UserRole+1);
    MuItemData itemData = var.value<MuItemData>();

    QString p2_title = itemData.title;
    QString dates = itemData.dates;

    clickUsername = itemData.title;
    ui -> p3_notes -> show();
    ui -> M_zhuxiao -> hide();
}

void KoalaNotes::itemClicked4(const QModelIndex &index)
{
    ui -> M_zhuxiao -> hide();
    QVariant var = index.data(Qt::UserRole+1);
    MuItemData itemData = var.value<MuItemData>();

    QString p2_title = itemData.title;
    QString dates = itemData.dates;

    ui -> p2_title -> setText(p2_title);
    QString sendData2 = "8|" + mine.username + "|" + clickUsername + "|" + p2_title + "|" + dates;
    QString sendData1 = QString::number(sendData2.toUtf8().size());
    int tail = 100 - (sendData1.size());
    for(int i = 0; i < tail; ++i)
        sendData1 += '*';

    tSocket -> write(sendData1.toLatin1());
    tSocket -> write(sendData2.toUtf8());
}

void KoalaNotes::itemClicked5(const QModelIndex &index)
{
    QVariant var = index.data(Qt::UserRole+1);
    MuItemData itemData = var.value<MuItemData>();

    QString p3_title = itemData.title;
    QString dates = itemData.dates;

    ui -> p3_return1 -> show();
    ui -> p3_title1 -> show();
    ui -> p3_text1 -> show();
    ui -> p3_share -> hide();
    ui -> M_zhuxiao -> hide();
    ui -> p3_title1 -> setText(p3_title);
    ui -> p3_text1 -> setText(dates);
}

void KoalaNotes::itemClicked6(const QModelIndex &index)
{
    QVariant var = index.data(Qt::UserRole+1);
    MuItemData itemData = var.value<MuItemData>();

    QString p3_title = itemData.title;
    QString dates = itemData.dates;

    ui -> p3_return2 -> show();
    ui -> p3_title2 -> show();
    ui -> p3_text2 -> show();
    ui -> p3_title2 -> setText(p3_title);
    ui -> p3_shared -> hide();
    ui -> p3_text2 -> setText(dates);
}

void KoalaNotes::netLink()
{
    // 网络部分
    tSocket = nullptr;
    tSocket = new QTcpSocket(this);
    // 主动和服务器建立连接
    tSocket -> connectToHost(QHostAddress(QString("xxx.xx.xx.xxx")), 9002);
    connect(tSocket, &QTcpSocket::connected,
           [=]()
               {
                   netlink = true;
               });
    connect(tSocket, &QTcpSocket::disconnected,
           [=]()
               {
                   netlink = false;
                   qDebug() << "网络出错！" << endl;
               });
}

void KoalaNotes::readyRead()
{
    connect(tSocket, &QTcpSocket::readyRead,
                                   [=]()
                                      {
                                          QByteArray tip = tSocket -> readAll();
                                          QString str = tip;
                                          QString nn = str.at(0);
                                          int n = nn.toInt();
                                          switch(n)
                                          {
                                          case 0:
                                              listNotes(str);
                                              break;
                                          case 1:
                                              getitemText(str);
                                              break;
                                          case 2:
                                              presult(str);
                                              break;
                                          case 3:
                                              presult(str);
                                              break;
                                          case 4:
                                              listfollow(str);
                                              break;
                                          case 5:
                                              listfollownotes(str);
                                              break;
                                          case 6:
                                              getfoitemText(str);
                                              break;
                                          case 7:
                                              addfollowresult(str);
                                              break;
                                          case 8:
                                              presult(str);
                                              break;
                                          case 9:
                                              koalashared(str);
                                              break;
                                          default:
                                              break;
                                          }
                                      });
}

void KoalaNotes::listNotes(QString ar)
{
    QString arr;
    for(int i = 2; i < ar.size(); ++i)
        arr += ar.at(i);
    QStringList arlist = arr.split("|");

    Note note;
    listnotes.clear();
    for(int i = 0; i < arlist.size(); i += 2)
    {
        note.title = arlist.at(i);
        note.dates = arlist.at(i + 1);
        listnotes.push_back(note);
    }
}

void KoalaNotes::getitemText(QString str)
{
    QString text;
    for(int i = 2; i < str.size(); ++i)
        text += str.at(i);
    ui -> p1_text -> setText(text);
}

void KoalaNotes::presult(QString str)
{
    QString text;
    for(int i = 2; i < str.size(); ++i)
        text += str.at(i);
    QString yes = "1";
    if(text == yes)
        QMessageBox::information(this, tr("KoalaNotes"), QStringLiteral("Success!"), QMessageBox::Ok);
    else
        QMessageBox::information(this, tr("KoalaNotes"), QStringLiteral("失败，请检查网络后重试！"), QMessageBox::Ok);
}

void KoalaNotes::on_p1_public_clicked()
{
    QDateTime dateTime(QDateTime::currentDateTime());
    QString qStr = dateTime.toString("MM-dd hh:mm:ss");

    QString sendData2 = "2|" + mine.username + "|" + ui -> p1_title ->toPlainText() + "|" + qStr + "|" + "0" + "|" + ui -> p1_text -> toPlainText();
    qDebug() << sendData2 << endl;
    QString datano = QString::number(sendData2.toUtf8().size());

    QString sendData1 = QString::number(sendData2.toUtf8().size());
    int tail = 100 - datano.size();
    for(int i = 0 ; i < tail; ++i)
    {
        sendData1 += '*';
    }

    tSocket -> write(sendData1.toLatin1());
    tSocket -> write(sendData2.toUtf8());
}

void KoalaNotes::on_p1_private_clicked()
{
    QDateTime dateTime(QDateTime::currentDateTime());
    QString qStr = dateTime.toString("MM-dd hh:mm:ss");

    QString sendData2 = "2|" + mine.username + "|" + ui -> p1_title ->toPlainText() + "|" + qStr + "|" + "1" + "|" + ui -> p1_text -> toPlainText();
    qDebug() << sendData2 << endl;
    QString datano = QString::number(sendData2.toUtf8().size());

    QString sendData1 = QString::number(sendData2.toUtf8().size());
    int tail = 100 - datano.size();
    for(int i = 0 ; i < tail; ++i)
    {
        sendData1 += '*';
    }

    tSocket -> write(sendData1.toLatin1());
    tSocket -> write(sendData2.toUtf8());
}

void KoalaNotes::on_p1_modify_clicked()
{
    QDateTime dateTime(QDateTime::currentDateTime());
    QString qStr = dateTime.toString("MM-dd hh:mm:ss");

    QString sendData2;
    if(ui -> p1_p0 -> isChecked())
        sendData2 = "3|" + mine.username + "|" + ui -> p1_title ->toPlainText() + "|" + qStr + "|" + "0" + "|" + ui -> p1_text -> toPlainText() + "|" + raw_title + "|" + raw_date;
    else
        sendData2 = "3|" + mine.username + "|" + ui -> p1_title ->toPlainText() + "|" + qStr + "|" + "1" + "|" + ui -> p1_text -> toPlainText() + "|" + raw_title + "|" + raw_date;
    qDebug() << sendData2 << endl;
    QString datano = QString::number(sendData2.size());

    QString sendData1 = QString::number(sendData2.toUtf8().size());
    int tail = 100 - datano.size();
    for(int i = 0 ; i < tail; ++i)
    {
        sendData1 += '*';
    }

    qDebug() << sendData1 << endl;

    tSocket -> write(sendData1.toLatin1());
    tSocket -> write(sendData2.toUtf8());
}

void KoalaNotes::listfollow(QString ar)
{
    QString arr;
    for(int i = 2; i < ar.size(); ++i)
        arr += ar.at(i);
    QStringList arlist = arr.split("|");

    UserData follow;
    listfo.clear();
    for(int i = 0; i < arlist.size(); i += 2)
    {
        follow.username = arlist.at(i);
        follow.hdno = arlist.at(i + 1);
        listfo.push_back(follow);
    }
    page2Content();
}

void KoalaNotes::listfollownotes(QString ar)
{
    QString arr;
    for(int i = 2; i < ar.size(); ++i)
        arr += ar.at(i);
    QStringList arlist = arr.split("|");

    Note note;
    listfonotes.clear();
    for(int i = 0; i < arlist.size(); i += 2)
    {
        note.title = arlist.at(i);
        note.dates = arlist.at(i + 1);
        listfonotes.push_back(note);
    }
    page2ContentNotes();
}

void KoalaNotes::getfoitemText(QString str)
{
    QString text;
    for(int i = 2; i < str.size(); ++i)
        text += str.at(i);
    ui -> p2_text -> setText(text);
}

void KoalaNotes::on_p2_addfollow_clicked()
{
    if(a_click % 2 == 0)
    {
        ui -> addfollow -> show();
        a_click++;
    }
    else
    {
        ui -> addfollow -> hide();
        a_click++;
    }
}

void KoalaNotes::addfollowresult(QString str)
{
    QString text;
    for(int i = 2; i < str.size(); ++i)
        text += str.at(i);
    QString yes = "1";
    if(text == yes)
    {
        ui -> p2_addtip -> setStyleSheet("color:white;");
        ui -> p2_addtip -> setText("关注成功！");
    }

    else
    {
        ui -> p2_addtip -> setStyleSheet("color:red;");
        ui -> p2_addtip -> setText("关注失败，请检查用户名");
    }
}

void KoalaNotes::on_p2_addsure_clicked()
{
    ui -> p2_addtip -> clear();
    QString addusername = ui -> p2_addusername -> text();
    if(addusername == "")
    {
        ui -> p2_addtip -> setStyleSheet("color:red;");
        ui -> p2_addtip -> setText("用户名不能为空！");
    }
    else if(netlink == false)
    {
        ui -> p2_addtip -> setStyleSheet("color:red;");
        ui -> p2_addtip -> setText("请检查网络是否连接");
    }
    else if(addusername == mine.username)
    {
        ui -> p2_addtip -> setStyleSheet("color:white;");
        ui -> p2_addtip -> setText("您时刻关注着自己呢~");
    }
    else
    {
        QString sendData2 = "7|" + mine.username + "|" + addusername;
        QString sendData1 = QString::number(sendData2.size());
        int tail = 100 - (sendData1.size());
        for(int i = 0; i < tail; ++i)
            sendData1 += '*';

        tSocket -> write(sendData1.toLatin1());
        tSocket -> write(sendData2.toLatin1());
    }
}

void KoalaNotes::on_p3_wantshare_clicked()
{
    if(s_click %2 == 0)
    {
        ui -> shared -> show();
        s_click++;
    }
    else
    {
        ui -> shared -> hide();
        s_click++;
    }
}

void KoalaNotes::koalashared(QString ar)
{
    QString arr;
    for(int i = 2; i < ar.size(); ++i)
        arr += ar.at(i);
    QStringList arlist = arr.split("|");

    Share s;
    share.clear();
    for(int i = 0; i < arlist.size(); i += 5)
    {
        s.myshare = arlist.at(i);
        s.username = arlist.at(i + 1);
        s.hdno = arlist.at(i + 2);
        s.title= arlist.at(i + 3);
        s.text = arlist.at(i + 4);
        share.push_back(s);
    }
    page3Content();
}

void KoalaNotes::on_p3_return1_clicked()
{
    ui -> p3_return1 -> hide();
    ui -> p3_title1 -> hide();
    ui -> p3_text1 -> hide();
    ui -> p3_share -> show();
}

void KoalaNotes::on_p3_return2_clicked()
{
    ui -> p3_return2 -> hide();
    ui -> p3_title2 -> hide();
    ui -> p3_text2 -> hide();
    ui -> p3_shared -> show();
}

void KoalaNotes::on_M_zhuxiao_clicked()
{
    if(netlink == true)
    {
        QString sendData2 = "d|" + mine.username;
        QString sendData1 = QString::number(sendData2.size());
        int tail = 100 - (sendData1.size());
        for(int i = 0; i < tail; ++i)
            sendData1 += '*';

        tSocket -> write(sendData1.toLatin1());
        tSocket -> write(sendData2.toLatin1());
        QMessageBox::information(this, tr("注销成功"), QStringLiteral("您已注销此账号！"), QMessageBox::Ok);
        ui -> M_zhuxiao -> hide();
        on_W_cancel_clicked();
    }
    else
        QMessageBox::information(this, tr("KoalaNotes"), QStringLiteral("未登陆或连接网络失败!"), QMessageBox::Ok);
}

void KoalaNotes::on_M_account1_clicked()
{
    if(netlink == true)
        ui -> M_zhuxiao -> show();
}

void KoalaNotes::on_M_account_clicked()
{
    if(netlink == true)
        on_M_account1_clicked();
}

void KoalaNotes::on_p2_good_clicked()
{
    ui -> p2_good -> setFlat(true);
    QIcon icon;
    icon.addFile(tr("../Icons/p2_good1.png"));
    ui -> p2_good -> setIconSize(QSize(40, 40));
    ui -> p2_good -> setIcon(icon);
    QString god = ui -> p2_good1 -> text();
    unsigned int good = god.toUInt();
    good++;
    ui -> p2_good1 -> setText(QString::number(good));
}

void KoalaNotes::on_p2_comment_clicked()
{
    if(c_click %2 == 0)
    {
        ui -> p2_widget -> show();
        c_click++;
    }
    else
    {
        ui -> p2_widget -> hide();
        c_click++;
    }
}

void KoalaNotes::on_comment_sure_clicked()
{
    QString edit = ui -> comment_edit -> toPlainText();
    QString comment = mine.username + "：" + edit;
    ui -> comment_text -> append(comment);
    ui -> comment_edit -> clear();
}
