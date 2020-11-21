#include "paperwidget.h"

PaperWidget::PaperWidget(QWidget *parent) : QWidget(parent)
{
    // 预留的接口，MUJI知道会用来干嘛啊
}

PaperWidget::PaperWidget(const QString& _UID, QWidget *parent):
    QWidget(parent), UID(_UID)
{
    init();
    show();
}

PaperWidget::~PaperWidget()
{
    // 果然和我想的一样，当 Qt 中的组件使用 setLayout or addWidget 这类
    // 的函数后，被操纵的组件的生存周期便，完完全全的交由上级控制

    if (readWidget != nullptr)
    {
        readWidget->close();
        delete readWidget;
    }

    if (writeWidget != nullptr)
    {
        writeWidget->close();
        delete writeWidget;
    }
}

void
PaperWidget::init()
{
    resize(815, 600);
    // 设置编码格式，因为 Qt 使用外部环境中编译器，像 MSVC 和 MinGW 这类的
    // 前者，的编码方式不属于 UTF-8，而后者支持 UTF-8
    // 为了避免编码的麻烦，统一使用 MinGW 编译器进行编译
    // 下面这行代码在 MinGW 中使不使用都无所谓
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    // 禁止改变窗体大小 - 等待添加函数

    // NO.1 设置 Tags 区域的 UI
    paperKindsList = new QListWidget;
    paperKindsList->setParent(this);
    paperKindsList->setGeometry(10, 280, 240, 250);

    QVector<QString> paperColumns;
    QSqlQuery query;
    QString strCMD = QString("select tagName from tags");
    if (query.exec(strCMD))
    {
        while (query.next())
        {
            paperColumns.append(query.value("tagName").toString());
        }
    } else qDebug() << "PaperWidget -> " << strCMD << " run FAILED...";

    for (int i = 0; i < paperColumns.count(); i++)
    {
        paperKindsList->addItem(paperColumns[i]);
    }
    connect(paperKindsList, SIGNAL(currentRowChanged(int)), this, SLOT(SLOT_paperTagsList_CLICKED(int)));


    // NO.2 设置 paperWidget 区域的 UI
    paperStackedWidget = new QStackedWidget;
    for (int i = 0; i < paperKindsList->count(); i++)
    {
        QString tag = paperKindsList->item(i)->text();
        qDebug() << "PaperWidget -> tag: " << tag;
        init_paperShowList(tag);
    }
    paperStackedWidget->setCurrentIndex(0);


    // mainLayout->addWidget(paperListScrollWidget);
    // 掌握新技能点： QWidget 实例如果想添加附属组件，不需要通过（也不能） addWidget() 函数来实现附属组件的添加
    // 而是通过，对附属组件设置 setParent(this) 方式，即可将附属组件添加到 “上层组件” 中，
    // 待附属组件添加完成之后，再对其使用 setGeometry(x, y, width, hight) ,
    // 那么，这里所操作的坐标系，便是相对坐标

    // NO.3 设置 info 区域的 UI

    init_Info();

    userGroupBox->setParent(this);
    userGroupBox->setGeometry(10, 10, 240, 266);


    // NO.4 设置进入文章写作空间的按钮
    writePtn = new QPushButton(this);
    writePtn->setText(QStringLiteral("进入创作空间"));
    writePtn->setGeometry(10, 540, 130, 50);
    connect(writePtn, SIGNAL(clicked()), this, SLOT(SLOT_writePtn_CLICKED()));
}


/*
 *  argument: const QString &paperTag 用于 init_paperList 方法的封装
 *  使用这种封装方式之后，可以将初始化 文章列表 的功能可用于 QStackWidget 栈组件的页面跳转更新
 *  文章展示内容：
 *      1. 文章名
 *      2. 作者
 *      3. 内容摘要
 *      4. 文章发布时间
 *  点击对应文章后，跳转进入文章阅读界面
 */
void
PaperWidget::init_paperShowList(const QString &paperTag)
{
    paperListViewWidget = new QWidget;
    paperListScrollWidget = new QScrollArea;
    paperListWidgetLayout = new QVBoxLayout;

    QString strCMD = QString("select * "
                             "from table1, paperLists "
                             "where table1.UID=paperLists.UID and table1.UID=%1 and paperTag='%2'").arg(UID).arg(paperTag);
    QSqlQuery infoQuery;
    if(infoQuery.exec(strCMD))
        qDebug() << "PaperWidget -> exec(" << strCMD << ") TRUE";
    else qDebug() << "PaperWidget -> exec(" << strCMD << ") FALSE";

    qDebug() << "->next()";
    while (infoQuery.next())
    {
        QPushButton *ptn = new QPushButton;
        QString paperTitle = infoQuery.value("paperTitle").toString();
        QString userName = infoQuery.value("userName").toString();
        QString startTime = infoQuery.value("startTime").toString();
        QString paperIDTOptn = infoQuery.value("ID").toString();

        ptn->setObjectName(paperIDTOptn);                   // 为每个文章按钮添加 objectName，用户槽函数对信号发射者的反向搜索
        qDebug() << "PaperWidget -> ObjectName() = " << ptn->objectName();
        ptn->setText(QString("Title: %1\nAuthor: %2\nCreateTime: %3").arg(paperTitle).arg(userName).arg(startTime));
        qDebug() << "PaperWidget -> QPushButton::Text() = " << ptn->text();
        ptn->setMinimumSize(QSize(500, 80));

        connect(ptn, SIGNAL(clicked()), this, SLOT(SLOT_paperPtnList_CLICKED()));

        paperListWidgetLayout->addWidget(ptn);
    }

    paperListViewWidget->setLayout(paperListWidgetLayout);  // 实体的存储 众多文章按钮 的 “大型” 组件
    paperListScrollWidget->setWidget(paperListViewWidget);  // 用于展示 “大型” 组件的窗口，通过滑动条来移动观察被观察对象的其他部分
    paperStackedWidget->addWidget(paperListScrollWidget);
    paperStackedWidget->setParent(this);
    paperStackedWidget->setGeometry(260, 10, 550, 580);
}

