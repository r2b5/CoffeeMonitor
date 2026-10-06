// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include "qlabel.h"
#include <QSystemTrayIcon>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QDialog>
#include "homeassistantclient.h"
#include <QTimer>
#include <QScreen>
#include "token.h"


QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class Window : public QDialog
{
    Q_OBJECT

public:
    Window();

    void setVisible(bool visible) override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void setIcon(int index);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();
    void messageClicked();

private:
    void createIconGroupBox();
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();

    QGroupBox *iconGroupBox;
    QLabel *iconLabel;
    QComboBox *iconComboBox;
    QCheckBox *showIconCheckBox;

    QGroupBox *messageGroupBox;
    QLabel *typeLabel;
    QLabel *durationLabel;
    QLabel *durationWarningLabel;
    QLabel *titleLabel;
    QLabel *bodyLabel;
    QComboBox *typeComboBox;
    QSpinBox *durationSpinBox;
    QLineEdit *titleEdit;
    QTextEdit *bodyEdit;
    QPushButton *showMessageButton;


    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;


    QLabel *Kanne_1;
    QLabel *Kanne_2;
    QLabel *Kanne_3;
    QLabel *Steam_1,*Steam_2,*Steam_3;
    QLabel *Temp_1,*Temp_2,*Temp_3;
    QLabel *Bat_1,*Bat_2,*Bat_3;


    QString url = "http://fwd101:8123";
    QString token = MYTOKEN; // Dein langer Token

    QVector<HomeAssistantClient*> haClients;
    void registerEntity(const QString &entityId, QLabel *targetLabel);
    void SetTaskBarLabel();
    QLabel *taskbarLabel;

    QTimer *ui_timer;
    void TimerFunc();


};
//! [0]

#endif // QT_NO_SYSTEMTRAYICON

#endif
