
#ifndef __QL_QSTCOMMINT_H__
#define __QL_QSTCOMMINT_H__

#include "QstComm.h"

#pragma warning (disable: 4127)
#pragma warning (disable: 4512)

#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

/* 多线程锁简化宏 */
#define _ENTER_COM_SINGLE_  \
    mtlock_acquire(&(((sQstComm*)parm)->lock));
#define _LEAVE_COM_SINGLE_  \
    mtlock_release(&(((sQstComm*)parm)->lock));

/**************************************************/
/* 为了解决多线程问题的信号类 (需要人肉 moc 处理) */
/**************************************************/
class CTextOper : public QObject
{
    Q_OBJECT

public:
    CTextOper (QTextEdit* edit)
    {
        connect(this, SIGNAL(allClear()),
                edit, SLOT(clear()));
        connect(this, SIGNAL(setText(const QString&)),
                edit, SLOT(setPlainText(const QString&)));
        connect(this, SIGNAL(setHtml(const QString&)),
                edit, SLOT(setHtml(const QString&)));
    }

public:
    void clear ()
    {
        emit allClear();
    }
    void text (const QString& text)
    {
        emit setText(text);
    }
    void html (const QString& html)
    {
        emit setHtml(html);
    }

signals:
    void allClear ();
    void setText (const QString& text);
    void setHtml (const QString& html);
};

#endif  /* !__QL_QSTCOMMINT_H__ */
