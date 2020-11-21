#ifndef WIDGET_H
#define WIDGET_H

#include <QDialog>
#include <QMovie>
#include <QString>
#include <QWidget>
#include <QtSql>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QFont>
#include <QMessageBox>

#include "signupwidget.h"
#include "paperwidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void init();
    void connectSQL();
    bool loginVerify(QString &name, QString &passwd);
    void createPaperWidget();
    void createSignUpWidget();

private slots:
    void SLOT_SIGN_CLICKED();
    void SLOT_LOGIN_CLICKED();

private:
    // 数据库
    QSqlQueryModel  model;
    QSqlDatabase    db;
    QString         UID;

    // 登录界面
    QMovie          *gifPicture;
    QLabel          *movieLabel;
    QLabel          *nameLabel;
    QLabel          *passwdLabel;
    QLineEdit       *nameLineEdit;
    QLineEdit       *passwdLineEdit;
    QPushButton     *loginPushButton;
    QPushButton     *signPushButton;

    // 用户注册界面
    SignUpWidget    *signUpWidget = nullptr;
    // 文章展览中心
    PaperWidget     *paperWidget = nullptr;
    // 文章阅读界面
};
#endif // WIDGET_H
