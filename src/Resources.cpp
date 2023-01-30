#include "Resources.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QGlobalStatic>

#include "BuildConfig.h"

class ResourcesInfo {
public:
    ResourcesInfo() {
        if (tryPath(QCoreApplication::applicationDirPath() + '/' + BIN_TO_DATA_DIR)) {
            return;
        }
        // Try the absolute install path, useful when running the app from the
        // build directory
        if (tryPath(QString(INSTALL_PREFIX) + '/' + DATA_INSTALL_DIR)) {
            return;
        }
        qWarning() << "Can't find data dir, has the app been installed?";
    }

    std::optional<QString> findDir(const QString& name) const {
        if (!mDataDir.has_value()) {
            return {};
        }
        auto path = mDataDir.value().filePath(name);
        if (!QFile::exists(path)) {
            qWarning() << "Can't find" << name << "in" << mDataDir.value().absolutePath();
            return {};
        }
        return path;
    }

private:
    bool tryPath(const QString& path) {
        QDir dir(path);
        if (!dir.exists()) {
            return false;
        }
        mDataDir = dir;
        return true;
    }

    std::optional<QDir> mDataDir;
};

Q_GLOBAL_STATIC(ResourcesInfo, info)

namespace Resources {

std::optional<QString> findDir(const QString& name) {
    return info()->findDir(name);
}

} // namespace Resources
