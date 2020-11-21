#include "writewidget.h"

WriteWidget::WriteWidget(const QString &_UID, const int &_paperCount, QWidget *parent) :
    QMainWindow(parent), userPaperCount(_paperCount), UID(_UID)
{
    init();
}

WriteWidget::~WriteWidget()
{
    if (!this->copyString)
        delete this->copyString;
}

void
WriteWidget::init()
{
    loadImage();        // load images in order to use
    loadFont();         // load font -> { size, kind for font family}
    setMainWindows();   // set MainWindows
    setPlainTextEdit(); // set PlainTextEdit
    setAction();        // set QAction
    setMenubar();       // set Menu -> { QmenuBar, QMenu }
    setToolbar();       // set QToolBar

    setConnect();       // connect SIGNAL & SLOT

    show();
}

void
WriteWidget::loadImage()
{
    // loading images to use
    icon_file_Save.addFile(QStringLiteral(":/Image/save.png"),       QSize(), QIcon::Normal, QIcon::Off);
    icon_Edit_Cut.addFile(QStringLiteral(":/Image/cut.png"),         QSize(), QIcon::Normal, QIcon::Off);
    icon_Edit_Copy.addFile(QStringLiteral(":/Image/copy.png"),       QSize(), QIcon::Normal, QIcon::Off);
    icon_Edit_Paste.addFile(QStringLiteral(":/Image/paste.png"),     QSize(), QIcon::Normal, QIcon::Off);
    icon_Edit_Clear.addFile(QStringLiteral(":/Image/delete.png"),    QSize(), QIcon::Normal, QIcon::Off);
    icon_Edit_fontIcon.addFile(QStringLiteral(":/Image/font.png"),   QSize(), QIcon::Normal, QIcon::Off);
    icon_Edit_fontColor.addFile(QStringLiteral(":/Image/color.png"), QSize(), QIcon::Normal, QIcon::Off);

    icon_vie_Theme_Dark.addFile(QStringLiteral(":/Image/Dark.png"),     QSize(), QIcon::Normal, QIcon::Off);
    icon_vie_Theme_Custom.addFile(QStringLiteral(":/Image/Custom.png"), QSize(), QIcon::Normal, QIcon::Off);
}

void
WriteWidget::loadFont()
{
    this->font.setFamily(QStringLiteral("Nto Serif SC Medium"));
    this->font.setPointSize(12);
    this->font.setBold(true);

    this->font_PlainTextEdit.setFamily(QStringLiteral("Consolas"));
    this->font_PlainTextEdit.setPointSize(12);
}

void
WriteWidget::setMainWindows()
{
    // set MainWindows
    this->setWindowTitle("Alice");
    this->resize(1005, 855);

    // set MainWindows's style , but the effect was toolBar's color have changed...
    QPalette palette;
    QColor color("#4c4a48");
    palette.setColor(QPalette::Window, color);
    this->setPalette(palette);
}

void
WriteWidget::setPlainTextEdit()
{
    // set QPlainTextEdit
    display = new QPlainTextEdit;
    titlePart = new QLineEdit;
    tagPart = new QLineEdit;
    partWidget = new QWidget;
    mainWidget = new QWidget;

    partLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;

    titlePart->setMinimumSize(QSize(700, 55));
    titlePart->setMaximumSize(QSize(700, 55));
    tagPart->setMinimumSize(QSize(150, 55));
    tagPart->setMaximumSize(QSize(150, 55));
    display->setMinimumSize(QSize(995, 750));
    display->setMinimumSize(QSize(995, 750));

    partLayout->addWidget(titlePart);
    partLayout->addWidget(tagPart);
//    partLayout->setStretchFactor(titlePart, 5);
//    partLayout->setStretchFactor(tagPart, 2);
    partWidget->setLayout(partLayout);

    mainLayout->addWidget(partWidget);
    mainLayout->addWidget(display);

    // 嵌套使用 setStretchFactor() 会产生一种莫名其妙的 bug ，
    // 但是，貌似只有这里会产生这种 bug ，我在另一个项目中对 bug 进行复原，结果并没有
    // 至于原因是什么，暂不清楚，只是找到了临时的解决方案 —— 就是不采用嵌套 setStretchFactor() 设计
    // 不愧是我 (:
//    mainLayout->setStretchFactor(partWidget, 3);
//    mainLayout->setStretchFactor(display, 7);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);

    titlePart->setPlaceholderText(QString("文章标题"));
    tagPart->setPlaceholderText(QString("文章标签"));
    display->setFont(font_PlainTextEdit);
    font.setPointSize(16);
    titlePart->setFont(font);
    tagPart->setFont(font);
    font.setPointSize(12);
}

