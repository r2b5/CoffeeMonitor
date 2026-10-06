// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause


#include "window.h"


#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>


#ifdef Q_OS_WIN
#include <windows.h>
#endif
//! [0]
Window::Window()
{
    createIconGroupBox();
    createMessageGroupBox();

    iconLabel->setMinimumWidth(durationLabel->sizeHint().width());

    createActions();
    createTrayIcon();

    connect(showMessageButton, &QAbstractButton::clicked, this, &Window::showMessage);
    connect(showIconCheckBox, &QAbstractButton::toggled, trayIcon, &QSystemTrayIcon::setVisible);
    connect(iconComboBox, &QComboBox::currentIndexChanged,
            this, &Window::setIcon);
    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &Window::messageClicked);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Window::iconActivated);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(iconGroupBox);
    mainLayout->addWidget(messageGroupBox);
    setLayout(mainLayout);

    iconComboBox->setCurrentIndex(1);


    registerEntity("sensor.kanne_1_fullstand", Kanne_1);
    registerEntity("sensor.kanne_2_fullstand", Kanne_2);
    registerEntity("sensor.kanne_3_fullstand", Kanne_3);
    registerEntity("sensor.kanne_1_temperatur", Temp_1);
    registerEntity("sensor.kanne_2_temperatur", Temp_2);
    registerEntity("sensor.kanne_3_temperatur", Temp_3);
    registerEntity("sensor.kanne_1_steam_level", Steam_1);
    registerEntity("sensor.kanne_2_steam_level", Steam_2);
    registerEntity("sensor.kanne_3_steam_level", Steam_3);
    registerEntity("sensor.kanne_1_batterie", Bat_1);
    registerEntity("sensor.kanne_2_batterie", Bat_2);
    registerEntity("sensor.kanne_3_batterie", Bat_3);

    ui_timer = new QTimer;
    ui_timer->setSingleShot(true);
    connect(ui_timer, &QTimer::timeout, this, &Window::TimerFunc );
    ui_timer->start(50);

    taskbarLabel = new QLabel(nullptr);

    taskbarLabel->setWindowFlags(
        Qt::Tool |
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint
        );


    taskbarLabel->setAttribute(Qt::WA_TranslucentBackground);

    taskbarLabel->setStyleSheet(
        "QLabel {"
        "  background: #202020;"
        "  color: white;"
        "  padding: 4px 8px;"
        "  font-size: 12px;"
        "}"
        );


    SetTaskBarLabel();
    taskbarLabel->adjustSize();
    taskbarLabel->show();


#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(taskbarLabel->winId());

    SetWindowPos(
        hwnd,
        HWND_TOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE |
            SWP_NOSIZE |
            SWP_NOACTIVATE
        );
#endif


    taskbarLabel->setAlignment(Qt::AlignRight);

    QScreen *screen = QGuiApplication::primaryScreen();

    if (screen) {
        QRect available = screen->availableGeometry();

        int x = available.right() - taskbarLabel->width();
        int y = available.bottom() - taskbarLabel->height();

        taskbarLabel->move(x, y);
    }




    trayIcon->show();
    setWindowTitle(tr("CoffeeMonitor"));
    resize(400, 300);

}
//! [0]

//! [1]
void Window::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}
//! [1]

//! [2]
void Window::closeEvent(QCloseEvent *event)
{
    if (!event->spontaneous() || !isVisible())
        return;
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("CoffeeMonitor"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}
//! [2]

//! [3]
void Window::setIcon(int index)
{
    QIcon icon = iconComboBox->itemIcon(index);
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->setToolTip(iconComboBox->itemText(index));
}
//! [3]

//! [4]
void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}
//! [4]

//! [5]
void Window::showMessage()
{
    showIconCheckBox->setChecked(true);
    int selectedIcon = typeComboBox->itemData(typeComboBox->currentIndex()).toInt();
    QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(selectedIcon);

    if (selectedIcon == -1) { // custom icon
        QIcon icon(iconComboBox->itemIcon(iconComboBox->currentIndex()));
        trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), icon,
                          durationSpinBox->value() * 1000);
    } else {
        trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), msgIcon,
                          durationSpinBox->value() * 1000);
    }
}
//! [5]

//! [6]
void Window::messageClicked()
{
    QMessageBox::information(nullptr, tr("Systray"),
                             tr("Sorry, I already gave what help I could.\n"
                                "Maybe you should try asking a human?"));
}
//! [6]

void Window::createIconGroupBox()
{
    iconGroupBox = new QGroupBox(tr("Tray Icon"));

    iconLabel = new QLabel("Icon:");

    iconComboBox = new QComboBox;
    iconComboBox->addItem(QIcon(":/images/bad.png"), tr("Bad"));
    iconComboBox->addItem(QIcon(":/images/pot.png"), tr("Pot"));
    iconComboBox->addItem(QIcon(":/images/heart.png"), tr("Heart"));
    iconComboBox->addItem(QIcon(":/images/trash.png"), tr("Trash"));

    showIconCheckBox = new QCheckBox(tr("Show icon"));
    showIconCheckBox->setChecked(true);

    QHBoxLayout *iconLayout = new QHBoxLayout;
    iconLayout->addWidget(iconLabel);
    iconLayout->addWidget(iconComboBox);
    iconLayout->addStretch();
    iconLayout->addWidget(showIconCheckBox);
    iconGroupBox->setLayout(iconLayout);
}