/* 未完成 后续填补 */
void
PaperWidget::init_Info()
{
    userGroupBox = new QGroupBox("USER INFO");
    userPixmap = new QLabel;
    userPixmap->setParent(userGroupBox);
    userName = new QLabel;
    userName->setParent(userGroupBox);
    paperCount = new QLabel;
    paperCount->setParent(userGroupBox);
    paperKindsCount = new QLabel;
    paperKindsCount->setParent(userGroupBox);

    userPixmap->setGeometry(5, 15, 235, 140);
    userName->setGeometry(5, 150, 120, 30);
    paperCount->setGeometry(3, 190, 120, 30);
    paperKindsCount->setGeometry(3, 230, 120, 30);


    // userPixmap 设置用户画像
    QPixmap tmpPixiv(":/Image/default.jpg");
    userPixmap->setPixmap(tmpPixiv.scaled(QSize(220, 140), Qt::IgnoreAspectRatio));

    // userName 设置用户的用户名
    QString name;
    QSqlQuery infoQuery;
    QString userInfo = QString("select userName from table1 where UID=%1").arg(UID);
    if (infoQuery.exec(userInfo) && infoQuery.next())
    {
        name = infoQuery.value("userName").toString();
    } else {
        name = "~WoW~";
    }
    userName->setText("Name: " + name);

    // paperCount 设置用户发布的文章数量
    userInfo = QString("select paperCount from table1 where UID=%1").arg(UID);
    if (infoQuery.exec(userInfo))
    {
        infoQuery.next();
        paperCount->setText("paperCount: " + infoQuery.value("paperCount").toString());
    } else {
        paperCount->setText("?=w=?");
    }

    // paperKindsCount 设置用户创建的文章标签数
    userInfo = QString("select * from tags");
    if (infoQuery.exec(userInfo))
    {
        int count = 0;
        while (infoQuery.next())
            count++;

        QString tmp("Tags count: ");
        tmp = tmp + QString("%1").arg(count);
        paperKindsCount->setText(tmp);
    } else paperKindsCount->setText(QString("QoQ"));
}

/*
 * Function:
 *      1. get the path of the paper of user chooise
 *      2. create the new Widget of read paper
*/
void
PaperWidget::SLOT_paperPtnList_CLICKED()
{
    // 反向搜索信号的发射者
    QPushButton *ptn = qobject_cast<QPushButton*>(sender());
    QString ptnName = ptn->objectName();
    QString tmpPath = Path;
    QString tmpTitle;

    QString strCMD = QString("select * "
                             "from paperLists "
                             "where ID=%1").arg(ptnName);
    QSqlQuery query;
    if (query.exec(strCMD))
    {
        while (query.next())    // aaaa，搞了半天的错误，尽然只是没有使用 next()，将内部指针指向一个具有实际意义的数据条目
        {
            QString tmp = query.value("paperPath").toString();
            tmpPath.append(tmp);
            tmpTitle.append(query.value("paperTitle").toString());
            qDebug() << "PaperWidget -> paper Path: " << tmpPath;
        }
    } else {
        tmpPath.append("default.txt");
        qDebug() << "PaperWidget -> It's failed for get paper Path (;";
    }

    if (readWidget != nullptr)  // 内存防护措施
        delete readWidget;
    readWidget = new ReadWidget(tmpPath, tmpTitle);
}

void
PaperWidget::SLOT_paperTagsList_CLICKED(int currentRow)
{
    paperStackedWidget->setCurrentIndex(currentRow);
}

void
PaperWidget::SLOT_writePtn_CLICKED()
{
    QSqlQuery query;
    QString strCMD = QString("select paperCount from table1 where UID=%1").arg(UID);
    if (query.exec(strCMD))
    {
        query.next();
        userPaperCount = query.value("paperCount").toInt();
        qDebug() << "PaperWidget -> paperCount = " << userPaperCount;
    } else qDebug() << "PaperWidget -> QSqlQuery::exec(" << strCMD << ") Run FAILED...";

    if (writeWidget != nullptr) // 内存防护措施
        writeWidget->show();
    else
    {
        writeWidget = new WriteWidget(UID, userPaperCount);

        connect(writeWidget->act_fil_PublicArticles, SIGNAL(triggered()), this, SLOT(SLOT_paperUpdate()));
    }
}

