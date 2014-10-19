/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the MIT License. See LICENSE for details.
 *
 */

#ifndef CREATORSTYLEEDIT_H
#define CREATORSTYLEEDIT_H

#include <QPalette>
#include "creatorstyleedit_global.h"
#include <extensionsystem/iplugin.h>

class QSettings;

namespace CreatorStyleEdit {
namespace Internal {

class StyleEditor;

class CreatorStyleEditPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "CreatorStyleEdit.json")

public:
    CreatorStyleEditPlugin();
    ~CreatorStyleEditPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    bool delayedInitialize();
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:
    void triggerAction();
    void stylesheetChanged();

private:
    QString styleSheetPathFromSettings() const;
    QColor styleSheetBaseColorFromSetting() const;
    QWidget *widgetForClass(const QString &className);
    QWidget *childWidgetForClass(QWidget *widget, const QString &className);
    void writeStyleSheetToSettings();
    QString settingsKey(const QString &key) const;
    void debugWidget(QWidget *widget);
    StyleEditor *m_styleEditor;
};

} // namespace Internal
} // namespace CreatorStyleEdit

#endif // CREATORSTYLEEDIT_H

