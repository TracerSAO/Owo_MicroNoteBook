#ifndef READWIDGET_H
#define READWIDGET_H

#include <QWidget>
#include <QString>
#include <QTextBrowser>
#include <QLabel>
#include <QFont>
#include <QIODevice>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTextCodec>

class ReadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReadWidget(const QString &paperPath, const QString &paperTitle, QWidget *parent = nullptr);

signals:

private:
    void init();
    void setText();

private:
    QString              paperPath;
    QString              paperTile;
    QLabel              *titleLabel;
    QTextBrowser        *textBrowser;
};

#endif // READWIDGET_H
