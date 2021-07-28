#ifndef APPNAV_H
#define APPNAV_H

#include <QAbstractButton>

class AppNav
{
public:
    //主导航按钮图标左侧
    static bool NavMainLeft;
    //子导航按钮图标左侧
    static bool NavSubLeft;

    //设置图形字体图标
    static void setIconBtn(QAbstractButton *btn, const QChar &icon, bool iconFont, int size, int width, int height);
    static void setIconBtn(QAbstractButton *btn, const QChar &icon, bool iconFont = false);
    static void setIconNavMain(QAbstractButton *btn, const QChar &icon, bool iconFont = true, int offset = 0);
    static void setIconNavSub(QAbstractButton *btn, const QChar &icon, bool iconFont = true, int offset = 0);
};

#endif // APPNAV_H
