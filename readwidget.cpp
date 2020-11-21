#include "readwidget.h"

ReadWidget::ReadWidget(const QString &_paperPath, const QString &_paperTitle, QWidget *parent):
    QWidget(parent), paperPath(_paperPath), paperTile(_paperTitle)
{
    init();
    show();
}

void
ReadWidget::init()
{
    resize(1000, 800);
    // No.1 设置 titleLabel - 文章标题显示区
    titleLabel = new QLabel(this);
    titleLabel->setGeometry(5, 2, 990, 70);
    titleLabel->setText(paperTile);
    titleLabel->setAlignment(Qt::AlignCenter);

    QFont font;
    font.setFamily(QStringLiteral("Cascadia Mono"));
    font.setPointSize(20);
    titleLabel->setFont(font);

    // No.2 设置 textBrowser - 文章内容显示区
    textBrowser = new QTextBrowser(this);
    textBrowser->setGeometry(5, 88, 990, 610);

    font.setPointSize(12);
    textBrowser->setFont(font);
    setText();
    // No.3 头图，呵呵o(*￣︶￣*)o，不存在的
}

void
ReadWidget::setText()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

    QFile file(paperPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);
        while (!textStream.atEnd())
        {
            textBrowser->append(textStream.readLine());
//            if (!textStream.atEnd())
//                textBrowser->append(QString('\n'));
        }
        textBrowser->moveCursor(QTextCursor::Start);    // 定位光标回到开始位置 -> 即回到首行
    }
    else
    {
        // 为 TextBrowser 设置默认的文本信息： 倒霉倒霉倒霉。。。);
        textBrowser->setText(QString("倒霉倒霉倒霉 (っ °Д °;)っ"));
        qDebug() << "File open FAILED...";
    }
}
