#include "appnav.h"
#include "iconfont.h"
#include "quiwidget.h"

//可以自行更改这两个值切换导航按钮样式
bool AppNav::NavMainLeft = false;
bool AppNav::NavSubLeft = false;

void AppNav::setIconBtn(QAbstractButton *btn, const QChar &icon, bool iconFont, int size, int width, int height)
{
    QPixmap pix;
    if (iconFont) {
        pix = IconFont::Instance()->getPixmap(QUIConfig::TextColor, icon, size, width, height);
    } else {
        pix = IconHelper::Instance()->getPixmap(QUIConfig::TextColor, icon, size, width, height);
    }

    btn->setIconSize(QSize(width, height));
    btn->setIcon(QIcon(pix));
}

void AppNav::setIconBtn(QAbstractButton *btn, const QChar &icon, bool iconFont)
{
    //可以自行调整尺寸
    int size = 16;
    int width = 18;
    int height = 18;
    setIconBtn(btn, icon, iconFont, size, width, height);
}

void AppNav::setIconNavMain(QAbstractButton *btn, const QChar &icon, bool iconFont, int offset)
{
    //可以自行调整尺寸
    int size = 20;
    int width = 25;
    int height = 20;
    if (!AppNav::NavMainLeft) {
        size = 35;
        width = 40;
        height = 35;
    }

    //偏移值 可能部分图标要调整大小
    size += offset;
    setIconBtn(btn, icon, iconFont, size, width, height);
}

void AppNav::setIconNavSub(QAbstractButton *btn, const QChar &icon, bool iconFont, int offset)
{
    //可以自行调整尺寸
    int size = 20;
    int width = 25;
    int height = 20;
    if (!AppNav::NavSubLeft) {
        size = 35;
        width = 40;
        height = 35;
    }

    //偏移值 可能部分图标要调整大小
    size += offset;
    setIconBtn(btn, icon, iconFont, size, width, height);
}
