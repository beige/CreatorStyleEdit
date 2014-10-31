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

namespace Core {
class IMode;
}

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
    void styleNameChanged(const QString &styleName);
    void modeChanged(Core::IMode *mode);

private:
    QString customStyleSheetPathFromSettings() const;
    QString selectedStyleFromSettings() const;
    QColor styleSheetBaseColorFromSetting() const;
    QWidget *widgetForClass(const QString &className);
    QWidget *childWidgetForClass(QWidget *widget, const QString &className);
    void writeStyleSheetToSettings();
    QString settingsKey(const QString &key) const;
    void setStylesheetOnChildWidgetsWithClass(QWidget *widget, const QString &stylesheet,
                                              const QString &className);
    void applyStylesheet();
    StyleEditor *m_styleEditor;
};

} // namespace Internal
} // namespace CreatorStyleEdit

#endif // CREATORSTYLEEDIT_H

