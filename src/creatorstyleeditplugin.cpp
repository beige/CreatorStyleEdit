/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the MIT License. See LICENSE for details.
 *
 */

#include "creatorstyleeditplugin.h"
#include "creatorstyleeditconstants.h"
#include "applicationproxystyle.h"
#include "styleeditor.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <utils/stylehelper.h>
#include <coreplugin/icore.h>
#include <coreplugin/navigationwidget.h>

#include <QSettings>
#include <QString>
#include <QAction>
#include <QMenu>
#include <QStatusBar>
#include <QApplication>
#include <QStyleFactory>

#include <QtPlugin>
#include <QDebug>
#include <QFile>

#include "defines.h"

namespace CreatorStyleEdit {
namespace Internal {

static const QString pluginNameSettingsKey(QStringLiteral("CreatorStyleEdit"));

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
    connect(m_styleEditor, &StyleEditor::stylesheetChanged,
            this, &CreatorStyleEditPlugin::stylesheetChanged);

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

bool CreatorStyleEditPlugin::delayedInitialize()
{
    QStyle *applicationStyle = qApp->style();

    ApplicationProxyStyle *style = new ApplicationProxyStyle(applicationStyle);
    qApp->setStyle(style);

    QPalette palette = paletteFromSettings();

    applyPaletteOnAllWidgets(palette);
    m_styleEditor->setUiFromPalette(palette);

    return true;
}

void CreatorStyleEditPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag CreatorStyleEditPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

void CreatorStyleEditPlugin::applyPaletteOnAllWidgets(const QPalette &palette)
{
    return;

    using namespace Core;
    using namespace Core::Internal;
    // Set Qt Creator base color. This color affects the frame around the main window
    Utils::StyleHelper::setBaseColor(palette.base().color());

    Core::NavigationWidget::instance()->setPalette(palette);

    // The locator widget in the status bar has always a black font
    // => Reset its palette to default
    Core::ICore::statusBar()->setPalette(QApplication::palette());

    // The palette on navigation widgest must be set, because the widgets are deleted and
    // newly created, if the NavigationSubWidget has to show another output
    const char *projectViewClass =                  "Utils::NavigationTreeView";
    const char *bookmarksViewClass =                "Bookmarks::Internal::BookmarkView";
    const char *classViewClass =                    "ClassView::Internal::NavigationWidget";
    const char *openEditorsViewClass =              "Core::Internal::OpenEditorsWidget";
    const char *cppTypeHierarchyViewClass =         "CppEditor::Internal::CppTypeHierarchyStackedWidget";
    const char *cppIncludeHierarchyViewClass =      "CppEditor::Internal::CppIncludeHierarchyStackedWidget";
    const char *folderNavigationWidgetViewClass =   "ProjectExplorer::Internal::FolderNavigationWidget";
    const char *outlineViewClass =                  "TextEditor::Internal::OutlineWidgetStack";
    const char *NavigationSubwidgetClass =          "Core::Internal::NavigationSubWidget";
    const char *StyledBarClass =                    "Utils::StyledBar";
    const char *NavComboBoxClass =                  "Core::Internal::NavComboBox";
    const char *ElidingLabelClass =                 "Utils::ElidingLabel";

    // The next two classes are used by the Type Hierarchy and Include Hierarchy as headers.
    // Setting palette on this class names takes no effect, because they don't have a Q_OBJECT
    // macro.
    const char *ClassLabelClass =                   "CppEditor::Internal::CppClassLabel";
    const char *IncludeLabelClass =                 "CppEditor::Internal::CppIncludeLabel";

    // QListViews are getting the palette because the Filesystem navigation widget uses this.
    // This will affect QListViews in other places as well (e.g. configuration dialog)
    const char *ListViewClass =                     "QListView";

    setPaletteOnClass(palette, ElidingLabelClass);
    setPaletteOnClass(palette, ListViewClass);
    setPaletteOnClass(palette, ClassLabelClass);
    setPaletteOnClass(palette, IncludeLabelClass);
    setPaletteOnClass(palette, projectViewClass);
    setPaletteOnClass(palette, bookmarksViewClass);
    setPaletteOnClass(palette, classViewClass);
    setPaletteOnClass(palette, openEditorsViewClass);
    setPaletteOnClass(palette, cppIncludeHierarchyViewClass);
    setPaletteOnClass(palette, cppTypeHierarchyViewClass);
    setPaletteOnClass(palette, folderNavigationWidgetViewClass);
    setPaletteOnClass(palette, outlineViewClass);
    setPaletteOnClass(palette, NavigationSubwidgetClass);
    setPaletteOnClass(palette, StyledBarClass);
    setPaletteOnClass(palette, NavComboBoxClass);

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

    // QApplication::setPalette doesn't work for relyable for output widgets. So the
    // palette must be set explicit on the widget
    QWidget *outputPaneManagerWidget = widgetForClass(QStringLiteral("Core::Internal::OutputPaneManager"));
    if (outputPaneManagerWidget) {
        outputPaneManagerWidget->setPalette(palette);
    }

    // The search widget's input widgets are mostly style dependent and only the text color
    // is affected by the palette. Because this can get really unreadable, reset the palette for
    // input widgets here.
    QWidget *searchWidget = widgetForClass(QStringLiteral("Core::Internal::FindToolWindow"));
    if (searchWidget) {
        resetPaletteOnInputChildWidgets(searchWidget);
    }
    QWidget *searchToolBar = widgetForClass(QStringLiteral("Core::Internal::FindToolBar"));
    if (searchToolBar) {
        resetPaletteOnInputChildWidgets(searchToolBar);
    }
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

/*!
 * \brief CreatorStyleEditPlugin::debugWidget Debug prints a widget's Class and its child
 *        widgets classes recursively.
 * \param widget The widget for debug printing
 */
void CreatorStyleEditPlugin::debugWidget(QWidget *widget)
{
    foreach (QObject *childObject, widget->children()) {
        if (!childObject->isWidgetType())
            continue;

        QWidget *widget = qobject_cast<QWidget *>(childObject);
        if (!widget)
            continue;

        QString className(QString::fromUtf8(widget->metaObject()->className()));
        qDebug() << "Widget class: " << className;

        debugWidget(widget);
    }
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

QWidget *CreatorStyleEditPlugin::widgetForClass(const QString &className)
{
    foreach (QWidget *topLevelWidget, QApplication::topLevelWidgets()) {
        QWidget *childWidgetWithClass = childWidgetForClass(topLevelWidget, className);
        if (childWidgetWithClass != 0)
            return childWidgetWithClass;
    }

    return 0;
}

/*!
 * \brief CreatorStyleEditPlugin::childWidgetForClass
 *        Get a pointer to a child widget that has a specific class name
 */
QWidget *CreatorStyleEditPlugin::childWidgetForClass(QWidget *widget, const QString &className)
{
    QString widgetClassName(QString::fromUtf8(widget->metaObject()->className()));
    if (widgetClassName.contains(className))
        return widget;

    foreach (QObject *childObject, widget->children()) {
        if (!childObject->isWidgetType())
            continue;

        QWidget *childWidget = qobject_cast<QWidget *>(childObject);
        if (!childWidget)
            continue;

        widgetClassName = QString::fromUtf8(childWidget->metaObject()->className());

        if (QWidget *subChildWidget = childWidgetForClass(childWidget, className)) {
            return subChildWidget;
        }
    }

    return 0;
}

/*!
 * \brief CreatorStyleEditPlugin::resetPaletteOnInputChildWidgets
 *        Reset the palette of some imput widgets like e.g. QComboBox to default
 *        QApplication palette. Some Styles doesn't work nicely with a custom palette.
 * \param widget The widget containing child widgets to reset.
 */
void CreatorStyleEditPlugin::resetPaletteOnInputChildWidgets(QWidget *widget)
{
    foreach (QObject *childObject, widget->children()) {
        if (!childObject->isWidgetType())
            continue;

        QWidget *childWidget = qobject_cast<QWidget *>(childObject);
        if (!childWidget)
            continue;

        if (childWidget->inherits("QComboBox") ||
                childWidget->inherits("QPushButton") ||
                childWidget->inherits("QLineEdit")) {
//            qDebug() << "Child palette will be reset: " << childWidget->metaObject()->className();
            childWidget->setPalette(QApplication::palette());
        }

        resetPaletteOnInputChildWidgets(childWidget);
    }
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

void CreatorStyleEditPlugin::stylesheetChanged(const QString &fileName)
{
    QFile styleFile(fileName);
    styleFile.open(QIODevice::ReadOnly);
    QString styleContent(QString::fromUtf8(styleFile.readAll().data()));

    Core::NavigationWidget::instance()->setStyleSheet(styleContent);

    // QApplication::setPalette doesn't work for relyable for output widgets. So the
    // palette must be set explicit on the widget
    QWidget *outputPaneManagerWidget = widgetForClass(QStringLiteral("Core::Internal::OutputPaneManager"));
    if (outputPaneManagerWidget) {
        outputPaneManagerWidget->setStyleSheet(styleContent);
    }

    QWidget *debuggerWindow = widgetForClass(QStringLiteral("Debugger::DebuggerMainWindow"));
    if (debuggerWindow) {
        qDebug() << "Found Debugger window";
        debuggerWindow->setStyleSheet(styleContent);
    }
}

Q_EXPORT_PLUGIN2(CreatorStyleEdit, CreatorStyleEditPlugin)

