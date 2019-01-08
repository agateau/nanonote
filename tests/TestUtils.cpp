#include "TestUtils.h"

std::ostream &operator<<(std::ostream &ostr, const QString &str)
{
    ostr << '"' << str.toStdString() << '"';
    return ostr;
}
