#include "utils/logger.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "utils/exceptions.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
    std::string GetCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);

        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &t);  // Windows-safe
#else
        localtime_r(&t, &tm);  // POSIX-safe
#endif

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
        return oss.str();
    }

    template <typename Rep, typename Period>

    void DeleteOldLogs(
        const std::filesystem::path &log_dir, std::chrono::duration<Rep, Period> max_age
    ) {
        const auto now = std::chrono::system_clock::now();

        for (const auto &entry : std::filesystem::directory_iterator(log_dir)) {
            if (!entry.is_regular_file()) continue;

            if (entry.path().extension() != ".log") continue;

            auto ftime = entry.last_write_time();

            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - std::filesystem::file_time_type::clock::now() +
                std::chrono::system_clock::now()
            );

            if (now - sctp > max_age) {
                std::filesystem::remove(entry.path());
            }
        }
    }
}  // namespace

namespace app_logger {
    void InitLogger() {
        try {
            // Create logs directory if it doesn't exist
            if (!std::filesystem::exists("logs")) {
                std::filesystem::create_directory("logs");
            }

            // Delete old logs
#ifndef NDEBUG
            DeleteOldLogs("logs", std::chrono::hours(1));  // 1 hour for debug builds
#else
            DeleteOldLogs("logs",
                          std::chrono::hours(24 * 7));  // 7 days for release builds
#endif
            // Get current timestamp for log file name
            std::string log_filename = "logs/log_" + GetCurrentTimestamp() + ".log";
            // Setup sinks (Console + Dynamic File)
            std::vector<spdlog::sink_ptr> sinks;
            auto file_sink =
                std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_filename, true);
            file_sink->set_level(spdlog::level::trace);
            sinks.push_back(file_sink);
#ifndef NDEBUG
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::debug);
            sinks.push_back(console_sink);
#endif
            // Create and Register Logger
            auto logger = std::make_shared<spdlog::logger>("main_log", sinks.begin(), sinks.end());
            logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
            logger->set_level(spdlog::level::trace);
            spdlog::register_logger(logger);
            spdlog::set_default_logger(logger);
            // Flush every 10 seconds so logs appear in the file while the app is
            // running
            spdlog::flush_every(std::chrono::seconds(10));
            // Flush on Warning to ensure logs are saved if app crashes
            spdlog::flush_on(spdlog::level::warn);

            spdlog::info("Logger initialized.");
        } catch (const spdlog::spdlog_ex &e) {
            throw AppException::LoggerInitException(
                std::string("spdlog initialization failed: ") + e.what()
            );
        } catch (const std::filesystem::filesystem_error &e) {
            throw AppException::LoggerInitException(
                std::string("Failed to create logs directory: ") + e.what()
            );
        } catch (const std::exception &e) {
            throw AppException::LoggerInitException(
                std::string("Failed to initialize logger: ") + e.what()
            );
        }
    }
}  // namespace app_logger