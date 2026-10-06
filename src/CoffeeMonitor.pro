QT += widgets network
requires(qtConfig(combobox))

CONFIG += c++17

# RC_ICONS = hzdr.ico
RC_FILE = appinfo.rc

SOURCES       = main.cpp \
                homeassistantclient.cpp \
                window.cpp

HEADERS       = window.h \
                homeassistantclient.h \
                token.h

RESOURCES     = CoffeeMonitor.qrc



# install
#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/desktop/systray
#INSTALLS += target

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    HZDR.ico
