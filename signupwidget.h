#ifndef SIGNUPWIDGET_H
#define SIGNUPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFont>
#include <QtSql>
#include <QMessageBox>

class SignUpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignUpWidget(QWidget *parent = nullptr);
    ~SignUpWidget();

signals:

private slots:
    void SLOT_SIGN_OK_Button();
    void SLOT_SIGN_Cancel_Button();

private:
    void init();                // 初始化 signWidget 组件
    void close_();              // 整合基本的 close 逻辑
    void isDigitalString();     // 判断密码是否为全数字字符串

private:
//    QSqlQuery      *sign_sqlQuery = nullptr;
    QLabel         *_sign_tipLabel = nullptr;
    QLabel         *_sign_nameLabel = nullptr;
    QLineEdit      *_sign_nameLine = nullptr;
    QLabel         *_sign_passwdLabel = nullptr;
    QLineEdit      *_sign_passwdLine = nullptr;
    QLabel         *_sign_2passwdLabel = nullptr;
    QLineEdit      *_sign_2passwdLine = nullptr;
    QPushButton    *_sign_OK = nullptr;
    QPushButton    *_sign_Cancel = nullptr;
};

#endif // SIGNUPWIDGET_H
