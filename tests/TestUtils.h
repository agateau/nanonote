#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <fstream>

#include <QString>

// Let Catch know how to print a QString
std::ostream &operator<<(std::ostream &ostr, const QString &str);

#endif // TESTUTILS_H
