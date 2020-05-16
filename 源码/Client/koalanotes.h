#ifndef KOALANOTES_H
#define KOALANOTES_H

#include "head.h"
#include "signin.h"
#include "register.h"
#include "muitemdelegate.h"
#include "mulistitemdata.h"

struct Note
{
    QString title;
    QString dates;
};

struct UserData
{
    QString username;
    QString hdno;
};

struct Share
{
    QString username;
    QString hdno;
    QString title;
    QString text;
    QString myshare;
};

QT_BEGIN_NAMESPACE
namespace Ui { class KoalaNotes; }
QT_END_NAMESPACE

class KoalaNotes : public QMainWindow
{
    Q_OBJECT

public:
    KoalaNotes(QWidget *parent = nullptr);
    ~KoalaNotes();
    void setWindows();
    void setToolTips();
    void setHeadPicture();
    void setButtonStyle();
    void initVar();
    void mousePressEvent(QMouseEvent* mEvent);
    void mouseMoveEvent(QMouseEvent* mEvent);
    void bindPages();
    void page0Content();
    void page1Content();
    void page2Content();
    void page2ContentNotes();
    void page3Content();
    void page4Content();
    void receive_a_signal();
    void netLink();
    void readyRead();
    void listNotes(QString ar);
    void getitemText(QString ar);
    void presult(QString ar);
    void listfollow(QString ar);
    void listfollownotes(QString ar);
    void getfoitemText(QString ar);
    void addfollowresult(QString ar);
    void koalashared(QString ar);

private slots:
    void on_M_close_clicked();

    void on_M_min_clicked();

    void on_M_top_clicked();

    void on_M_follow_clicked();

    void on_M_new_clicked();

    void on_M_mynotes_clicked();

    void on_M_prishare_clicked();

    void on_M_more_clicked();

    void on_M_headb_clicked();

    void on_M_header_clicked();

    void on_W_login_clicked();

    void on_W_logup_clicked();

    void on_W_cancel_clicked();

    void on_p1_private_clicked();

    void on_p1_public_clicked();

    void on_p1_modify_clicked();

    void on_p2_addfollow_clicked();

    void on_p2_addsure_clicked();

    void on_M_chbg_clicked();

    void on_p3_wantshare_clicked();

    void on_p3_return1_clicked();

    void on_p3_return2_clicked();

    void on_M_zhuxiao_clicked();

    void on_M_account1_clicked();

    void on_M_account_clicked();

public slots:
    // 定义接收登陆窗口发来信号的槽函数
    void receive_from_signin(QString recv_msg);
    // 列表点击事件
    void itemClicked0(const QModelIndex &index);
    void itemClicked1(const QModelIndex &index);
    void itemClicked2(const QModelIndex &index);
    void itemClicked3(const QModelIndex &index); // shared
    void itemClicked4(const QModelIndex &index); // shared
    void itemClicked5(const QModelIndex &index); // sharedlist
    void itemClicked6(const QModelIndex &index); // sharedlist

private:
    Ui::KoalaNotes *ui;
    QPoint dragPosition;
    QString rollBar;
    bool ontop;
    bool login;
    bool newnotes;
    Signin *signin;
    Register *signup;
    int h_click;
    int a_click;
    int bg_click;
    int s_click;
    QStandardItem *pItem;
    QStandardItemModel *pModel;
    QStandardItem *pItem1;
    QStandardItemModel *pModel1;
    QStandardItem *pItem2;
    QStandardItemModel *pModel2;
    QStandardItem *pItem3;
    QStandardItemModel *pModel3;
    UserData mine;
    QTcpSocket *tSocket;
    bool netlink;
    QVector<Note> listnotes;
    QVector<UserData> listfo;
    QVector<Note> listfonotes;
    QString raw_title, raw_date;
    QString clickUsername;
    QVector<Share> share;
};
#endif // KOALANOTES_H
