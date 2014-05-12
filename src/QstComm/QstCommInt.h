
#ifndef __QL_QSTCOMMINT_H__
#define __QL_QSTCOMMINT_H__

#include "QstComm.h"

#pragma warning (push)
#pragma warning (disable: 4127)
#pragma warning (disable: 4512)
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#pragma warning (pop)

/************************/
/* 实现自动滚动的文本框 */
/************************/
class CTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    CTextEdit (QWidget* parent = 0) : QTextEdit(parent)
    {
    }

public slots:
    void autoScroll ()
    {
        QTextCursor cursor = this->textCursor();

        cursor.movePosition(QTextCursor::End);

        this->setTextCursor(cursor);
    }
};

/**************************************************/
/* 为了解决多线程问题的信号类 (需要人肉 moc 处理) */
/**************************************************/
class CTextOper : public QObject
{
    Q_OBJECT

public:
    CTextOper (QTextEdit* edit)
    {
        connect(this, SIGNAL(gotoEnd()),
                edit, SLOT(autoScroll()));
        connect(this, SIGNAL(allClear()),
                edit, SLOT(clear()));
        connect(this, SIGNAL(setText(const QString&)),
                edit, SLOT(insertPlainText(const QString&)));
        connect(this, SIGNAL(setHtml(const QString&)),
                edit, SLOT(insertHtml(const QString&)));
    }

public:
    void clear ()
    {
        emit allClear();
    }
    void text (const QString& text)
    {
        emit gotoEnd();
        emit setText(text);
    }
    void html (const QString& html)
    {
        emit gotoEnd();
        emit setHtml(html);
    }

signals:
    void gotoEnd ();
    void allClear ();
    void setText (const QString& text);
    void setHtml (const QString& html);
};

#endif  /* !__QL_QSTCOMMINT_H__ */
