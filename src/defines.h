/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the GNU GENERAL PUBLIC LICENSE. See LICENSE for details.
 *
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <QString>

namespace CreatorStyleEdit {
namespace Internal {

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

#endif // DEFINES_H
