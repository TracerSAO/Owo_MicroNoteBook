#include "signupwidget.h"

SignUpWidget::SignUpWidget(QWidget *parent) : QWidget(parent)
{
    init();
}

SignUpWidget::~SignUpWidget()
{
}

void
SignUpWidget::init()
{
    setGeometry(670, 250, 450, 550);

    // new Label & LineEdit
    _sign_nameLabel    = new QLabel(this);
    _sign_nameLine     = new QLineEdit(this);
    _sign_passwdLabel  = new QLabel(this);
    _sign_passwdLine   = new QLineEdit(this);
    _sign_2passwdLabel = new QLabel(this);
    _sign_2passwdLine  = new QLineEdit(this);
    _sign_tipLabel     = new QLabel(this);
    _sign_nameLabel->setText(tr("Name"));
    _sign_passwdLabel->setText(tr("Passwd"));
    _sign_2passwdLabel->setText(tr("surePasswd"));
    _sign_tipLabel->setText(tr("passwd could only to use digit (:"));
    QFont font, font1;
    font.setPixelSize(20);
    font1.setPixelSize(15);
    _sign_tipLabel->setFont(font1);
    _sign_nameLine->setFont(font);
    _sign_passwdLine->setFont(font);
    _sign_2passwdLine->setFont(font);
    _sign_2passwdLine->setEchoMode(QLineEdit::Password);
    _sign_passwdLine->setEchoMode(QLineEdit::Password);
    // set position
    _sign_nameLabel->setGeometry(100, 30, 75, 50);
    _sign_nameLine->setGeometry(190, 30, 220, 50);
    _sign_passwdLabel->setGeometry(100, 95, 75, 50);
    _sign_passwdLine->setGeometry(190, 95, 220, 50);
    _sign_2passwdLabel->setGeometry(95, 150, 80, 50);
    _sign_2passwdLine->setGeometry(190, 150, 220, 50);
    _sign_tipLabel->setGeometry(125, 230, 250, 50);

    // new Button
    _sign_OK = new QPushButton(this);
    _sign_Cancel = new QPushButton(this);
    _sign_OK->setText("OK");
    _sign_Cancel->setText("Cancel");
    // set position
    _sign_OK->setGeometry(132, 400, 100, 65);
    _sign_Cancel->setGeometry(282, 400, 100, 65);

    show();

    connect(_sign_OK, SIGNAL(clicked()), this, SLOT(SLOT_SIGN_OK_Button()));
    connect(_sign_Cancel, SIGNAL(clicked()), this, SLOT(SLOT_SIGN_Cancel_Button()));

}

void
SignUpWidget::close_()
{
    _sign_nameLine->clear();
    _sign_passwdLine->clear();
    _sign_2passwdLine->clear();
    close();
}

void
SignUpWidget::SLOT_SIGN_OK_Button()
{
    // 1. 判断两次的密码是否相同
    // 2. 将 name 和 passwd 添加到 access DB 中去
    // 2.1. passwd 只能是数字类型字符
    // 3. 关闭 signWidget，打开 this(Dialog - 登录界面)
    QString passwd_1th = _sign_passwdLine->text();
    QString passwd_2th = _sign_2passwdLine->text();

    if (passwd_1th == passwd_2th)
    {

        QString userName = _sign_nameLine->text();
        QString addUser = QString("INSERT INTO table1(userName, userPasswd) VALUES ('%1', '%2')").arg(userName).arg(passwd_1th);
        QSqlQuery accessQuery;

        accessQuery.prepare(addUser);
        if (accessQuery.exec()) {
            QMessageBox::information(this, "Success!",
                                     "OK, User sign up success",
                                     QMessageBox::Ok);
            close_();
        } else {
            QMessageBox::warning(this, "Error!",
                                 "Sorry, add user failure",
                                 QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::warning(this, "Error!",
                             "Sorry, twice password does not similarity );",
                             QMessageBox::Ok);
    }
}

void
SignUpWidget::SLOT_SIGN_Cancel_Button()
{
    close_();
}