void
PaperWidget::SLOT_paperUpdate()
{
//    QThread::msleep(2);      // 1000 ms = 1s 等待 1s，先让数据库进行更新
    QTime t;
    t.start();
    while(t.elapsed() < 1000)
        QCoreApplication::processEvents();

    qDebug() << "PaperWidget -> " << writeWidget->_paperTag << " " << writeWidget->_toDay << " " << writeWidget->_paperTitle;
    QString _paperPath = QString("P%1_%2.txt").arg(UID).arg(userPaperCount + 1);
    QSqlQuery query;
    QString strCMD = QString("update table1 set paperCount = paperCount + 1 where UID=%1").arg(UID);

    // 将新发布的文章信息插入到 paperLists 表中
    if (!query.exec(strCMD))
        qDebug() << "PaperWidget -> " << strCMD << " run FAILED...";
    else
    {
        strCMD = QString("insert "
                         "into paperLists(UID, paperTitle, startTime, paperTag, paperPath) "
                         "values (%1, '%2', '%3', '%4', '%5')").
                arg(UID).arg(writeWidget->_paperTitle).arg(writeWidget->_toDay).arg(writeWidget->_paperTag).arg(_paperPath);
        if (!query.exec(strCMD))
            qDebug() << "PaperWidget -> " << strCMD << " run FAILED...";
    }

    userPaperCount++;

    // 判断 papertag 是否已存在于 tags 表中
    strCMD = QString("select * from tags where tagName = '%1'").arg(writeWidget->_paperTag);
    if (query.exec(strCMD))
    {
        if (!query.next())      // tag 未存在于 tags 表中
        {
            strCMD = QString("insert into tags(tagName) values('%1')").arg(writeWidget->_paperTag);
            qDebug() << "PaperWidget -> " << strCMD;
            if (query.exec(strCMD))
            {
                // 为 paperKindsList 组件，添加新的成员 papertag
                paperKindsList->addItem(writeWidget->_paperTag);
                // 为 paperStackedWidget 组件，添加新的页面
                init_paperShowList(writeWidget->_paperTag);               // HOHOHOHHOOO~~ 太幸运了，之前自己封装了一个 构造 stackwidget 组件的函数，一步到位，赞！

            } else qDebug() << "PaperWidget -> query.exec(" << strCMD << ") run FAILED...";
        }
        else    // tag 已经存在于 tags 表中
        {
            int order = 0;      // tag 在 paperKindsList 中的序列
            for (int i = 0; i < paperKindsList->count(); i++)
            {
                QString str = paperKindsList->item(i)->text();
                if (str == writeWidget->_paperTag)
                {
                    order = i;
                }
            }
            paperStackedWidget->removeWidget(paperStackedWidget->widget(order));

            paperListViewWidget = new QWidget;
            paperListScrollWidget = new QScrollArea;
            paperListWidgetLayout = new QVBoxLayout;

            QString strCMD = QString("select * "
                                     "from table1, paperLists "
                                     "where table1.UID=paperLists.UID and table1.UID=%1 and paperTag='%2'").arg(UID).arg(writeWidget->_paperTag);
            QSqlQuery infoQuery;
            if(infoQuery.exec(strCMD))
                qDebug() << "PaperWidget -> exec(" << strCMD << ") TRUE";
            else qDebug() << "PaperWidget -> exec(" << strCMD << ") FALSE";

            qDebug() << "->next()";
            while (infoQuery.next())
            {
                QPushButton *ptn = new QPushButton;
                QString paperTitle = infoQuery.value("paperTitle").toString();
                QString userName = infoQuery.value("userName").toString();
                QString startTime = infoQuery.value("startTime").toString();
                QString paperIDTOptn = infoQuery.value("ID").toString();

                ptn->setObjectName(paperIDTOptn);                   // 为每个文章按钮添加 objectName，用户槽函数对信号发射者的反向搜索
                qDebug() << "PaperWidget -> ObjectName() = " << ptn->objectName();
                ptn->setText(QString("Title: %1\nAuthor: %2\nCreateTime: %3").arg(paperTitle).arg(userName).arg(startTime));
                qDebug() << "PaperWidget -> QPushButton::Text() = " << ptn->text();
                ptn->setMinimumSize(QSize(500, 80));

                connect(ptn, SIGNAL(clicked()), this, SLOT(SLOT_paperPtnList_CLICKED()));

                paperListWidgetLayout->addWidget(ptn);
            }

            paperListViewWidget->setLayout(paperListWidgetLayout);  // 实体的存储 众多文章按钮 的 “大型” 组件
            paperListScrollWidget->setWidget(paperListViewWidget);  // 用于展示 “大型” 组件的窗口，通过滑动条来移动观察被观察对象的其他部分
//            paperStackedWidget->addWidget(paperListScrollWidget);
            paperStackedWidget->insertWidget(order, paperListScrollWidget);
            paperStackedWidget->setCurrentIndex(order);
        }
    }
    else qDebug() << "PaperWidget -> " << strCMD << " run FAILED...";
}