void
WriteWidget::setAction()
{
    // set QAction
    act_fil_PublicArticles = new QAction("PublicArticles");
    act_edi_Copy = new QAction("Copy");
    act_edi_Cut  = new QAction("Cut");
    act_edi_Paste = new QAction("Paste");
    act_edi_Clear = new QAction("Clear");
    act_edi_fontIcon  = new QAction("fontIcon");
    act_edi_fontcolor = new QAction("fontColor");
    act_vie_Theme_Dark   = new QAction("Dark");
    act_vie_Theme_Light  = new QAction("Light");
    act_vie_Theme_Pink   = new QAction("Pink");
    act_vie_Theme_Custom = new QAction("Custom");

    // action to add icon
    act_fil_PublicArticles->setIcon(icon_file_Save);

    act_edi_Cut->setIcon(icon_Edit_Cut);
    act_edi_Copy->setIcon(icon_Edit_Copy);
    act_edi_Paste->setIcon(icon_Edit_Paste);
    act_edi_Clear->setIcon(icon_Edit_Clear);
    act_edi_fontIcon->setIcon(icon_Edit_fontIcon);
    act_edi_fontcolor->setIcon(icon_Edit_fontColor);

    act_vie_Theme_Dark->setIcon(icon_vie_Theme_Dark);
    act_vie_Theme_Custom->setIcon(icon_vie_Theme_Custom);

    // action to add shortcuts
//    act_fil_New->setShortcut(tr("ctrl+d"));

    // action to add statusTip
//    act_fil_New->setStatusTip("new file");
}

void
WriteWidget::setMenubar()
{
    // set Menu -> { QmenuBar, QMenu }
    menuBar   = new QMenuBar(this);
    menu_File = new QMenu("File", menuBar);
    menu_Edit = new QMenu("Edit", menuBar);
    menu_View = new QMenu("View", menuBar);
    menu_View_Theme = new QMenu("Theme", menu_View);

    this->setMenuBar(menuBar);
    menuBar->addAction(menu_File->menuAction());
    menuBar->addAction(menu_Edit->menuAction());
    menuBar->addAction(menu_View->menuAction());
    menu_View->addAction(menu_View_Theme->menuAction());
    //    menuBar->addMenu(menu_File);  // 使用这两段和上面两端的区别就是，
    //    menuBar->addMenu(menu_Edit);  // 上面两端的作用只是帮助我们减轻将 action 添加进 menu 的重复 coding || PS：action 对象仍然属于 mainWindows
//    menu_File->addAction(act_fil_New);
//    menu_File->addAction(act_fil_Open);
    menu_File->addAction(act_fil_PublicArticles);
//    menu_File->addAction(act_fil_SaveAs);

    menu_Edit->addAction(act_edi_Copy);
    menu_Edit->addAction(act_edi_Cut);
    menu_Edit->addAction(act_edi_Paste);
    menu_Edit->addAction(act_edi_Clear);
    menu_Edit->addAction(act_edi_fontIcon);
    menu_Edit->addAction(act_edi_fontcolor);

    menu_View_Theme->addAction(act_vie_Theme_Dark);
    menu_View_Theme->addAction(act_vie_Theme_Light);
    menu_View_Theme->addAction(act_vie_Theme_Pink);
    menu_View_Theme->addAction(act_vie_Theme_Custom);

    menuBar->setFont(font);
    menu_File->setFont(font);
    menu_Edit->setFont(font);
    menu_View->setFont(font);
    menu_View_Theme->setFont(font);

    // set styleSheet for menuBar & menu
    menuBar->setStyleSheet("\
                           QMenuBar{background-color:#4c4a48;}\
                           QMenuBar:item{background-color:#4c4a48;}\
                           QMenuBar::item:selected {background:rgba(82,130,164,1);border:1px solid rgba(82,130,164,1);}\
                           ");
    menu_File->setStyleSheet("\
                             QMenu{background-color:#4c4a48;}\
                             QMenu:item{background-color:#4c4a48;}\
                             ");
    menu_Edit->setStyleSheet("\
                             QMenu{background-color:#4c4a48;}\
                             QMenu:item{background-color:#4c4a48;}\
                             ");
    menu_View->setStyleSheet("\
                             QMenu{background-color:#4c4a48;}\
                             QMenu:item{background-color:#4c4a48;}\
                             ");
}

