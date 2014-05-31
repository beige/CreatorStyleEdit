/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the MIT License. See LICENSE for details.
 *
 */

#ifndef COLORSELECTORWIDGET_H
#define COLORSELECTORWIDGET_H

#include <QColor>
#include <QWidget>

namespace Ui {
class ColorSelectorWidget;
}

class ColorSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorSelectorWidget(QWidget *parent = 0);
    ~ColorSelectorWidget();

    void setColor(const QColor &color);
    QColor color() const;

signals:
    void colorChanged(const QColor &color);

private slots:
    void selectColor();

private:
    Ui::ColorSelectorWidget *ui;
    QColor m_color;
};

#endif // COLORSELECTORWIDGET_H
