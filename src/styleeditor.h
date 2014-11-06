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

    void setCustomStyleSheetPath(const QString &path);
    void setSelectedStyle(const QString &styleName);
    QString selectedStyle() const;
    QString styleSheetPath() const;
    QString customStyleSheetPath() const;

    void initStyleListView();
signals:
    void stylesheetChanged();
    void styleNameChanged(const QString &styleName);

private slots:
    void buttonClicked(QAbstractButton *button);
    void selectStyleSheet();
    void currentStyleListItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    enum StyleFileType {
        CssFile = Qt::UserRole + 1,
        ColorSchemeFile = Qt::UserRole + 2,
        ReadmeFile = Qt::UserRole + 3
    };

    Ui::StyleEditor *ui;
    QListWidgetItem *m_noStyleItem;
    QListWidgetItem *m_customStyleItem;
    QString m_currentStyleSheetPath;
};

} // namespace Internal
} // namespace CreatorStyleEdit
#endif // STYLEEDITOR_H
