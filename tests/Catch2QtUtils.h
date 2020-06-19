#ifndef CATCH2QTUTILS_H
#define CATCH2QTUTILS_H

// Qt
#include <QApplication>
#include <QElapsedTimer>
#include <QString>
#include <QUrl>
#include <QtTest>

// std
#include <fstream>

/**
 * Wait until the active window can be qobject_cast'ed to class T.
 * @return a pointer to the active window on success, nullptr on failure
 */
template <class T> static T waitForActiveWindow(int timeout = 5000) {
    QElapsedTimer timer;
    timer.start();
    while (!timer.hasExpired(timeout)) {
        T window = qobject_cast<T>(QApplication::activeWindow());
        if (window) {
            return window;
        }
        QTest::qWait(200);
    }
    return nullptr;
}

// Let Catch know how to print some Qt types
std::ostream& operator<<(std::ostream& ostr, const QString& str);
std::ostream& operator<<(std::ostream& ostr, const QUrl& url);

#endif // CATCH2QTUTILS_H
