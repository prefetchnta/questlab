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

#ifndef QBLUETOOTHSERVICEDISCOVERYAGENT_H
#define QBLUETOOTHSERVICEDISCOVERYAGENT_H

#include <QtBluetooth/qbluetoothglobal.h>

#include <QObject>

#include <QtBluetooth/QBluetoothServiceInfo>
#include <QtBluetooth/QBluetoothUuid>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>

QT_BEGIN_NAMESPACE

class QBluetoothAddress;
class QBluetoothServiceDiscoveryAgentPrivate;

class Q_BLUETOOTH_EXPORT QBluetoothServiceDiscoveryAgent : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QBluetoothServiceDiscoveryAgent)

public:
    enum Error {
        NoError =  QBluetoothDeviceDiscoveryAgent::NoError,
        InputOutputError = QBluetoothDeviceDiscoveryAgent::InputOutputError,
        PoweredOffError = QBluetoothDeviceDiscoveryAgent::PoweredOffError,
        UnknownError = QBluetoothDeviceDiscoveryAgent::UnknownError //=100
        //New Errors must be added after Unknown Error the space before UnknownError is reserved
        //for future device discovery errors
    };

    enum DiscoveryMode {
        MinimalDiscovery,
        FullDiscovery
    };

    QBluetoothServiceDiscoveryAgent(QObject *parent = 0);
    explicit QBluetoothServiceDiscoveryAgent(const QBluetoothAddress &deviceAdapter, QObject *parent = 0);
    ~QBluetoothServiceDiscoveryAgent();

    bool isActive() const;

    Error error() const;
    QString errorString() const;

    QList<QBluetoothServiceInfo> discoveredServices() const;

    void setUuidFilter(const QList<QBluetoothUuid> &uuids);
    void setUuidFilter(const QBluetoothUuid &uuid);
    QList<QBluetoothUuid> uuidFilter() const;
    bool setRemoteAddress(const QBluetoothAddress &address);
    QBluetoothAddress remoteAddress() const;

public Q_SLOTS:
    void start(DiscoveryMode mode = MinimalDiscovery);
    void stop();
    void clear();

Q_SIGNALS:
    void serviceDiscovered(const QBluetoothServiceInfo &info);
    void finished();
    void canceled();
    void error(QBluetoothServiceDiscoveryAgent::Error error);

private:
    QBluetoothServiceDiscoveryAgentPrivate *d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_deviceDiscovered(const QBluetoothDeviceInfo &info))
    Q_PRIVATE_SLOT(d_func(), void _q_deviceDiscoveryFinished())
    Q_PRIVATE_SLOT(d_func(), void _q_deviceDiscoveryError(QBluetoothDeviceDiscoveryAgent::Error))
    Q_PRIVATE_SLOT(d_func(), void _q_serviceDiscoveryFinished())
#ifdef QT_BLUEZ_BLUETOOTH
    Q_PRIVATE_SLOT(d_func(), void _q_discoveredServices(QDBusPendingCallWatcher*))
    Q_PRIVATE_SLOT(d_func(), void _q_createdDevice(QDBusPendingCallWatcher*))
#endif
};

QT_END_NAMESPACE

#endif
