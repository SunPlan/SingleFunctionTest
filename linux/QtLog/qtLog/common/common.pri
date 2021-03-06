QT += core gui dbus
CONFIG += c++11 no_keywords link_pkgconfig x11extras
CONFIG -= app_bundle

INCLUDEPATH += -I $$PWD/

#PLUGIN_INSTALL_DIRS = $$[QT_INSTALL_LIBS]/ukui-settings-daemon

PKGCONFIG += \
        glib-2.0\
        gio-2.0 libxklavier \
        x11 xrandr xtst

SOURCES += \
    $$PWD/clib-syslog.c

HEADERS += \
    $$PWD/clib-syslog.h \
    $$PWD/main.h
