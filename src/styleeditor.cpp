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
#include <QJsonDocument>
#include <QJsonObject>
#include <QAbstractButton>

#include "defines.h"
#include "styleeditor.h"

using namespace CreatorStyleEdit::Internal;

StyleEditor::StyleEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StyleEditor)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked,
            this, &StyleEditor::buttonClicked);
    connect(ui->exportPushButton, &QPushButton::clicked,
            this, &StyleEditor::exportStyle);
    connect(ui->importPushButton, &QPushButton::clicked,
            this, &StyleEditor::importStyle);

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

void StyleEditor::exportStyle()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export style"), QDir::homePath(),
                                                    QStringLiteral("Json (*.json)"));

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(QStringLiteral(".json"))) {
        fileName.append(QStringLiteral(".json"));
    }

    QJsonDocument jsonDoc(paletteToJson(paletteFromUi()));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(jsonDoc.toJson());
    file.close();
}

void StyleEditor::importStyle()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import style"), QDir::homePath(),
                                                    QStringLiteral("Json (*.json)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject jsonStyle = jsonDoc.object();
    if (jsonStyle.isEmpty())
        return;

    QPalette palette = paletteFromJson(jsonStyle);
    setUiFromPalette(palette);

    emit paletteChanged(palette);
}

QJsonObject StyleEditor::paletteToJson(const QPalette &palette)
{
    QJsonObject json;
    json.insert(baseColorSettingKey, palette.base().color().name());
    json.insert(textColorSettingKey, palette.text().color().name());
    json.insert(brightTextColorSettingKey, palette.brightText().color().name());
    json.insert(windowColorSettingKey, palette.window().color().name());
    json.insert(windowTextColorSettingKey, palette.windowText().color().name());
    json.insert(lightColorSettingKey, palette.light().color().name());
    json.insert(midColorSettingKey, palette.mid().color().name());
    json.insert(darkColorSettingKey, palette.dark().color().name());
    json.insert(buttonColorSettingKey, palette.button().color().name());
    json.insert(alternateBaseColorSettingKey, palette.alternateBase().color().name());
    json.insert(buttonTextColorSettingKey, palette.buttonText().color().name());
    json.insert(midlightColorSettingKey, palette.midlight().color().name());
    json.insert(shadowColorSettingKey, palette.shadow().color().name());
    json.insert(highlightColorSettingKey, palette.highlight().color().name());
    json.insert(highlightedTextColorSettingKey, palette.highlightedText().color().name());
    return json;
}

QPalette StyleEditor::paletteFromJson(const QJsonObject &json)
{
    QPalette palette;
    palette.setColor(QPalette::Base, QColor(json.value(baseColorSettingKey).toString()));
    palette.setColor(QPalette::Text, QColor(json.value(textColorSettingKey).toString()));
    palette.setColor(QPalette::BrightText, QColor(json.value(brightTextColorSettingKey).toString()));
    palette.setColor(QPalette::Window, QColor(json.value(windowColorSettingKey).toString()));
    palette.setColor(QPalette::WindowText, QColor(json.value(windowTextColorSettingKey).toString()));
    palette.setColor(QPalette::Light, QColor(json.value(lightColorSettingKey).toString()));
    palette.setColor(QPalette::Mid, QColor(json.value(midColorSettingKey).toString()));
    palette.setColor(QPalette::Dark, QColor(json.value(darkColorSettingKey).toString()));
    palette.setColor(QPalette::Button, QColor(json.value(buttonColorSettingKey).toString()));
    palette.setColor(QPalette::AlternateBase, QColor(json.value(alternateBaseColorSettingKey).toString()));
    palette.setColor(QPalette::ButtonText, QColor(json.value(buttonTextColorSettingKey).toString()));
    palette.setColor(QPalette::Midlight, QColor(json.value(midlightColorSettingKey).toString()));
    palette.setColor(QPalette::Shadow, QColor(json.value(shadowColorSettingKey).toString()));
    palette.setColor(QPalette::Highlight, QColor(json.value(highlightColorSettingKey).toString()));
    palette.setColor(QPalette::HighlightedText, QColor(json.value(highlightedTextColorSettingKey).toString()));
    return palette;
}
