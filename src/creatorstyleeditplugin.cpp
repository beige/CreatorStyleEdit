/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the MIT License. See LICENSE for details.
 *
 */

#include "creatorstyleeditplugin.h"
#include "creatorstyleeditconstants.h"
#include "styleeditor.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <QSettings>
#include <QString>
#include <QAction>
#include <QMenu>
#include <QApplication>

#include <QtPlugin>

namespace CreatorStyleEdit {
namespace Internal {

static const QString pluginNameSettingsKey(QStringLiteral("CreatorStyleEdit"));

static const QString baseColorSettingKey(QStringLiteral("base"));
static const QString textColorSettingKey(QStringLiteral("text"));
static const QString brightTextColorSettingKey(QStringLiteral("brightText"));
static const QString windowColorSettingKey(QStringLiteral("window"));
static const QString windowTextColorSettingKey(QStringLiteral("windowText"));
static const QString lightColorSettingKey(QStringLiteral("light"));
static const QString midColorSettingKey(QStringLiteral("mid"));
static const QString darkColorSettingKey(QStringLiteral("dark"));
static const QString buttonColorSettingKey(QStringLiteral("button"));
static const QString alternateBaseColorSettingKey(QStringLiteral("alternate base"));
static const QString buttonTextColorSettingKey(QStringLiteral("button text"));
static const QString midlightColorSettingKey(QStringLiteral("midlight"));
static const QString shadowColorSettingKey(QStringLiteral("shadow"));
static const QString highlightColorSettingKey(QStringLiteral("highlighted"));
static const QString highlightedTextColorSettingKey(QStringLiteral("highlighted text"));

} // namespace Internal
} // namespace CreatorStyleEdit

using namespace CreatorStyleEdit::Internal;

CreatorStyleEditPlugin::CreatorStyleEditPlugin()
    : m_styleEditor(0)
{
}

CreatorStyleEditPlugin::~CreatorStyleEditPlugin()
{
}