void
WriteWidget::setToolbar()
{
    // set QToolBar
    toolBar = new QToolBar(this);

    this->addToolBar(toolBar);
    toolBar->addAction(act_fil_PublicArticles);

    toolBar->addAction(act_edi_Copy);
    toolBar->addAction(act_edi_Cut);
    toolBar->addAction(act_edi_Paste);
    toolBar->addAction(act_edi_Clear);
    toolBar->addAction(act_edi_fontIcon);
    toolBar->addAction(act_edi_fontcolor);

    toolBar->addAction(act_vie_Theme_Dark);
    toolBar->addAction(act_vie_Theme_Custom);

    toolBar->setIconSize(QSize(50, 50));
}

void
WriteWidget::setConnect()
{
    // connect SIGNAL & SLOT
    connect(this->act_fil_PublicArticles,   SIGNAL(triggered()), this, SLOT(slot_act_File_PublicArticles()));

    connect(this->act_edi_Copy,      SIGNAL(triggered()), this, SLOT(slot_act_Edit_Copy()));
    connect(this->act_edi_Cut,       SIGNAL(triggered()), this, SLOT(slot_act_Edit_Cut()));
    connect(this->act_edi_Paste,     SIGNAL(triggered()), this, SLOT(slot_act_Edit_Paste()));
    connect(this->act_edi_Clear,     SIGNAL(triggered()), this, SLOT(slot_act_Edit_Clear()));
    connect(this->act_edi_fontIcon,  SIGNAL(triggered()), this, SLOT(slot_act_Edit_Iconfont()));
    connect(this->act_edi_fontcolor, SIGNAL(triggered()), this, SLOT(slot_act_Edit_Colorfont()));

    connect(this->act_vie_Theme_Dark,   SIGNAL(triggered()), this, SLOT(slot_act_View_Theme_Dark()));
    connect(this->act_vie_Theme_Light,  SIGNAL(triggered()), this, SLOT(slot_act_View_Theme_Light()));
    connect(this->act_vie_Theme_Pink,   SIGNAL(triggered()), this, SLOT(slot_act_View_Theme_Pink()));
    connect(this->act_vie_Theme_Custom, SIGNAL(triggered()), this, SLOT(slot_act_View_Theme_Custom()));
}

inline void     // 封装的 save 用于 save 和 saveAs 的复用！
WriteWidget::PUBLICARTICLES()
{
    QFile myFile(this->fileName);

    if (!myFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(this, "Error Message", "Please Select a Text File!");
        return ;
    }
    QTextStream out(&myFile);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    out.setCodec(codec);

    QString fileString =  this->display->toPlainText();
    QStringList _textLineList = fileString.split("\n");

    QProgressDialog* progress = new QProgressDialog(tr("cloning..."),
                    tr("cancel"), 0, 100, this);    // 初始化进度条
    progress->setWindowTitle("ProgressDialog");
    progress->setWindowModality(Qt::WindowModal);   // 具体意义暂不明

    float file_size = _textLineList.length(), lines = 0;
    for (QStringList::ConstIterator _iter = _textLineList.begin(); ; _iter++)
    {
        if (_iter == --(_textLineList.end()))
        {
            out << *_iter;
            lines++;
            int count = lines / file_size * 100;
            progress->setValue(count);

            break;
        }

        out << *_iter << endl;
        lines++;

        int count = lines / file_size * 100;
        progress->setValue(count);
        QThread::msleep(1);
        if (progress->wasCanceled())
            break;
    }

    myFile.close();
}

