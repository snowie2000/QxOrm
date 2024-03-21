#ifndef _QX_FARLAND_H_
#define _QX_FARLAND_H_

#include "QxOrm.h"
#include <QMutex>
#include <QString>
#include <QThread>
#include <any>
#include <atomic>
#include <functional>
#include <map>
#include <vector>

namespace qx {
typedef qx::QxCollection<QString, qx::any> hook_list;
typedef std::map<QString, hook_list> hook_map;
extern std::map<Qt::HANDLE, hook_map> threadhooks;
extern std::atomic<int> hookCounter;
extern QMutex hookLock;

using TableHookFunction = std::function<void(QString* tableName)>;

#define QX_HOOK_TABLENAME "tableHook"
#define QX_HOOK_FIELDNAME "fieldHook"

template <typename T, typename... Args>
QString registerHook(QString hookType, std::function<void(Args... args)> fn) {
    QString className = qx::QxClass<T>::getSingleton()->getKey();
    return registerHook(hookType, className, fn);
}

template <typename... Args>
QString registerHook(QString hookType, QString className, std::function<void(Args... args)> fn) {
    QString hookId = QString("tbl_%1_ty_%2_sn_%3").arg(className, hookType).arg(++hookCounter);
    auto tid = QThread::currentThreadId();

    QMutexLocker _(&hookLock);
    if (threadhooks.find(tid) == threadhooks.end()) {
        threadhooks[tid] = hook_map();
    }
    auto& hooks = threadhooks[tid];
    if (hooks.find(hookType) == hooks.end()) {
        hooks[hookType] = hook_list();
    }
    hooks[hookType].insert(hookId, fn);
    return hookId;
}

void unregisterHook(QString hookId);

template <typename... Args>
bool callHook(QString hookType, QString key, Args... args) {
    auto tid = QThread::currentThreadId();
    QMutexLocker _(&hookLock);
    if (threadhooks.find(tid) == threadhooks.end()) {
        return false;
    }
    auto& hooks = threadhooks[tid];
    if (hooks.find(hookType) == hooks.end()) {
        return false;
    }
    auto& hookList = hooks[hookType];
    bool found = false;
    QString hookPrefix = QString("tbl_%1_ty_%2_sn_").arg(key, hookType);
    for (int i = 0; i < hookList.count(); i++) {
        if (hookList.getKeyByIndex(i).startsWith(hookPrefix)) {
            found = true;
            std::function<void(Args... args)> fn = qx::any_cast<std::function<void(Args... args)>>(hookList.getByIndex(i));
            fn(args...);
        }
    }
    return found;
}
} // namespace qx

#endif // _QX_FARLAND_H_
