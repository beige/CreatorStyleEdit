/**
 * @author  Thomas Baumann <teebaum@ymail.com>
 *
 * @section LICENSE
 * Licensed under the MIT License. See LICENSE for details.
 *
 */

#ifndef CREATORSTYLEEDIT_GLOBAL_H
#define CREATORSTYLEEDIT_GLOBAL_H

#include <QtGlobal>

#if defined(CREATORSTYLEEDIT_LIBRARY)
#  define CREATORSTYLEEDITSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CREATORSTYLEEDITSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CREATORSTYLEEDIT_GLOBAL_H

