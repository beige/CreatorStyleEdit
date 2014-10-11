/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the MIT License. See LICENSE for details.
 *
 */

#ifndef STYLEEDITOR_H
#define STYLEEDITOR_H

#include <QDialog>
#include "ui_styleeditor.h"

class QAbstractButton;

namespace CreatorStyleEdit {
namespace Internal {

class StyleEditor : public QDialog
{
    Q_OBJECT

public:
    explicit StyleEditor(QWidget *parent = 0);
    ~StyleEditor();

    void setUiFromPalette(const QPalette &palette);
    QPalette paletteFromUi() const;

signals:
    void paletteChanged(const QPalette &palette);
    void stylesheetChanged(const QString &stylesheetFile);

private slots:
    void buttonClicked(QAbstractButton *button);
    void newColorSelected();
    void exportStyle();
    void importStyle();
    void selectStyleSheet();

private:
    QJsonObject paletteToJson(const QPalette &palette);
    QPalette paletteFromJson(const QJsonObject &json);
    Ui::StyleEditor *ui;
};

} // namespace Internal
} // namespace CreatorStyleEdit
#endif // STYLEEDITOR_H
