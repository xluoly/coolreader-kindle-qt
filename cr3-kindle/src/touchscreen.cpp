#include "touchscreen.h"

const Qt::Key TouchScreen::TAP_ACTIONS[][7] = {
    { // single tap
      Qt::Key_unknown, Qt::Key_unknown, Qt::Key_unknown,
      Qt::Key_PageUp, Qt::Key_PageDown, Qt::Key_PageDown,
      Qt::Key_PageDown
    },
    { // two finger tap
      Qt::Key_Escape, Qt::Key_Home, Qt::Key_Select,
      Qt::Key_Up, Qt::Key_O, Qt::Key_Down,
      Qt::Key_Menu
    },
    { // two finger tap while reading
      Qt::Key_Escape, Qt::Key_Home, Qt::Key_Select,
      Qt::Key_Up, Qt::Key_O, Qt::Key_Down,
      Qt::Key_Menu
    },
    { // long tap
      Qt::Key_Select, Qt::Key_Select, Qt::Key_Select,
      Qt::Key_Select, Qt::Key_Select, Qt::Key_Select,
      Qt::Key_Select
    },
    { // long tap while reading
      Qt::Key_Minus, Qt::Key_Select, Qt::Key_Plus,
      Qt::Key_unknown, Qt::Key_O, Qt::Key_unknown,
      Qt::Key_Menu
    }
};

const Qt::Key TouchScreen::SWIPE_ACTIONS[][4] = { // UP / RIGHT / DOWN / LEFT
    { // swipe while not in reader
      Qt::Key_PageDown, Qt::Key_Home, Qt::Key_PageUp, Qt::Key_Escape
    },
    { // swipe when in reader
      Qt::Key_PageDown, Qt::Key_PageUp, Qt::Key_PageUp, Qt::Key_PageDown
    }
};

TouchScreen::TouchScreen()
{
    topMargin = 30; // margins in %
    bottomMargin = 30;
    rightMargin = 30;
    leftMargin = 30;

    w = Device::getWidth();
    h = Device::getHeight();

    lpx = w * leftMargin / 100;
    rpx = w * rightMargin / 100;
    tpx = h * topMargin / 100;
    bpx = h * bottomMargin / 100;
}

TouchScreen::Area TouchScreen::getPointArea(int x, int y)
{
    if (y < tpx) {
        if (x < lpx) return TOP_LEFT;
        else if (x >= lpx && x < w - rpx) return TOP_MIDDLE;
        else if (x >= w - rpx) return TOP_RIGHT;
    } else if (y >= tpx) {
        if (x < lpx) return LEFT;
        else if (x >= w - rpx) return RIGHT;
        else if (x >= lpx && x < w - rpx) {
            if (y >= h - bpx) return BOTTOM_MIDDLE;
            else return CENTER;
        }
    }
    qDebug("UNCOVERED AREA: %d, %d", x, y);
    return CENTER;
}

Qt::Key TouchScreen::getAreaAction(int x, int y, TouchScreen::TouchType t)
{
    return TAP_ACTIONS[t][getPointArea(x, y)];
}

Qt::Key TouchScreen::getSwipeAction(int x, int y, int oldX, int oldY, TouchScreen::SwipeType t)
{
    if (!isGesture(x, y, oldX, oldY)) {
        return Qt::Key_unknown; // too short swipe
    }

    SwipeGesture g = SWIPE_UNRECOGNIZED;

    bool isHorizontal = abs(x - oldX) > abs(y - oldY);

    if (isHorizontal) {
        if (oldX > x) {
            g = SWIPE_LEFT;
        } else {
            g = SWIPE_RIGHT;
        }
    } else { // vertical
        if (y > oldY) {
            g = SWIPE_DOWN;
        } else {
            g = SWIPE_UP;
        }
    }

    return g == SWIPE_UNRECOGNIZED ? Qt::Key_unknown : SWIPE_ACTIONS[t][g];
}

bool TouchScreen::isGesture(int x, int y, int oldX, int oldY)
{
    return (abs(y - oldY) > MIN_SWIPE_PIXELS || abs(x - oldX) > MIN_SWIPE_PIXELS);
}