/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the MIT License. See LICENSE for details.
 *
 */

#include <QApplication>
#include <QAbstractButton>
#include "styleeditor.h"

using namespace CreatorStyleEdit::Internal;

StyleEditor::StyleEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StyleEditor)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked,
            this, &StyleEditor::buttonClicked);

    // Raise this dialog for MAC OS X
    connect(ui->baseSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->textSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->brightTextSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->windowSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->windowTextSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->lightSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->midSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->darkSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->buttonSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->alternateBaseSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->buttonTextSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->midlightSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->shadowSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->highlightSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
    connect(ui->highlightedTextSelector, &ColorSelectorWidget::colorChanged,
            this, &StyleEditor::newColorSelected);
}

StyleEditor::~StyleEditor()
{
    delete ui;
}

void StyleEditor::setUiFromPalette(const QPalette &palette)
{
    ui->baseSelector->setColor(palette.color(QPalette::Base));
    ui->textSelector->setColor(palette.color(QPalette::Text));
    ui->brightTextSelector->setColor(palette.color(QPalette::BrightText));
    ui->windowSelector->setColor(palette.color(QPalette::Window));
    ui->windowTextSelector->setColor(palette.color(QPalette::WindowText));
    ui->lightSelector->setColor(palette.color(QPalette::Light));
    ui->midSelector->setColor(palette.color(QPalette::Mid));
    ui->darkSelector->setColor(palette.color(QPalette::Dark));
    ui->buttonSelector->setColor(palette.color(QPalette::Button));
    ui->alternateBaseSelector->setColor(palette.color(QPalette::AlternateBase));
    ui->buttonTextSelector->setColor(palette.color(QPalette::ButtonText));
    ui->midlightSelector->setColor(palette.color(QPalette::Midlight));
    ui->shadowSelector->setColor(palette.color(QPalette::Shadow));
    ui->highlightSelector->setColor(palette.color(QPalette::Highlight));
    ui->highlightedTextSelector->setColor(palette.color(QPalette::HighlightedText));
}

QPalette StyleEditor::paletteFromUi() const
{
    QPalette palette;
    palette.setColor(QPalette::Base, ui->baseSelector->color());
    palette.setColor(QPalette::Text, ui->textSelector->color());
    palette.setColor(QPalette::BrightText, ui->brightTextSelector->color());
    palette.setColor(QPalette::Window, ui->windowSelector->color());
    palette.setColor(QPalette::WindowText, ui->windowTextSelector->color());
    palette.setColor(QPalette::Light, ui->lightSelector->color());
    palette.setColor(QPalette::Mid, ui->midSelector->color());
    palette.setColor(QPalette::Dark, ui->darkSelector->color());
    palette.setColor(QPalette::Button, ui->buttonSelector->color());
    palette.setColor(QPalette::AlternateBase, ui->alternateBaseSelector->color());
    palette.setColor(QPalette::ButtonText, ui->buttonTextSelector->color());
    palette.setColor(QPalette::Midlight, ui->midlightSelector->color());
    palette.setColor(QPalette::Shadow, ui->shadowSelector->color());
    palette.setColor(QPalette::Highlight, ui->highlightSelector->color());
    palette.setColor(QPalette::HighlightedText, ui->highlightedTextSelector->color());

    return palette;
}

void StyleEditor::buttonClicked(QAbstractButton *button)
{
    QDialogButtonBox::ButtonRole buttonRole = ui->buttonBox->buttonRole(button);
    if (buttonRole == QDialogButtonBox::ApplyRole ||
            buttonRole == QDialogButtonBox::AcceptRole)
    {
        emit paletteChanged(paletteFromUi());
    }

    QDialogButtonBox::StandardButton standardButton = ui->buttonBox->standardButton(button);
    if (standardButton == QDialogButtonBox::RestoreDefaults)
    {
        setUiFromPalette(QApplication::palette());
    }
}

void StyleEditor::newColorSelected()
{
    raise();
}
