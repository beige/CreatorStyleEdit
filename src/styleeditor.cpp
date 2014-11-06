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
#include <QFileInfo>
#include <QDebug>
#include <QAbstractButton>
#include <QListWidgetItem>

#include <coreplugin/icore.h>

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

    m_noStyleItem = new QListWidgetItem(tr("No Style"));
    m_customStyleItem = new QListWidgetItem(tr("Custom Style"));

    ui->styleListWidget->addItem(m_noStyleItem);
    ui->styleListWidget->addItem(m_customStyleItem);

    // Qt Creator styles dir
//    QString resourcePath = Core::ICore::resourcePath();
//    QDir styleDir(resourcePath + QLatin1String("/styles"));

    initStyleListView();

    connect(ui->styleListWidget, &QListWidget::currentItemChanged,
            this, &StyleEditor::currentStyleListItemChanged);

    ui->styleListWidget->setCurrentItem(m_noStyleItem);
}

void StyleEditor::initStyleListView()
{
    QDir stylesBaseDir(QStringLiteral(":/CreatorStyleEdit/styles/"));
    foreach (const QFileInfo styleInfo, stylesBaseDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString styleName = styleInfo.fileName();

        QString colorSchemePath;
        QString stylesheetPath;
        QString readmePath;
        QDir styleDir = styleInfo.absoluteFilePath();
        foreach (const QFileInfo styleFile, styleDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
            if (styleFile.fileName().endsWith(QStringLiteral(".xml"))) {
                colorSchemePath = styleFile.absoluteFilePath();
            }
            if (styleFile.fileName().endsWith(QStringLiteral(".css"))) {
                stylesheetPath = styleFile.absoluteFilePath();
            }
            if (styleFile.fileName().toLower().contains(QStringLiteral("readme"))) {
                readmePath = styleFile.absoluteFilePath();
            }
        }

        if (colorSchemePath.isEmpty()) {
            qWarning() << "Style " << styleName << " has no colorscheme file";
            continue;
        }
        if (stylesheetPath.isEmpty()) {
            qWarning() << "Style " << styleName << " has no stylesheet file";
            continue;
        }
        if (readmePath.isEmpty()) {
            qWarning() << "Style " << styleName << " has no readme file";
            continue;
        }

        // Export color scheme file to Qt Creator install dir
        QString resourcePath = Core::ICore::resourcePath();
        QDir colorSchemesDir(resourcePath + QLatin1String("/styles"));
        QString colorSchemeFileName = QFileInfo(colorSchemePath).fileName();
        QFile sourceSchemeFile(colorSchemePath);
        QFile destSchemeFile(colorSchemesDir.absoluteFilePath(colorSchemeFileName));
        // Doesn't use QFile::copy, because destination file will be read only
        sourceSchemeFile.open(QIODevice::ReadOnly);
        destSchemeFile.open(QIODevice::WriteOnly);
        destSchemeFile.write(sourceSchemeFile.readAll());
        sourceSchemeFile.close();
        destSchemeFile.close();

        QListWidgetItem *styleItem = new QListWidgetItem(styleName);
        styleItem->setData(CssFile, stylesheetPath);
        styleItem->setData(ColorSchemeFile, colorSchemePath);
        QFile readmeFile(readmePath);
        readmeFile.open(QIODevice::ReadOnly);
        styleItem->setData(ReadmeFile, QString::fromUtf8(readmeFile.readAll()));
        readmeFile.close();

        ui->styleListWidget->addItem(styleItem);
    }
}

StyleEditor::~StyleEditor()
{
    delete ui;
}

void StyleEditor::setCustomStyleSheetPath(const QString &path)
{
    ui->stylesheetPathLineEdit->setText(path);
}

void StyleEditor::setSelectedStyle(const QString &styleName)
{
    if (styleName.isEmpty()) {
        ui->styleListWidget->setCurrentItem(m_noStyleItem);
        emit styleNameChanged(m_noStyleItem->text());
        return;
    }

    QList<QListWidgetItem*> items = ui->styleListWidget->findItems(styleName, Qt::MatchExactly);
    if (items.count()) {
        QListWidgetItem *item = items.first();
        ui->styleListWidget->setCurrentItem(item);
        emit styleNameChanged(item->text());
    }
}

QString StyleEditor::selectedStyle() const
{
    QListWidgetItem *current = ui->styleListWidget->currentItem();
    if (current == 0) {
        return QString();
    }

    return current->text();
}

QString StyleEditor::styleSheetPath() const
{
    if (ui->styleListWidget->currentItem() == m_customStyleItem) {
        return customStyleSheetPath();
    }

    return m_currentStyleSheetPath;
}

QString StyleEditor::customStyleSheetPath() const
{
    return ui->stylesheetPathLineEdit->text();
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
    if (current == m_noStyleItem) {
        ui->styleDescriptionTextEdit->setText(tr("<h3>No style sheet</h3>"
                                                 "<p>No style sheet will be set for Qt Creator. "
                                                 "Restart required.</p>"));
        m_currentStyleSheetPath.clear();
    } else if (current == m_customStyleItem) {
        ui->styleDescriptionTextEdit->setText(tr("<h3>Custon style sheet</h3>"
                                                 "<p>A path to a custom stylesheet can be set.</p>"));
        m_currentStyleSheetPath = ui->stylesheetPathLineEdit->text();
    } else {
        // Custom style
        ui->styleDescriptionTextEdit->setText(current->data(ReadmeFile).toString());
        m_currentStyleSheetPath = current->data(CssFile).toString();
    }

    emit stylesheetChanged();
    emit styleNameChanged(current->text());
}
