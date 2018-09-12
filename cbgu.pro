#-------------------------------------------------
#
# Project created by QtCreator 2018-08-29T12:35:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cbgu
TEMPLATE = app
CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS = cbgu.ts

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    meal.cpp \
    mealtreeitem.cpp \
    categorytreeitem.cpp \
    abstracttreeitem.cpp \
    treemodel.cpp \
    roottreeitem.cpp \
    ingredientsdialog.cpp \
    ingredienttreeitem.cpp \
    treeutils.cpp \
    productdictionary.cpp \
    mealsdialog.cpp \
    mealingredientsdialog.cpp \
    tablemodel.cpp \
    sortednames.cpp \
    ingredientcompleterdelegate.cpp \
    checkablelistmodel.cpp

HEADERS += \
        mainwindow.h \
    ingredient.h \
    meal.h \
    abstractproduct.h \
    categorytreeitem.h \
    ingredienttreeitem.h \
    mealtreeitem.h \
    abstracttreeitem.h \
    treemodel.h \
    roottreeitem.h \
    ingredientsdialog.h \
    nlohmann/json.hpp \
    treeutils.h \
    productdictionary.h \
    mealsdialog.h \
    mealingredientsdialog.h \
    tablemodel.h \
    sortednames.h \
    ingredientcompleterdelegate.h \
    checkablelistmodel.h

FORMS += \
        mainwindow.ui \
    ingredientsdialog.ui \
    mealsdialog.ui \
    mealingredientsdialog.ui

RESOURCES += \
    resources.qrc

RC_FILE = myapp.rc
