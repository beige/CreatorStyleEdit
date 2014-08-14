/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef APPLICATIONPROXYSTYLE_H
#define APPLICATIONPROXYSTYLE_H

#include <QProxyStyle>

class ApplicationProxyStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ApplicationProxyStyle(QStyle *style);

    void polish(QWidget *widget);
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;
};

#endif // APPLICATIONPROXYSTYLE_H
