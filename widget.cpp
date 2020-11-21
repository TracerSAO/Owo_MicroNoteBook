#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    init();
    connectSQL();
}

Widget::~Widget()
{

    if (paperWidget != nullptr)
        delete paperWidget;
    if (signUpWidget != nullptr)
        delete signUpWidget;
}

void
Widget::init()
{
    resize(800, 600);

    movieLabel = new QLabel(this);
    movieLabel->setGeometry(0, 0, 800, 350);
    gifPicture = new QMovie(":/Image/word.gif");
    movieLabel->setMovie(gifPicture);
    movieLabel->setAlignment(Qt::AlignCenter);
    movieLabel->setScaledContents(true);
    gifPicture->start();

    nameLabel = new QLabel(this);
    nameLabel->setText(QStringLiteral("账号"));
    nameLabel->setGeometry(165, 360, 70, 50);
    nameLineEdit = new QLineEdit(this);
    nameLineEdit->setGeometry(240, 360, 320, 50);

    passwdLabel = new QLabel(this);
    passwdLabel->setText(QStringLiteral("密码"));
    passwdLabel->setGeometry(165, 425, 70, 50);
    passwdLineEdit = new QLineEdit(this);
    passwdLineEdit->setEchoMode(QLineEdit::Password);
    passwdLineEdit->setGeometry(240, 425, 320, 50);

    loginPushButton = new QPushButton(this);
    loginPushButton->setText(tr("Login"));
    loginPushButton->setGeometry(200, 500, 180, 75);
    signPushButton = new QPushButton(this);
    signPushButton->setText(tr("Sign Up"));
    signPushButton->setGeometry(420, 500, 180, 75);

    QFont font;
    font.setFamily(QStringLiteral("Cascadia Mono"));
    font.setPointSize(12);
    nameLabel->setFont(font);
    passwdLabel->setFont(font);

    QFont font1;
    font1.setFamily(QStringLiteral("Cascadia Code"));
    font1.setPointSize(11);
    nameLineEdit->setFont(font1);
    passwdLineEdit->setFont(font1);


    QFont font2;
    font2.setFamily(QStringLiteral("Cascadia Code"));
    font2.setPointSize(12);
    font2.setBold(true);
    font2.setWeight(75);
    loginPushButton->setFont(font2);
    signPushButton->setFont(font2);

    connect(loginPushButton, SIGNAL(clicked()), this, SLOT(SLOT_LOGIN_CLICKED()));
    connect(signPushButton, SIGNAL(clicked()), this, SLOT(SLOT_SIGN_CLICKED()));
}

void
Widget::connectSQL()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    db = QSqlDatabase::addDatabase(QString("QODBC"));
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=C:\\Users\\13422\\Desktop\\runoob.mdb");
    if (!db.open()) {
        QMessageBox::critical (0, QStringLiteral("打开数据库错误"),
                               QStringLiteral("不能建立数据库连接\n"),
                               QMessageBox::Cancel);
    }
}

bool
Widget::loginVerify(QString &name, QString &passwd)
{
    QSqlQuery loginQuery;
    QString userInfo = QString("select * from table1 where userName='%1' and userPasswd=%2").arg(name).arg(passwd);

    if (loginQuery.exec(userInfo) && loginQuery.next())     // next() 是对从数据库中返回的数据集(set) 进行判断，是否为空
    {
        UID = loginQuery.value("UID").toString();
        return true;
    }
    else
        return false;
}

void
Widget::createPaperWidget()
{
    paperWidget = new PaperWidget(UID);
}

void
Widget::createSignUpWidget()
{
    signUpWidget = new SignUpWidget;
}

void
Widget::SLOT_SIGN_CLICKED()
{
    if (signUpWidget != nullptr)
        signUpWidget->show();
    else
        createSignUpWidget();
}

void
Widget::SLOT_LOGIN_CLICKED()
{
    QString name = nameLineEdit->text();
    QString passwd = passwdLineEdit->text();

    if (loginVerify(name, passwd))  // 登录成功
    {
        close();
        createPaperWidget();

        if (signUpWidget != nullptr)    // 释放资源
        {
            delete signUpWidget;
            signUpWidget = nullptr;
        }
    }
    else    // 登录失败
    {
        // 弹出对话框 -> 账户或密码错误
        QMessageBox::warning(this, "Failed",
                             "Sorry, user name or passwd not right\nPleas try again );",
                             QMessageBox::Ok);

        // 清空 ui->nameLineEdit 和 ui->passwdLineEdit 中的存下的数据
        nameLineEdit->clear();
        passwdLineEdit->clear();
    }
}
