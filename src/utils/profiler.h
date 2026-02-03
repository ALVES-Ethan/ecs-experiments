#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <fstream>
#include <mutex>
#include <iomanip>
#include <sstream>
#include <algorithm>

class Profiler {
public:
    struct Stats {
        double totalTime = 0.0;
        uint64_t totalCalls = 0;

        double firstCallTime = 0.0;
        bool firstCallThisFrame = true;
    };

    static void beginFrame() {
        std::lock_guard<std::mutex> lock(mutex());
        for (auto& [_, s] : stats()) {
            s.firstCallThisFrame = true;
            s.firstCallTime = 0.0;
        }
    }

    static void addSample(const char* name, double durationMs) {
        std::lock_guard<std::mutex> lock(mutex());
        auto& s = stats()[name];

        if (s.firstCallThisFrame) {
            s.firstCallTime = durationMs;
            s.firstCallThisFrame = false;
        }

        s.totalTime += durationMs;
        s.totalCalls++;
    }

    static void dump(const std::string& filename) {
        std::lock_guard<std::mutex> lock(mutex());
        std::ofstream file(filename);

        file << "Name"
            << std::string(28, ' ')
            << "FirstCallBudget (ms)"
            << std::string(4, ' ')
            << "AverageCallBudget (ms)\n";

        for (const auto& [name, s] : stats()) {
            double avg =
                s.totalCalls ? s.totalTime / s.totalCalls : 0.0;

            file << name
                << std::string(32 - std::min<size_t>(32, name.size()), ' ')
                << formatMs(s.firstCallTime)
                << std::string(8, ' ')
                << formatMs(avg)
                << "\n";
        }
    }

private:
    static std::unordered_map<std::string, Stats>& stats() {
        static std::unordered_map<std::string, Stats> instance;
        return instance;
    }

    static std::mutex& mutex() {
        static std::mutex instance;
        return instance;
    }

    static std::string formatMs(double ms) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << ms << "ms";
        return ss.str();
    }
};

class ProfileScope {
public:
    explicit ProfileScope(const char* name)
        : m_name(name),
        m_start(std::chrono::high_resolution_clock::now()) {
    }

    ~ProfileScope() {
        auto end = std::chrono::high_resolution_clock::now();
        double durationMs =
            std::chrono::duration<double, std::milli>(end - m_start).count();
        Profiler::addSample(m_name, durationMs);
    }

private:
    const char* m_name;
    std::chrono::high_resolution_clock::time_point m_start;
};

#ifdef ENABLE_PROFILER
#define PROFILE_SCOPE(name) ProfileScope scope_##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__func__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif