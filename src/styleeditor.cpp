/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the MIT License. See LICENSE for details.
 *
 */

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QAbstractButton>
#include <QListWidgetItem>

#include "defines.h"
#include "styleeditor.h"

using namespace CreatorStyleEdit::Internal;

StyleEditor::StyleEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StyleEditor),
    m_noStyleItem(0),
    m_customStyleItem(0)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked,
            this, &StyleEditor::buttonClicked);

    connect(ui->selectStyleSheetPushButton, &QPushButton::clicked,
            this, &StyleEditor::selectStyleSheet);

    // Raise this dialog for MAC OS X
    connect(this, &StyleEditor::stylesheetChanged,
            this, &StyleEditor::raise);
    connect(ui->styleSheetBaseSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::stylesheetChanged);

    m_noStyleItem = new QListWidgetItem(tr("No Style"));
    m_customStyleItem = new QListWidgetItem(tr("Custom Style"));

    ui->styleListWidget->addItem(m_noStyleItem);
    ui->styleListWidget->addItem(m_customStyleItem);

    connect(ui->styleListWidget, &QListWidget::currentItemChanged,
            this, &StyleEditor::currentStyleListItemChanged);

    ui->styleListWidget->setCurrentItem(m_noStyleItem);
}

StyleEditor::~StyleEditor()
{
    delete ui;
}

void StyleEditor::setStyleSheetPath(const QString &path)
{
    ui->stylesheetPathLineEdit->setText(path);
}

QString StyleEditor::styleSheetPathFromUi() const
{
    return ui->stylesheetPathLineEdit->text();
}

void StyleEditor::setStyleSheetBaseColor(const QColor &color)
{
    ui->styleSheetBaseSelector->setColor(color);
}

QColor StyleEditor::styleSheetBaseColorFromUi() const
{
    return ui->styleSheetBaseSelector->color();
}

void StyleEditor::buttonClicked(QAbstractButton *button)
{
    QDialogButtonBox::ButtonRole buttonRole = ui->buttonBox->buttonRole(button);
    if (buttonRole == QDialogButtonBox::ApplyRole ||
            buttonRole == QDialogButtonBox::AcceptRole)
    {
        QString styleFile = ui->stylesheetPathLineEdit->text();
        if (QFile::exists(styleFile)) {
            emit stylesheetChanged();
        }
    }
}

void StyleEditor::selectStyleSheet()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select Stylesheet"), QDir::homePath(),
                                                QStringLiteral("CSS (*.css)"));
    if (file.isEmpty()) {
        return;
    }

    ui->stylesheetPathLineEdit->setText(file);

    emit stylesheetChanged();
}

void StyleEditor::currentStyleListItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);

    ui->customStyleGroupBox->setEnabled(current == m_customStyleItem);
}
