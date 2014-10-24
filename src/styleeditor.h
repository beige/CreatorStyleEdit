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
class QListWidgetItem;

namespace CreatorStyleEdit {
namespace Internal {

class StyleEditor : public QDialog
{
    Q_OBJECT

public:
    explicit StyleEditor(QWidget *parent = 0);
    ~StyleEditor();

    void setStyleSheetPath(const QString &path);
    QString styleSheetPathFromUi() const;

    void setStyleSheetBaseColor(const QColor &color);
    QColor styleSheetBaseColorFromUi() const;

signals:
    void stylesheetChanged();

private slots:
    void buttonClicked(QAbstractButton *button);
    void selectStyleSheet();
    void currentStyleListItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::StyleEditor *ui;
    QListWidgetItem *m_noStyleItem;
    QListWidgetItem *m_customStyleItem;
};

} // namespace Internal
} // namespace CreatorStyleEdit
#endif // STYLEEDITOR_H