bool CreatorStyleEditPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    m_styleEditor = new StyleEditor;
    connect(m_styleEditor, &StyleEditor::paletteChanged,
            this, &CreatorStyleEditPlugin::paletteChanged);

    QAction *action = new QAction(tr("Edit Style"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("Style Editor"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    return true;
}

void CreatorStyleEditPlugin::extensionsInitialized()
{
    QPalette palette = paletteFromSettings();
    applyPaletteOnAllWidgets(palette);
    m_styleEditor->setUiFromPalette(palette);
}

ExtensionSystem::IPlugin::ShutdownFlag CreatorStyleEditPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

void CreatorStyleEditPlugin::applyPaletteOnAllWidgets(const QPalette &palette)
{
    const char *projectViewClass =                  "Utils::NavigationTreeView";
    const char *bookmarksViewClass =                "Bookmarks::Internal::BookmarkView";
    const char *classViewClass =                    "ClassView::Internal::NavigationWidget";
    const char *openEditorsViewClass =              "Core::Internal::OpenEditorsWidget";
    const char *cppTypeHierarchyViewClass =         "CppEditor::Internal::CppTypeHierarchyWidget";
    const char *cppIncludeHierarchyViewClass =      "CppEditor::Internal::CppIncludeHierarchyStackedWidget";
    const char *folderNavigationWidgetViewClass =   "ProjectExplorer::Internal::FolderNavigationWidget";
    const char *outlineViewClass =                  "TextEditor::Internal::OutlineWidgetStack";

    setPaletteOnClass(palette, projectViewClass);
    setPaletteOnClass(palette, bookmarksViewClass);
    setPaletteOnClass(palette, classViewClass);
    setPaletteOnClass(palette, openEditorsViewClass);
    setPaletteOnClass(palette, cppIncludeHierarchyViewClass);
    setPaletteOnClass(palette, cppTypeHierarchyViewClass);
    setPaletteOnClass(palette, folderNavigationWidgetViewClass);
    setPaletteOnClass(palette, outlineViewClass);

    // Output panes
    const char *searchResulScrollAreaClass =    "Core::Internal::InternalScrollArea";
    const char *searchResultWidgetClass =       "Core::Internal::SearchResultWidget";
    const char *messageOutputWindowClass =      "Core::OutputWindow";
    const char *applicationOutputWindowClass =  "ProjectExplorer::Internal::TabWidget";
    const char *compileOutputWindowClass =      "ProjectExplorer::Internal::CompileOutputTextEdit";
    const char *taskOutputWindowClass =         "ProjectExplorer::Internal::TaskView";
    const char *qmlConsoleOutputWindowClass =   "QmlJSTools::Internal::QmlConsoleView";
    const char *qmlConsoleWindowClass =         "QmlJSTools::Internal::QWidget";
    const char *todoWindowClass =               "Todo::Internal::TodoOutputTreeView";
    const char *versionControlWindowClass =     "VcsBase::Internal::OutputWindowPlainTextEdit";

    // Output panes
    setPaletteOnClass(palette, searchResulScrollAreaClass);
    setPaletteOnClass(palette, searchResultWidgetClass);
    setPaletteOnClass(palette, messageOutputWindowClass);
    setPaletteOnClass(palette, applicationOutputWindowClass);
    setPaletteOnClass(palette, compileOutputWindowClass);
    setPaletteOnClass(palette, taskOutputWindowClass);
    setPaletteOnClass(palette, qmlConsoleOutputWindowClass);
    setPaletteOnClass(palette, qmlConsoleWindowClass);
    setPaletteOnClass(palette, todoWindowClass);
    setPaletteOnClass(palette, versionControlWindowClass);

    // Debugging outputs
    const char *baseTreeView =          "Debugger::Internal::BaseTreeView";
    const char *breakTreeView =         "Debugger::Internal::BreakTreeView";
    const char *modulesTreeView =       "Debugger::Internal::ModulesTreeView";
    const char *registerTreeView =      "Debugger::Internal::RegisterTreeView";
    const char *snapshotTreeView =      "Debugger::Internal::SnapshotTreeView";
    const char *sourceFilesTreeView =   "Debugger::Internal::SourceFilesTreeView";
    const char *stackTreeView =         "Debugger::Internal::StackTreeView";
    const char *threadsTreeView =       "Debugger::Internal::ThreadsTreeView";
    const char *watchTreeView =         "Debugger::Internal::WatchTreeView";
    const char *valgrindTreeView =      "Valgrind::Internal::CostView";

    setPaletteOnClass(palette, baseTreeView);
    setPaletteOnClass(palette, breakTreeView);
    setPaletteOnClass(palette, modulesTreeView);
    setPaletteOnClass(palette, registerTreeView);
    setPaletteOnClass(palette, snapshotTreeView);
    setPaletteOnClass(palette, sourceFilesTreeView);
    setPaletteOnClass(palette, stackTreeView);
    setPaletteOnClass(palette, threadsTreeView);
    setPaletteOnClass(palette, watchTreeView);
    setPaletteOnClass(palette, valgrindTreeView);
}

QColor CreatorStyleEditPlugin::colorFromSettings(QSettings *settings, QPalette::ColorRole colorRole) const
{
    QPalette defaultPalette = QApplication::palette();
    QVariant defaultBaseColor = defaultPalette.color(colorRole).name();
    QString colorString = settings->value(settingsKeyForColorRole(colorRole),
                                          defaultBaseColor).toString();
    QColor baseColor(colorString);

    return baseColor;
}

QPalette CreatorStyleEditPlugin::paletteFromSettings() const
{
    QSettings *settings = Core::ICore::settings();
    QPalette palette;

    palette.setColor(QPalette::Base, colorFromSettings(settings, QPalette::Base));
    palette.setColor(QPalette::Text, colorFromSettings(settings, QPalette::Text));
    palette.setColor(QPalette::BrightText, colorFromSettings(settings, QPalette::BrightText));
    palette.setColor(QPalette::Window, colorFromSettings(settings, QPalette::Window));
    palette.setColor(QPalette::WindowText, colorFromSettings(settings, QPalette::WindowText));
    palette.setColor(QPalette::Light, colorFromSettings(settings, QPalette::Light));
    palette.setColor(QPalette::Mid, colorFromSettings(settings, QPalette::Mid));
    palette.setColor(QPalette::Dark, colorFromSettings(settings, QPalette::Dark));
    palette.setColor(QPalette::Button, colorFromSettings(settings, QPalette::Button));
    palette.setColor(QPalette::AlternateBase, colorFromSettings(settings, QPalette::AlternateBase));
    palette.setColor(QPalette::ButtonText, colorFromSettings(settings, QPalette::ButtonText));
    palette.setColor(QPalette::Midlight, colorFromSettings(settings, QPalette::Midlight));
    palette.setColor(QPalette::Shadow, colorFromSettings(settings, QPalette::Shadow));
    palette.setColor(QPalette::Highlight, colorFromSettings(settings, QPalette::Highlight));
    palette.setColor(QPalette::HighlightedText, colorFromSettings(settings, QPalette::HighlightedText));

    return palette;
}

void CreatorStyleEditPlugin::writePaletteToSettings(const QPalette &palette)
{
    QSettings *settings = Core::ICore::settings();

    settings->setValue(settingsKeyForColorRole(QPalette::Base),
                       palette.color(QPalette::Base).name());
    settings->setValue(settingsKeyForColorRole(QPalette::Text),
                       palette.color(QPalette::Text).name());
    settings->setValue(settingsKeyForColorRole(QPalette::BrightText),
                       palette.color(QPalette::BrightText).name());
    settings->setValue(settingsKeyForColorRole(QPalette::Window),
                       palette.color(QPalette::Window).name());
    settings->setValue(settingsKeyForColorRole(QPalette::WindowText),
                       palette.color(QPalette::WindowText).name());
    settings->setValue(settingsKeyForColorRole(QPalette::Light),
                       palette.color(QPalette::Light).name());
    settings->setValue(settingsKeyForColorRole(QPalette::Mid),
                       palette.color(QPalette::Mid).name());
    settings->setValue(settingsKeyForColorRole(QPalette::Dark),
                       palette.color(QPalette::Dark).name());
    settings->setValue(settingsKeyForColorRole(QPalette::Button),
                       palette.color(QPalette::Button).name());
    settings->setValue(settingsKeyForColorRole(QPalette::AlternateBase),
                       palette.color(QPalette::AlternateBase).name());
    settings->setValue(settingsKeyForColorRole(QPalette::ButtonText),
                       palette.color(QPalette::ButtonText).name());
    settings->setValue(settingsKeyForColorRole(QPalette::Midlight),
                       palette.color(QPalette::Midlight).name());
    settings->setValue(settingsKeyForColorRole(QPalette::Shadow),
                       palette.color(QPalette::Shadow).name());
    settings->setValue(settingsKeyForColorRole(QPalette::Highlight),
                       palette.color(QPalette::Highlight).name());
    settings->setValue(settingsKeyForColorRole(QPalette::HighlightedText),
                       palette.color(QPalette::HighlightedText).name());
}

QString CreatorStyleEditPlugin::settingsKeyForColorRole(QPalette::ColorRole role) const
{
    switch (role) {
    case QPalette::Base: {
        return settingsKey(baseColorSettingKey);
    }
    case QPalette::Text: {
        return settingsKey(textColorSettingKey);
    }
    case QPalette::BrightText: {
        return settingsKey(brightTextColorSettingKey);
    }
    case QPalette::Window: {
        return settingsKey(windowColorSettingKey);
    }
    case QPalette::WindowText: {
        return settingsKey(windowTextColorSettingKey);
    }
    case QPalette::Light: {
        return settingsKey(lightColorSettingKey);
    }
    case QPalette::Mid: {
        return settingsKey(midColorSettingKey);
    }
    case QPalette::Dark: {
        return settingsKey(darkColorSettingKey);
    }
    case QPalette::Button: {
        return settingsKey(buttonColorSettingKey);
    }
    case QPalette::AlternateBase: {
        return settingsKey(alternateBaseColorSettingKey);
    }
    case QPalette::ButtonText: {
        return settingsKey(buttonTextColorSettingKey);
    }
    case QPalette::Midlight: {
        return settingsKey(midlightColorSettingKey);
    }
    case QPalette::Shadow: {
        return settingsKey(shadowColorSettingKey);
    }
    case QPalette::Highlight: {
        return settingsKey(highlightColorSettingKey);
    }
    case QPalette::HighlightedText: {
        return settingsKey(highlightedTextColorSettingKey);
    }
    default:
        return settingsKey(baseColorSettingKey);
    }
}

QString CreatorStyleEditPlugin::settingsKey(const QString &key) const
{
    return QString(QStringLiteral("%1/%2"))
            .arg(pluginNameSettingsKey)
            .arg(key);
}

void CreatorStyleEditPlugin::triggerAction()
{
    m_styleEditor->show();
    m_styleEditor->raise();
}

void CreatorStyleEditPlugin::setPaletteOnClass(const QPalette &palette, const char *className)
{
    QApplication::setPalette(palette, className);
}

void CreatorStyleEditPlugin::paletteChanged(const QPalette &palette)
{
    applyPaletteOnAllWidgets(palette);
    writePaletteToSettings(palette);
}

Q_EXPORT_PLUGIN2(CreatorStyleEdit, CreatorStyleEditPlugin)

