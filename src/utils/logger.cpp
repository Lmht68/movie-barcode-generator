#include "logger.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>

static std::string GetCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t); // Windows-safe
#else
    localtime_r(&t, &tm); // POSIX-safe
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

namespace app_logger
{
    void InitLogger()
    {

        try
        {
            // Create logs directory if it doesn't exist
            if (!std::filesystem::exists("logs"))
            {
                std::filesystem::create_directory("logs");
            }

            // Get current timestamp for log file name
            std::string log_filename = "logs/log_" + GetCurrentTimestamp() + ".log";
            // Setup sinks (Console + Dynamic File)
            std::vector<spdlog::sink_ptr> sinks;
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_filename, true);
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
            // Flush every 10 seconds so logs appear in the file while the app is running
            spdlog::flush_every(std::chrono::seconds(10));
            // Flush on Warning to ensure logs are saved if app crashes
            spdlog::flush_on(spdlog::level::warn);

            spdlog::info("Logger initialized.");
        }
        catch (const spdlog::spdlog_ex &ex)
        {
            std::cerr << "spdlog initialization failed: " << ex.what() << std::endl;
        }
        catch (const std::filesystem::filesystem_error &fe)
        {
            std::cerr << "Failed to create logs directory: " << fe.what() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to initialize logger: " << e.what() << std::endl;
        }
    }
} // namespace app_logger