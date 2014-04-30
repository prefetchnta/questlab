/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
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

#ifndef QBLUETOOTHADDRESS_H
#define QBLUETOOTHADDRESS_H

#include <QtBluetooth/qbluetoothglobal.h>

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QMetaType>

QT_BEGIN_NAMESPACE

class QBluetoothAddressPrivate;

class Q_BLUETOOTH_EXPORT QBluetoothAddress
{
public:
    QBluetoothAddress();
    explicit QBluetoothAddress(quint64 address);
    explicit QBluetoothAddress(const QString &address);
    QBluetoothAddress(const QBluetoothAddress &other);
    ~QBluetoothAddress();

    QBluetoothAddress &operator=(const QBluetoothAddress &other);

    bool isNull() const;

    void clear();

    bool operator<(const QBluetoothAddress &other) const;
    bool operator==(const QBluetoothAddress &other) const;
    inline bool operator!=(const QBluetoothAddress &other) const { return !operator==(other); }

    quint64 toUInt64() const;
    QString toString() const;

private:
    Q_DECLARE_PRIVATE(QBluetoothAddress)
    QBluetoothAddressPrivate *d_ptr;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QBluetoothAddress)

#endif
