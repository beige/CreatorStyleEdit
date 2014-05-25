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
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:
    void triggerAction();
    void setPaletteOnClass(const QPalette &palette, const char *className);
    void paletteChanged(const QPalette &palette);

private:
    void applyPaletteOnAllWidgets(const QPalette &palette);
    QPalette paletteFromSettings() const;
    void writePaletteToSettings(const QPalette &palette);
    QString settingsKeyForColorRole(QPalette::ColorRole role) const;
    QString settingsKey(const QString &key) const;
    QColor colorFromSettings(QSettings *settings, QPalette::ColorRole colorRole) const;
    StyleEditor *m_styleEditor;
};

} // namespace Internal
} // namespace CreatorStyleEdit

#endif // CREATORSTYLEEDIT_H

