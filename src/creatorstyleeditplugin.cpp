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

#include <utils/stylehelper.h>
#include <coreplugin/icore.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/navigationwidget.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/imode.h>
#include <debugger/debuggerconstants.h>

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
    m_styleEditor->setSelectedStyle(selectedStyleFromSettings());
    m_styleEditor->setCustomStyleSheetPath(customStyleSheetPathFromSettings());
    connect(m_styleEditor, &StyleEditor::stylesheetChanged,
            this, &CreatorStyleEditPlugin::stylesheetChanged);
    connect(m_styleEditor, &StyleEditor::styleNameChanged,
            this, &CreatorStyleEditPlugin::styleNameChanged);

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

    connect(Core::ModeManager::instance(), SIGNAL(currentModeChanged(Core::IMode*)),
            this, SLOT(modeChanged(Core::IMode*)));

    // Give the main stacked widget of the OutputPaneManager a object name for better styling through
    // stylesheet.
    // This code assumes that the OutputPaneManager widget has only one direct child QStackedWidget,
    // which is the desired output widget. This can also be accomplished by a patch of Qt Creator
    // which sets the object name directly.
    QWidget *outputPaneManagerWidget = widgetForClass(QStringLiteral("Core::Internal::OutputPaneManager"));
    if (outputPaneManagerWidget) {
        foreach (QObject *childObject, outputPaneManagerWidget->children()) {
            if (!childObject->isWidgetType())
                continue;

            QWidget *childWidget = qobject_cast<QWidget *>(childObject);
            if (!childWidget)
                continue;

            QString widgetClassName = QString::fromUtf8(childWidget->metaObject()->className());

            if (widgetClassName  == QStringLiteral("QStackedWidget")) {
                childWidget->setObjectName(QStringLiteral("OutputPaneManagerMainWidget"));
            }
        }
    }

    stylesheetChanged();

    return true;
}

void CreatorStyleEditPlugin::extensionsInitialized()
{
}

ExtensionSystem::IPlugin::ShutdownFlag CreatorStyleEditPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

void CreatorStyleEditPlugin::setStylesheetOnChildWidgetsWithClass(QWidget *widget,
                                                                  const QString &stylesheet,
                                                                  const QString &className)
{
    foreach (QObject *childObject, widget->children()) {
        if (!childObject->isWidgetType())
            continue;

        QWidget *childWidget = qobject_cast<QWidget *>(childObject);
        if (!childWidget)
            continue;

        if (childWidget->inherits(className.toUtf8())) {
            childWidget->setStyleSheet(stylesheet);
        }

        setStylesheetOnChildWidgetsWithClass(childWidget, stylesheet, className);
    }
}

QString CreatorStyleEditPlugin::customStyleSheetPathFromSettings() const
{
    QSettings *settings = Core::ICore::settings();
    return settings->value(settingsKey(styleSheetPathSettingsKey)).toString();
}

QString CreatorStyleEditPlugin::selectedStyleFromSettings() const
{
    QSettings *settings = Core::ICore::settings();
    return settings->value(settingsKey(selectedStyleSettingsKey)).toString();
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

void CreatorStyleEditPlugin::stylesheetChanged()
{
    writeStyleSheetToSettings();

    applyStylesheet();
}

void CreatorStyleEditPlugin::styleNameChanged(const QString &)
{
    writeStyleSheetToSettings();

    applyStylesheet();
}

void CreatorStyleEditPlugin::writeStyleSheetToSettings()
{
    QSettings *settings = Core::ICore::settings();

    settings->setValue(settingsKey(styleSheetPathSettingsKey),
                       m_styleEditor->customStyleSheetPath());
    settings->setValue(settingsKey(selectedStyleSettingsKey),
                       m_styleEditor->selectedStyle());
}

void CreatorStyleEditPlugin::applyStylesheet()
{
    QString styleSheetPath = m_styleEditor->styleSheetPath();
    if (styleSheetPath.isEmpty())
        return;

    QFile styleFile(styleSheetPath);
    styleFile.open(QIODevice::ReadOnly);
    QString styleContent(QString::fromUtf8(styleFile.readAll().data()));
    styleFile.close();

    Core::NavigationWidget::instance()->setStyleSheet(styleContent);

    // QApplication::setPalette doesn't work for relyable for output widgets. So the
    // palette must be set explicit on the widget
    QWidget *outputPaneManagerWidget = widgetForClass(QStringLiteral("Core::Internal::OutputPaneManager"));
    if (outputPaneManagerWidget) {
        outputPaneManagerWidget->setStyleSheet(styleContent);
    }

    QWidget *debuggerMainWindow = widgetForClass(QStringLiteral("Debugger::DebuggerMainWindow"));
    if (debuggerMainWindow) {
        setStylesheetOnChildWidgetsWithClass(debuggerMainWindow, styleContent, QStringLiteral("QDockWidget"));
    }
}

void CreatorStyleEditPlugin::modeChanged(Core::IMode *mode)
{
    if (mode->id() == Debugger::Constants::MODE_DEBUG) {
        stylesheetChanged();
    }
}

Q_EXPORT_PLUGIN2(CreatorStyleEdit, CreatorStyleEditPlugin)

