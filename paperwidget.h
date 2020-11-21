#ifndef PAPERWIDGET_H
#define PAPERWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QListWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QPixmap>
#include <QSize>
#include <QtSql>
#include <QString>
#include <QStackedWidget>
#include <QTextCodec>
#include <QThread>

#include "readwidget.h"
#include "writewidget.h"

class PaperWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaperWidget(QWidget *parent = nullptr);
    explicit PaperWidget(const QString &UID, QWidget *parent = nullptr);
    ~PaperWidget();

signals:

private slots:
    void SLOT_paperTagsList_CLICKED(int currentRow);
    void SLOT_writePtn_CLICKED();
    void SLOT_paperPtnList_CLICKED();
    void SLOT_paperUpdate();

private:
    void init();
    void init_paperShowList(const QString &Papertag);
    void init_Info();

private:
    // data
    int     userPaperCount = 0;
    QString UID;
    QString Path = "E:\\Coding\\helloQT\\Test_Everything_2th\\LibraryManageSystem\\Test_LAST\\docs\\";

    // 文章列表区    - paper part
    QScrollArea         *paperListScrollWidget;
    QWidget             *paperListViewWidget;
    QVBoxLayout         *paperListWidgetLayout;
    QStackedWidget      *paperStackedWidget;

    // 用户信息      - info part
    QGroupBox           *userGroupBox;
    QLabel              *userPixmap;
    QLabel              *userName;
    QLabel              *paperCount;
    QLabel              *paperKindsCount;
    // 文章分类区    - kinds part
    QListWidget         *paperKindsList;
    // info & kindsList 的上下布局器
    QVBoxLayout         *leftMainLayout;    // left part main layout

    // 跳转按钮
    QPushButton         *writePtn;      // 进入文章写作空间

    // 用户阅读界面
    ReadWidget          *readWidget = nullptr;

    // 用户创作界面
    WriteWidget         *writeWidget = nullptr;
};

#endif // PAPERWIDGET_H
