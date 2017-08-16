#ifndef GVA_H
#define GVA_H
#include <QVariant>
#include <QMetaType>

enum LEVEL{INFO,WARNNING,ERROR,FATAL};
struct loginfo
{
    char log[100];
    time_t logtime;
    LEVEL level;
};
Q_DECLARE_METATYPE(loginfo)

#endif // GVA_H
