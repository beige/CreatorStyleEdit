/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#include <QPalette>
#include <QColorDialog>
#include "colorselectorwidget.h"
#include "ui_colorselectorwidget.h"

ColorSelectorWidget::ColorSelectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorSelectorWidget)
{
    ui->setupUi(this);
}

ColorSelectorWidget::~ColorSelectorWidget()
{
    delete ui;
}

void ColorSelectorWidget::setColor(const QColor &color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(color);

    QPalette framePalette = ui->colorFrame->palette();
    framePalette.setBrush(ui->colorFrame->backgroundRole(), color);
    ui->colorFrame->setPalette(framePalette);
}

QColor ColorSelectorWidget::color() const
{
    return m_color;
}

void ColorSelectorWidget::selectColor()
{
    QColor color = QColorDialog::getColor(m_color);
    if (!color.isValid())
        return;

    setColor(color);
}
