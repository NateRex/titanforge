#include <common/Utils.h>
#include <common/Constants.h>
#include <common/exceptions/IllegalStateException.h>
#include <cmath>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <limits.h>
#endif

bool equals(float a, float b, float tol)
{
    return fabs(a - b) <= tol;
}

float rad2Deg(float rad)
{
    return rad * 180. / PI;
}

float deg2Rad(float deg)
{
    return deg * PI / 180.;
}

std::string resolvePath(const std::string& relPath)
{
    char path[260];

#ifdef _WIN32
    GetModuleFileNameA(NULL, path, 260);

#elif defined(__linux__)
    ssize_t count = readlink("/proc/self/exe", path, 259);
    if (count == -1) {
        throw IllegalStateException("Failed to get executable path");
    }

    // Ensure null termination
    path[count] = '\0';

#elif defined(__APPLE__)
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        throw IllegalStateException("Failed to get executable path");
    }

#endif

    std::filesystem::path execPath(path);
    return execPath.parent_path().append(relPath).string();
}

bool checkSuffix(const std::string& str, const std::string& suffix)
{
    if (str.length() < suffix.length()) {
        return false;
    }

    return str.substr(str.length() - suffix.length()) == suffix;
}