void Window::createMessageGroupBox()
{
    messageGroupBox = new QGroupBox(tr("Entities"));

    typeLabel = new QLabel(tr("Type:"));

    typeComboBox = new QComboBox;
    typeComboBox->addItem(tr("None"), QSystemTrayIcon::NoIcon);
    typeComboBox->addItem(style()->standardIcon(
            QStyle::SP_MessageBoxInformation), tr("Information"),
            QSystemTrayIcon::Information);
    typeComboBox->addItem(style()->standardIcon(
            QStyle::SP_MessageBoxWarning), tr("Warning"),
            QSystemTrayIcon::Warning);
    typeComboBox->addItem(style()->standardIcon(
            QStyle::SP_MessageBoxCritical), tr("Critical"),
            QSystemTrayIcon::Critical);
    typeComboBox->addItem(QIcon(), tr("Custom icon"),
            -1);
    typeComboBox->setCurrentIndex(1);

    durationLabel = new QLabel(tr("Duration:"));

    durationSpinBox = new QSpinBox;
    durationSpinBox->setRange(5, 60);
    durationSpinBox->setSuffix(" s");
    durationSpinBox->setValue(15);

    durationWarningLabel = new QLabel(tr("(some systems might ignore this "
                                         "hint)"));
    durationWarningLabel->setIndent(10);

    titleLabel = new QLabel(tr("Title:"));

    titleEdit = new QLineEdit(tr("The coffee is ready!"));
    bodyEdit = new QTextEdit;
    bodyEdit->setPlainText(tr("let's go..."));


    bodyLabel = new QLabel(tr("Body:"));

    Kanne_1 = new QLabel(tr("100"));
    Steam_1 = new QLabel(tr("100"));
    Temp_1 = new QLabel(tr("100"));
    Bat_1 = new QLabel(tr("100"));

    Kanne_2 = new QLabel(tr("100"));
    Steam_2 = new QLabel(tr("100"));
    Temp_2 = new QLabel(tr("100"));
    Bat_2 = new QLabel(tr("100"));

    Kanne_3 = new QLabel(tr("100"));
    Steam_3 = new QLabel(tr("100"));
    Temp_3 = new QLabel(tr("100"));
    Bat_3 = new QLabel(tr("100"));


    showMessageButton = new QPushButton(tr("Show Message"));
    showMessageButton->setDefault(true);

    QGridLayout *messageLayout = new QGridLayout;

    messageLayout->addWidget(Kanne_1, 1, 0);
    messageLayout->addWidget(Steam_1, 1, 1);
    messageLayout->addWidget(Temp_1, 1, 2);
    messageLayout->addWidget(Bat_1, 1, 3);

    messageLayout->addWidget(Kanne_2, 2, 0);
    messageLayout->addWidget(Steam_2, 2, 1);
    messageLayout->addWidget(Temp_2, 2, 2);
    messageLayout->addWidget(Bat_2, 2, 3);

    messageLayout->addWidget(Kanne_3, 3, 0);
    messageLayout->addWidget(Steam_3, 3, 1);
    messageLayout->addWidget(Temp_3, 3, 2);
    messageLayout->addWidget(Bat_3, 3, 3);

    messageLayout->addWidget(showMessageButton, 5, 4);

    messageLayout->setColumnStretch(3, 1);
    messageLayout->setRowStretch(4, 1);
    messageGroupBox->setLayout(messageLayout);
}

void Window::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
//    trayIconMenu->addAction(minimizeAction);
//    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}



void Window::TimerFunc()
{

    //haClients[0]->fetchEntityState("sensor.kanne_1_fullstand");
    haClients[0]->fetchEntityState();
    haClients[1]->fetchEntityState();
    haClients[2]->fetchEntityState();
    haClients[3]->fetchEntityState();
    haClients[4]->fetchEntityState();
    haClients[5]->fetchEntityState();
    haClients[6]->fetchEntityState();
    haClients[7]->fetchEntityState();
    haClients[8]->fetchEntityState();
    haClients[9]->fetchEntityState();
    haClients[10]->fetchEntityState();
    haClients[11]->fetchEntityState();

    SetTaskBarLabel();

    ui_timer->start(1000);

}


void Window::registerEntity(const QString &entityId,
                                QLabel *targetLabel)
{
    if (!targetLabel)
        return;

    auto *client = new HomeAssistantClient(url, token ,entityId ,this);
    haClients.append(client);

    connect(client,
            &HomeAssistantClient::entityStateReceived,
            this,
            [targetLabel, entityId]
            (const QString &incomingEntityId,
             const QString &state,
             const QJsonObject &attributes)
            {
                if (incomingEntityId == entityId) {
                    targetLabel->setText(state);
                }
            });
}

void Window::SetTaskBarLabel(){

    int Temperature_1 = Steam_1->text().toInt();
    QString TempStr1 = (Temperature_1 <= 1) ? "❄️"
                       : (Temperature_1 == 3) ? "🔥"
                                              : "🔆";

    int Temperature_2 = Steam_2->text().toInt();
    QString TempStr2 = (Temperature_2 <= 1) ? "❄️"
                       : (Temperature_2 == 3) ? "🔥"
                                              : "🔆";


    int Temperature_3 = Steam_3->text().toInt();
    QString TempStr3 = (Temperature_3 <= 1) ? "❄️"
                       : (Temperature_3 == 3) ? "🔥"
                                              : "🔆";


    QString str="";
    str+="Coffepot 1: 💧"+Kanne_1->text()+"cup | "+TempStr1+Temp_1->text()+"°C | 🔋"+Bat_1->text()+"%\n";
    str+="Coffepot 2: 💧"+Kanne_2->text()+"cup | "+TempStr2+Temp_2->text()+"°C | 🔋"+Bat_2->text()+"%\n";
    str+="Coffepot 3: 💧"+Kanne_3->text()+"cup | "+TempStr3+Temp_3->text()+"°C | 🔋"+Bat_3->text()+"%";

    taskbarLabel->setText(str);

}

#endif
