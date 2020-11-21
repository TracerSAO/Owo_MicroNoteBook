#ifndef WRITEWIDGET_H
#define WRITEWIDGET_H

#include <QMainWindow>
#include <QStatusBar>
#include <QPlainTextEdit>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QProgressDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QtSql>
#include <QMessageBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>

#include <qthread.h>            // ??? 我当时为啥要包含这个头文件 ???

class WriteWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit WriteWidget(const QString &_UID, const int &_paperCount, QWidget *parent = nullptr);
    ~WriteWidget();
signals:

private:
    void init();
    void loadImage();
    void loadFont();
    void setMainWindows();
    void setPlainTextEdit();
    void setMenubar();
    void setToolbar();
    void setAction();
    void setConnect();
    void PUBLICARTICLES();
    void THEME(const QString &back, const QString &font);

private slots:
//    void slot_act_File_New();
//    void slot_act_File_Open();
    void slot_act_File_PublicArticles();    // 发布文章
//    void slot_act_File_SaveAs();

    void slot_act_Edit_Copy();
    void slot_act_Edit_Cut();
    void slot_act_Edit_Paste();
    void slot_act_Edit_Clear();
    void slot_act_Edit_Iconfont();
    void slot_act_Edit_Colorfont();

    void slot_act_View_Theme_Dark();
    void slot_act_View_Theme_Light();
    void slot_act_View_Theme_Pink();
    void slot_act_View_Theme_Custom();

public:
    QString _paperTitle;
    QString _toDay;
    QString _paperTag;

private:
    int     userPaperCount = 0;
    QString UID;
    QString fileName = "E:\\Coding\\helloQT\\Test_Everything_2th\\LibraryManageSystem\\Test_LAST\\docs\\";
    QString *copyString = nullptr;


//    QIcon icon_ManWin;
//    QIcon icon_file_New;
//    QIcon icon_file_Open;
    QIcon icon_file_Save;
//    QIcon icon_file_SaveAs;
    QIcon icon_Edit_Copy;
    QIcon icon_Edit_Cut;
    QIcon icon_Edit_Paste;
    QIcon icon_Edit_Clear;
    QIcon icon_Edit_fontIcon;
    QIcon icon_Edit_fontColor;
    QIcon icon_vie_Theme_Dark;
    QIcon icon_vie_Theme_Custom;

    QFont font;
    QFont font_PlainTextEdit;

    QPlainTextEdit  *display;
    QLineEdit       *titlePart;
    QLineEdit       *tagPart;
    QWidget         *partWidget;
    QWidget         *mainWidget;

    QHBoxLayout     *partLayout;
    QVBoxLayout     *mainLayout;


    QMenuBar *menuBar;
    QMenu    *menu_File;
    QMenu    *menu_Edit;
    QMenu    *menu_View;
    QMenu    *menu_View_Theme;
    QToolBar *toolBar;

//    QAction  *act_fil_New;
//    QAction  *act_fil_Open;

//    QAction  *act_fil_SaveAs;
    QAction  *act_edi_Copy;
    QAction  *act_edi_Cut;
    QAction  *act_edi_Paste;
    QAction  *act_edi_Clear;
    QAction  *act_edi_fontcolor;
    QAction  *act_edi_fontIcon;
    QAction  *act_vie_Theme_Dark;
    QAction  *act_vie_Theme_Light;  // icon void
    QAction  *act_vie_Theme_Pink;   // icon void
    QAction  *act_vie_Theme_Custom;

public:
    QAction  *act_fil_PublicArticles;
//    friend class PaperWidget;       // 将 PaperWidget 添加为 友元，使其可以调用 slot_act_File_PublicArticles() 和 act_fil_PublicArticles
};

#endif // WRITEWIDGET_H
