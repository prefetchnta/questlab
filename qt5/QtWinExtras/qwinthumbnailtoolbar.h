/****************************************************************************
 **
 ** Copyright (C) 2013 Ivan Vizir <define-true-false@yandex.com>
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the QtWinExtras module of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Digia.  For licensing terms and
 ** conditions see http://qt.digia.com/licensing.  For further information
 ** use the contact form at http://qt.digia.com/contact-us.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Digia gives you certain additional
 ** rights.  These rights are described in the Digia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef QWINTHUMBNAILTOOLBAR_H
#define QWINTHUMBNAILTOOLBAR_H

#include <QtCore/qobject.h>
#include <QtCore/qscopedpointer.h>
#include <QtWinExtras/qwinextrasglobal.h>

QT_BEGIN_NAMESPACE

class QWindow;
class QWinThumbnailToolButton;
class QWinThumbnailToolBarPrivate;

class Q_WINEXTRAS_EXPORT QWinThumbnailToolBar : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count STORED false)
    Q_PROPERTY(QWindow *window READ window WRITE setWindow)

public:
    explicit QWinThumbnailToolBar(QObject *parent = 0);
    ~QWinThumbnailToolBar();

    void setWindow(QWindow *window);
    QWindow *window() const;

    void addButton(QWinThumbnailToolButton *button);
    void removeButton(QWinThumbnailToolButton *button);
    void setButtons(const QList<QWinThumbnailToolButton *> &buttons);
    QList<QWinThumbnailToolButton *> buttons() const;
    int count() const;

public Q_SLOTS:
    void clear();

private:
    Q_DISABLE_COPY(QWinThumbnailToolBar)
    Q_DECLARE_PRIVATE(QWinThumbnailToolBar)
    QScopedPointer<QWinThumbnailToolBarPrivate> d_ptr;
    friend class QWinThumbnailToolButton;

    Q_PRIVATE_SLOT(d_func(), void _q_updateToolbar())
    Q_PRIVATE_SLOT(d_func(), void _q_scheduleUpdate())
};

QT_END_NAMESPACE

#endif // QWINTHUMBNAILTOOLBAR_H
