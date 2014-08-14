/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QPainter>
#include <QStyleOption>

#include "applicationproxystyle.h"

ApplicationProxyStyle::ApplicationProxyStyle(QStyle *style) :
    QProxyStyle(style)
{
}

void ApplicationProxyStyle::polish(QWidget *widget)
{
    Q_UNUSED(widget);
}

void ApplicationProxyStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option,
                                          QPainter *painter, const QWidget *widget) const
{
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
