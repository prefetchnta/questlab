/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtDBus module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDBUSREPLY_H
#define QDBUSREPLY_H

#include <QtCore/qglobal.h>
#include <QtCore/qvariant.h>

#include <QtDBus/qdbusmacros.h>
#include <QtDBus/qdbusmessage.h>
#include <QtDBus/qdbuserror.h>
#include <QtDBus/qdbusextratypes.h>
#include <QtDBus/qdbuspendingreply.h>

#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE


Q_DBUS_EXPORT void qDBusReplyFill(const QDBusMessage &reply, QDBusError &error, QVariant &data);

template<typename T>
class QDBusReply
{
    typedef T Type;
public:
    inline QDBusReply(const QDBusMessage &reply)
    {
        *this = reply;
    }
    inline QDBusReply& operator=(const QDBusMessage &reply)
    {
        QVariant data(qMetaTypeId<Type>(), reinterpret_cast<void*>(0));
        qDBusReplyFill(reply, m_error, data);
        m_data = qvariant_cast<Type>(data);
        return *this;
    }

    inline QDBusReply(const QDBusPendingCall &pcall)
    {
        *this = pcall;
    }
    inline QDBusReply &operator=(const QDBusPendingCall &pcall)
    {
        QDBusPendingCall other(pcall);
        other.waitForFinished();
        return *this = other.reply();
    }
    inline QDBusReply(const QDBusPendingReply<T> &reply)
    {
        *this = static_cast<QDBusPendingCall>(reply);
    }

    inline QDBusReply(const QDBusError &dbusError = QDBusError())
        : m_error(dbusError), m_data(Type())
    {
    }
    inline QDBusReply& operator=(const QDBusError& dbusError)
    {
        m_error = dbusError;
        m_data = Type();
        return *this;
    }

    inline QDBusReply& operator=(const QDBusReply& other)
    {
        m_error = other.m_error;
        m_data = other.m_data;
        return *this;
    }

    inline bool isValid() const { return !m_error.isValid(); }

    inline const QDBusError& error() { return m_error; }
    inline const QDBusError& error() const { return m_error; }

    inline Type value() const
    {
        return m_data;
    }

    inline operator Type () const
    {
        return m_data;
    }

private:
    QDBusError m_error;
    Type m_data;
};

# ifndef Q_QDOC
// specialize for QVariant:
template<> inline QDBusReply<QVariant>&
QDBusReply<QVariant>::operator=(const QDBusMessage &reply)
{
    void *null = 0;
    QVariant data(qMetaTypeId<QDBusVariant>(), null);
    qDBusReplyFill(reply, m_error, data);
    m_data = qvariant_cast<QDBusVariant>(data).variant();
    return *this;
}

// specialize for void:
template<>
class QDBusReply<void>
{
public:
    inline QDBusReply(const QDBusMessage &reply)
        : m_error(reply)
    {
    }
    inline QDBusReply& operator=(const QDBusMessage &reply)
    {
        m_error = QDBusError(reply);
        return *this;
    }
    inline QDBusReply(const QDBusError &dbusError = QDBusError())
        : m_error(dbusError)
    {
    }
    inline QDBusReply(const QDBusPendingCall &pcall)
    {
        *this = pcall;
    }
    inline QDBusReply &operator=(const QDBusPendingCall &pcall)
    {
        QDBusPendingCall other(pcall);
        other.waitForFinished();
        return *this = other.reply();
    }
    inline QDBusReply& operator=(const QDBusError& dbusError)
    {
        m_error = dbusError;
        return *this;
    }

    inline QDBusReply& operator=(const QDBusReply& other)
    {
        m_error = other.m_error;
        return *this;
    }

    inline bool isValid() const { return !m_error.isValid(); }

    inline const QDBusError& error() { return m_error; }
    inline const QDBusError& error() const { return m_error; }

private:
    QDBusError m_error;
};
# endif

QT_END_NAMESPACE

#endif // QT_NO_DBUS
#endif
