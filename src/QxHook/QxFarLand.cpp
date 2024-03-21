
#include <QRegularExpression>
#include <QxHook/QxFarLand.h>

namespace qx {

std::map<Qt::HANDLE, hook_map> threadhooks;
std::atomic<int> hookCounter;
QMutex hookLock;

void unregisterHook(QString hookId) {
    QMutexLocker _(&hookLock);
    Qt::HANDLE tid = QThread::currentThreadId();
    QRegularExpression re("tbl_(.+)_ty_(.+)_sn_(.+)");
    QRegularExpressionMatch match = re.match(hookId);
    if (match.hasMatch()) {
        if (threadhooks.find(tid) != threadhooks.end()) {
            auto& hookmap = threadhooks[tid];
            auto it = hookmap.find(match.captured(2));
            if (it != hookmap.end()) {
                auto& hooklist = it->second;
                if (hooklist.contains(hookId)) {
                    hooklist.removeByKey(hookId);
                }
                if (hooklist.empty()) {
                    hookmap.erase(it);
                }
                if (hookmap.empty()) {
                    threadhooks.erase(tid);
                }
            }
        }
    }
}

} // namespace qx
