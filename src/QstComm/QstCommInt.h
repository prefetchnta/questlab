
#ifndef __QL_QSTCOMMINT_H__
#define __QL_QSTCOMMINT_H__

#include "QstComm.h"

#pragma warning (push)
#pragma warning (disable: 4481)
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
    void setupFont (const QFont& font)
    {
        this->setFont(font);
    }
};

/**************************************************/
/* 为了解决多线程问题的信号类 (需要人肉 moc 处理) */
/**************************************************/
class CTextOper : public QObject
{
    Q_OBJECT

public:
    CTextOper (QMainWindow* form, QTextEdit* edit)
    {
        connect(this, SIGNAL(quit()), form, SLOT(close()));
        connect(this, SIGNAL(gotoEnd()), edit, SLOT(autoScroll()));
        connect(this, SIGNAL(allClear()), edit, SLOT(clear()));
        connect(this, SIGNAL(setText(const QString&)),
                edit, SLOT(insertPlainText(const QString&)));
        connect(this, SIGNAL(setHtml(const QString&)),
                edit, SLOT(insertHtml(const QString&)));
        connect(this, SIGNAL(setFont(const QFont&)),
                edit, SLOT(setupFont(const QFont&)));
        connect(this, SIGNAL(setStyle(const QString&)),
                edit, SLOT(setStyleSheet(const QString&)));
    }

public:
    void exit ()
    {
        emit quit();
    }
    void clear ()
    {
        emit setHtml("x");
        emit allClear();
    }
    void text (const QString& text)
    {
        FILE*   fp;

        emit gotoEnd();
        emit setText(text);
        emit gotoEnd();

        fp = fopen(EXE_XNAME ".txt", "a+b");
        if (fp != NULL) {
            fwrite(text.data(), text.size(), 1, fp);
            fclose(fp);
        }
    }
    void html (const QString& html)
    {
        FILE*   fp;

        emit gotoEnd();
        emit setHtml(html);
        emit gotoEnd();

        fp = fopen(EXE_XNAME ".htm", "a+b");
        if (fp != NULL) {
            fwrite(html.data(), html.size(), 1, fp);
            fclose(fp);
        }
    }
    void setup (const QFont& font, const QString& style)
    {
        emit setFont(font);
        emit setStyle(style);
    }

signals:
    void quit ();
    void gotoEnd ();
    void allClear ();
    void setText (const QString& text);
    void setHtml (const QString& html);
    void setFont (const QFont& font);
    void setStyle (const QString& style);
};

#endif  /* !__QL_QSTCOMMINT_H__ */
