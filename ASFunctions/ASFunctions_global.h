#ifndef ASFUNCTIONS_GLOBAL_H
#define ASFUNCTIONS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ASFUNCTIONS_LIBARARY)
#  define ASFUNCTIONS_EXPORT Q_DECL_EXPORT
#else
#  define ASFUNCTIONS_EXPORT Q_DECL_IMPORT
#endif

#endif // ASFUNCTIONS_GLOBAL_H