void
WriteWidget::slot_act_File_PublicArticles()       // 只需要考虑 “new txt” & “txt” 的区别，两者需要的目标源不同
{
    QString tmp = fileName;
    QString str = QString("P%1_%2.txt").arg(UID).arg(userPaperCount + 1);
    qDebug() << "WrietWidget -> PublicArticle's Name is " << str;
    fileName = fileName + str;

    _paperTag = tagPart->text();
    _paperTitle = titlePart->text();
    QDateTime todayDate = QDateTime::currentDateTime();
    _toDay= todayDate.toString("M/d/yyyy");

    PUBLICARTICLES();

    userPaperCount++;   // 更新用户文章数量 。。。 本打算和 paperWidget class 中进行共享，已实现同步更新，但是，LAJI code 堆的太多，改起来费劲儿
    close();            // 关闭文章创作界面，防止用户显连续点击 PublicArticle 功能按钮，暂时没有添加用于应对连续发布文章导致的逻辑漏洞的防护
    fileName = tmp;     // fileName 重置
}

void
WriteWidget::slot_act_Edit_Copy()
{
    if (!this->copyString)
        copyString = new QString(this->display->toPlainText());
    else {
        delete this->copyString;
        copyString = new QString(this->display->toPlainText());

    }
}

/* cut 为对整篇文章进行剪切 */
void
WriteWidget::slot_act_Edit_Cut()
{
    slot_act_Edit_Copy();
    this->display->clear();
}

void
WriteWidget::slot_act_Edit_Paste()
{
    if (!this->copyString) {
        QMessageBox::information(this, "Tip Message", "Share Plate is void...");
        return ;
    }
    this->display->appendPlainText(*this->copyString);
//    delete this->copyString;      // 本想设计为：cut 只能粘贴一次的。。
}

void
WriteWidget::slot_act_Edit_Clear()
{
    this->display->clear();
}

void
WriteWidget::slot_act_Edit_Iconfont()
{
    bool ok = true;
    QFont font = QFontDialog::getFont(&ok, this->display->font(), this, "fontDialog");
    if (ok)
        this->display->setFont(font);
}

void
WriteWidget::slot_act_Edit_Colorfont()
{
    QPalette palette = this->display->palette();
    QColor color = QColorDialog::getColor(palette.color(QPalette::Text), this, "Dialog of fontColor");
    if (color.isValid()) {
        palette.setColor(QPalette::Text, color);
        this->display->setPalette(palette);
//        this->display->setBackgroundVisible(true);
    }
}

void
WriteWidget::THEME(const QString& back, const QString& font)
{
    QPalette palette;
    QColor color_Back(back);
    QColor color_Font(font);
    palette.setColor(QPalette::Base, color_Back);
    palette.setColor(QPalette::Text, color_Font);
    this->display->setPalette(palette);
}

void
WriteWidget::slot_act_View_Theme_Dark()
{
    THEME("#181a1b", "#64de98");
//    THEME_FONT("#64de98");
}

void
WriteWidget::slot_act_View_Theme_Light()
{
    THEME("#f8edda", "#212121");
}

void
WriteWidget::slot_act_View_Theme_Pink()
{
    THEME("#fb7299", "#ffffff");
//    THEME_FONT("#ffffff");
}

void
WriteWidget::slot_act_View_Theme_Custom()
{
    QPalette palette = this->display->palette();
    QColor color = QColorDialog::getColor(palette.color(QPalette::Background), this, "Choose a color...");
    if (color.isValid()) {
        palette.setColor(QPalette::Base, color);
        this->display->setPalette(palette);
    }

}
