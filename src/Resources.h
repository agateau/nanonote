#ifndef RESOURCES_H
#define RESOURCES_H

class QString;

#include <optional>

namespace Resources {

/**
 * Return the path to a directory called name in the resources directory if
 * it exists
 */
std::optional<QString> findDir(const QString& name);

} // namespace Resources

#endif /* RESOURCES_H */
