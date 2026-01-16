#pragma once

#include <QLatin1String>

namespace ResourcePath {
    constexpr auto kAppIcon = ":/icons/app-icon.png";
    constexpr auto kAppStyleDark = ":qdarkstyle/dark/darkstyle.qss";
    constexpr auto kAppStyleLight = ":qdarkstyle/light/lightstyle.qss";
    constexpr auto kAppStyle = ":/style/style.qss";
}  // namespace ResourcePath

namespace MediaFormat {
    inline constexpr const char* kImageExtensions[] = {"jpg", "jpeg", "png", "gif"};
    inline constexpr const char* kVideoExtensions[] = {"mp4", "mkv", "avi", "mov"};

    inline bool isMedia(const QString& suffix) {
        // Use QLatin1String to avoid heap allocations during comparison
        for (const char* e : kImageExtensions) {
            if (suffix.compare(QLatin1String(e), Qt::CaseInsensitive) == 0) return true;
        }
        for (const char* e : kVideoExtensions) {
            if (suffix.compare(QLatin1String(e), Qt::CaseInsensitive) == 0) return true;
        }
        return false;
    }
}  // namespace MediaFormat