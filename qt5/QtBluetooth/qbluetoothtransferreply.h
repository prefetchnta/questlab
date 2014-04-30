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

#ifndef QBLUETOOTHTRANSFERREPLY_H
#define QBLUETOOTHTRANSFERREPLY_H

#include <QtCore/QIODevice>

#include <QtBluetooth/QBluetoothTransferRequest>
#include <QtBluetooth/QBluetoothTransferManager>

QT_BEGIN_NAMESPACE

class QBluetoothTransferReplyPrivate;

class Q_BLUETOOTH_EXPORT QBluetoothTransferReply : public QObject
{
    Q_OBJECT

public:
    enum TransferError {
        NoError = 0,
        UnknownError,
        FileNotFoundError,
        HostNotFoundError,
        UserCanceledTransferError
    };


    ~QBluetoothTransferReply();

    virtual bool isFinished() const = 0;
    virtual bool isRunning() const = 0;

    QBluetoothTransferManager *manager() const;

    virtual TransferError error() const = 0;
    virtual QString errorString() const = 0;

    QBluetoothTransferRequest request() const;

public Q_SLOTS:
    void abort();

Q_SIGNALS:
    void finished(QBluetoothTransferReply *);
    void transferProgress(qint64 bytesTransferred, qint64 bytesTotal);

protected:
    explicit QBluetoothTransferReply(QObject *parent = 0);
    void setManager(QBluetoothTransferManager *manager);
    void setRequest(const QBluetoothTransferRequest &request);

protected:
    QBluetoothTransferReplyPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QBluetoothTransferReply)

};

QT_END_NAMESPACE

#endif // QBLUETOOTHTRANSFERREPLY_H
