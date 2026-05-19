#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <limits>
#include <mutex>
#include <sstream>
#include <thread>
#include <curl/curl/curl.h>
#include <curl/curl/easy.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/json/json.hpp>
#include <Utils/Logger/Logger.hpp>


//   Refer to https://support.playhive.com/in-game-unlocks/
static const std::vector<int> xpTable = {
    0, // Level 0
    0, // Level 1
    150, // Level 2
    450, // Level 3
    900, // Level 4
    1500, // Level 5
    2250, // Level 6
    3150, // Level 7
    4200, // Level 8
    5400, // Level 9
    6750, // Level 10
    8250, // Level 11
    9900, // Level 12
    11700, // Level 13
    13650, // Level 14
    15750, // Level 15
    18000, // Level 16
    20400, // Level 17
    22950, // Level 18
    25650, // Level 19
    28500, // Level 20
    31500, // Level 21
    34650, // Level 22
    37950, // Level 23
    41400, // Level 24
    45000, // Level 25
    48750, // Level 26
    52650, // Level 27
    56700, // Level 28
    60900, // Level 29
    65250, // Level 30
    69750, // Level 31
    74400, // Level 32
    79200, // Level 33
    84150, // Level 34
    89250, // Level 35
    94500, // Level 36
    99900, // Level 37
    105450, // Level 38
    111150, // Level 39
    117000, // Level 40
    123000, // Level 41
    129150, // Level 42
    135450, // Level 43
    141900, // Level 44
    148500, // Level 45
    155250, // Level 46
    162150, // Level 47
    169200, // Level 48
    176400, // Level 49
    183750, // Level 50
    191250, // Level 51
    198900, // Level 52
    206550, // Level 53
    214200, // Level 54
    221850, // Level 55
    229500, // Level 56
    237150, // Level 57
    244800, // Level 58
    252450, // Level 59
    260100, // Level 60
    267750, // Level 61
    275400, // Level 62
    283050, // Level 63
    290700, // Level 64
    298350, // Level 65
    306000, // Level 66
    313650, // Level 67
    321300, // Level 68
    328950, // Level 69
    336600, // Level 70
    344250, // Level 71
    351900, // Level 72
    359550, // Level 73
    367200, // Level 74
    374850, // Level 75
    382500, // Level 76
    390150, // Level 77
    397800, // Level 78
    405450, // Level 79
    413100, // Level 80
    420750, // Level 81
    428400, // Level 82
    436050, // Level 83
    443700, // Level 84
    451350, // Level 85
    459000, // Level 86
    466650, // Level 87
    474300, // Level 88
    481950, // Level 89
    489600, // Level 90
    497250, // Level 91
    504900, // Level 92
    512550, // Level 93
    520200, // Level 94
    527850, // Level 95
    535500, // Level 96
    543150, // Level 97
    550800, // Level 98
    558450, // Level 99
    566100 // Level 100
};


namespace Hive
{
    inline size_t CurlWriteCallback(void* contents, const size_t size, const size_t nmemb, void* userdata)
    {
        const size_t totalSize = size * nmemb;
        auto* output = static_cast<std::string*>(userdata);
        output->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    struct httpResponse
    {
        std::string response;
        int httpCode;
    };

    struct RateLimitHeaders
    {
        int limit = -1;
        int remaining = -1;
        int retryAfterSeconds = -1;
        long long resetEpoch = 0;
    };

    inline std::string trimHeaderValue(std::string value)
    {
        while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front())))
        {
            value.erase(value.begin());
        }
        while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back())))
        {
            value.pop_back();
        }
        return value;
    }

    inline int parseHeaderInt(const std::string& value, const int fallback = -1)
    {
        try
        {
            return std::stoi(value);
        }
        catch (...)
        {
            return fallback;
        }
    }

    inline long long parseHeaderLongLong(const std::string& value, const long long fallback = 0)
    {
        try
        {
            return std::stoll(value);
        }
        catch (...)
        {
            return fallback;
        }
    }

    inline size_t CurlHeaderCallback(char* buffer, const size_t size, const size_t nitems, void* userdata)
    {
        const size_t totalSize = size * nitems;
        auto* headers = static_cast<RateLimitHeaders*>(userdata);
        if (!headers) return totalSize;

        std::string header(buffer, totalSize);
        const size_t separator = header.find(':');
        if (separator == std::string::npos) return totalSize;

        std::string name = header.substr(0, separator);
        std::ranges::transform(name, name.begin(),
                               [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        const std::string value = trimHeaderValue(header.substr(separator + 1));

        if (name == "x-ratelimit-limit") headers->limit = parseHeaderInt(value);
        else if (name == "x-ratelimit-remaining") headers->remaining = parseHeaderInt(value);
        else if (name == "x-ratelimit-reset") headers->resetEpoch = parseHeaderLongLong(value);
        else if (name == "retry-after") headers->retryAfterSeconds = parseHeaderInt(value);

        return totalSize;
    }

    inline std::mutex& getRateLimitMutex()
    {
        static std::mutex mutex;
        return mutex;
    }

    inline std::chrono::steady_clock::time_point& getRateLimitPauseUntil()
    {
        static std::chrono::steady_clock::time_point pauseUntil = std::chrono::steady_clock::time_point::min();
        return pauseUntil;
    }

    inline void waitForRateLimitWindow()
    {
        while (true)
        {
            std::chrono::steady_clock::time_point pauseUntil;
            {
                std::lock_guard lock(getRateLimitMutex());
                pauseUntil = getRateLimitPauseUntil();
            }

            if (std::chrono::steady_clock::now() >= pauseUntil) return;
            std::this_thread::sleep_until(pauseUntil);
        }
    }

    inline void updateRateLimitWindow(const int httpCode, const RateLimitHeaders& headers)
    {
        int waitSeconds = 0;
        if ((httpCode == 429 || headers.remaining == 0) && headers.retryAfterSeconds > 0)
        {
            waitSeconds = headers.retryAfterSeconds;
        }
        else if (headers.remaining == 0 && headers.resetEpoch > 0)
        {
            const auto resetTime = std::chrono::system_clock::from_time_t(
                static_cast<std::time_t>(headers.resetEpoch));
            waitSeconds = static_cast<int>(
                std::chrono::duration_cast<std::chrono::seconds>(
                    resetTime - std::chrono::system_clock::now()).count());
            waitSeconds = std::max(waitSeconds, 1);
        }

        if (waitSeconds <= 0) return;

        const auto pauseUntil = std::chrono::steady_clock::now() +
            std::chrono::seconds(waitSeconds) + std::chrono::milliseconds(250);
        std::lock_guard lock(getRateLimitMutex());
        getRateLimitPauseUntil() = std::max(getRateLimitPauseUntil(), pauseUntil);
    }

    inline std::string getPrestigePrefix(const int prestige)
    {
        if (prestige <= 0) return "";

        static constexpr std::array<const char*, 11> romanPrestiges = {
            "", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X"
        };

        if (prestige < static_cast<int>(romanPrestiges.size()))
        {
            return "[" + std::string(romanPrestiges[prestige]) + "] ";
        }

        return "[" + std::to_string(prestige) + "] ";
    }

    inline std::string formatPrestigeLevel(const int prestige, const int level)
    {
        return getPrestigePrefix(prestige) + std::to_string(level);
    }

    class PlayerStats
    {
        float fkdr;
        float kd;
        float winRate;
        int level;
        int played;
        long long firstPlayed;
        int victories;
        int losses;
        int kills;
        int deaths;
        int code = 0;
        int prestige = 0;
        std::unordered_map<std::string, int> extraStats;

    public:
        explicit PlayerStats(const float fkdr = 0.0f, const float kd = 0.0f, const float winRate = 0.0f)
            : fkdr(fkdr), kd(kd), winRate(winRate), level(0), played(0), firstPlayed(0),
              victories(0), losses(0), kills(0), deaths(0)
        {
        }

        void setFKDR(const double fkdrValue) { fkdr = fkdrValue; }
        void setKD(const double kdValue) { kd = kdValue; }
        void setWinRate(const double winRateValue) { winRate = winRateValue; }
        void setCode(const int codeValue) { code = codeValue; }
        void setLevel(const int levelValue) { level = levelValue; }
        void setPlayed(const int playedValue) { played = playedValue; }
        void setFirstPlayed(const long long firstPlayedValue) { firstPlayed = firstPlayedValue; }
        void setVictories(const int victoriesValue) { victories = victoriesValue; }
        void setLosses(const int lossesValue) { losses = lossesValue; }
        void setKills(const int killsValue) { kills = killsValue; }
        void setDeaths(const int deathsValue) { deaths = deathsValue; }
        void setPrestige(const int pres) { prestige = pres; }
        void setExtraStat(const std::string& name, const int value) { extraStats[name] = value; }

        float getFKDR() const { return fkdr; }
        float getKD() const { return kd; }
        float getWinRate() const { return winRate; }
        int getLevel() const { return level; }
        int getPlayed() const { return played; }
        long long getFirstPlayed() const { return firstPlayed; }
        int getVictories() const { return victories; }
        int getLosses() const { return losses; }
        int getKills() const { return kills; }
        int getDeaths() const { return deaths; }
        int getCode() const { return code; };
        int getIntPrestige() const { return prestige; }

        int getExtraStat(const std::string& name, const int fallbackValue = -1) const
        {
            const auto it = extraStats.find(name);
            if (it == extraStats.end()) return fallbackValue;
            return it->second;
        }

        std::string getPrestige() const
        {
            return getPrestigePrefix(prestige);
        };
    };

    struct LeaderboardEntry
    {
        int index = -1;
        int humanIndex = -1;
        std::string username;
        int xp = -1;
        int played = -1;
        int victories = -1;
        int kills = -1;
        int deaths = -1;
        int prestige = -1;
        std::unordered_map<std::string, int> extraStats;

        int getExtraStat(const std::string& name, const int fallbackValue = -1) const
        {
            const auto it = extraStats.find(name);
            if (it == extraStats.end()) return fallbackValue;
            return it->second;
        }
    };

    struct LeaderboardResult
    {
        std::vector<LeaderboardEntry> entries;
        int code = 0;
    };

    inline double roundToSecond(const float value)
    {
        return std::round(value * 100.0f) / 100.0f;
    }

    inline double safeDivideStat(const int numerator, const int denominator,
                                 const double fallbackWhenNoDenominator = 0.0)
    {
        if (denominator <= 0)
        {
            return fallbackWhenNoDenominator;
        }

        return static_cast<double>(numerator) / static_cast<double>(denominator);
    }

    inline httpResponse GetString(const std::string& URL)
    {
        static const bool curlInitialized = []()
        {
            return curl_global_init(CURL_GLOBAL_DEFAULT) == CURLE_OK;
        }();

        if (!curlInitialized)
        {
            Logger::error("Hive API request failed: curl_global_init failed");
            return {"", 0};
        }

        CURL* curl = curl_easy_init();
        if (!curl)
        {
            Logger::error("Hive API request failed: curl_easy_init failed");
            return {"", 0};
        }

        std::string responseBody;
        RateLimitHeaders rateLimitHeaders;
        long statusCode = 0;

        waitForRateLimitWindow();
        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "FlarialClient/1.0");
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, CurlHeaderCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &rateLimitHeaders);

        const CURLcode curlResult = curl_easy_perform(curl);
        if (curlResult == CURLE_OK)
        {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
        }
        else
        {
            Logger::warn("Hive API request failed: {}", curl_easy_strerror(curlResult));
        }

        curl_easy_cleanup(curl);
        updateRateLimitWindow(static_cast<int>(statusCode), rateLimitHeaders);
        return {responseBody, static_cast<int>(statusCode)};
    }

    inline std::string replaceAll(std::string subject, const std::string& search,
                                  const std::string& replace)
    {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos)
        {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return subject;
    }

    inline std::string encodeUrlComponent(const std::string& value)
    {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (const unsigned char c : value)
        {
            if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            {
                escaped << static_cast<char>(c);
                continue;
            }

            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << static_cast<int>(c);
            escaped << std::nouppercase;
        }

        return escaped.str();
    }

    inline int getLevelFromXP(const int xp, const int max = 100)
    {
        if (xp <= 0) return 1;

        const int maxXP = xpTable[max];

        if (xp >= maxXP)
        {
            return max;
        }
        else
        {
            const auto it = std::ranges::upper_bound(xpTable, xp);
            const int level = static_cast<int>(it - xpTable.begin()) - 1;
            return level;
        }
    }

    inline LeaderboardResult GetLeaderboard(const std::string& gameId, const bool monthly,
                                            const int monthlyYear = 0,
                                            const int monthlyMonth = 0,
                                            const int amount = 100,
                                            const int skip = 0)
    {
        LeaderboardResult result;

        try
        {
            std::string url;
            if (monthly && monthlyYear > 0 && monthlyMonth > 0)
            {
                const int requestedAmount = std::clamp(amount, 1, 100);
                const int requestedSkip = std::max(skip, 0);
                url = "https://api.playhive.com/v0/game/monthly/" + gameId + "/" +
                    std::to_string(monthlyYear) + "/" + std::to_string(monthlyMonth) + "/" +
                    std::to_string(requestedAmount) + "/" + std::to_string(requestedSkip);
            }
            else
            {
                const std::string path = monthly ? "monthly" : "all";
                url = "https://api.playhive.com/v0/game/" + path + "/" + gameId;
            }

            auto [response, httpCode] = GetString(url);
            if (httpCode != 200)
            {
                result.code = httpCode == 404 ? 1 : 3;
                return result;
            }

            nlohmann::json jsonData = nlohmann::json::parse(response);
            if (!jsonData.is_array())
            {
                result.code = 2;
                return result;
            }

            for (const auto& entryData : jsonData)
            {
                if (!entryData.is_object() || !entryData.contains("username")) continue;

                LeaderboardEntry entry;
                entry.username = entryData["username"].get<std::string>();
                if (entryData.contains("index")) entry.index = entryData["index"].get<int>();
                if (entryData.contains("human_index")) entry.humanIndex = entryData["human_index"].
                    get<int>();
                if (entryData.contains("xp")) entry.xp = entryData["xp"].get<int>();
                if (entryData.contains("played")) entry.played = entryData["played"].get<int>();
                if (entryData.contains("victories")) entry.victories = entryData["victories"].get<
                    int>();
                if (entryData.contains("kills")) entry.kills = entryData["kills"].get<int>();
                if (entryData.contains("deaths")) entry.deaths = entryData["deaths"].get<int>();
                if (entryData.contains("prestige")) entry.prestige = entryData["prestige"].get<
                    int>();

                if (entryData.contains("beds_destroyed")) entry.extraStats["beds_destroyed"] =
                    entryData["beds_destroyed"].get<int>();
                if (entryData.contains("final_kills")) entry.extraStats["final_kills"] = entryData[
                    "final_kills"].get<int>();
                if (entryData.contains("ores_mined")) entry.extraStats["ores_mined"] = entryData[
                    "ores_mined"].get<int>();
                if (entryData.contains("mystery_chests_destroyed")) entry.extraStats[
                    "mystery_chests_destroyed"] = entryData["mystery_chests_destroyed"].get<int>();
                if (entryData.contains("spells_used")) entry.extraStats["spells_used"] = entryData[
                    "spells_used"].get<int>();
                if (entryData.contains("coins")) entry.extraStats["coins"] = entryData["coins"].get<
                    int>();
                if (entryData.contains("murders")) entry.extraStats["murders"] = entryData[
                    "murders"].get<int>();
                if (entryData.contains("murderer_eliminations")) entry.extraStats[
                    "murderer_eliminations"] = entryData["murderer_eliminations"].get<int>();
                if (entryData.contains("flags_captured")) entry.extraStats["flags_captured"] =
                    entryData["flags_captured"].get<int>();
                if (entryData.contains("flags_returned")) entry.extraStats["flags_returned"] =
                    entryData["flags_returned"].get<int>();
                if (entryData.contains("assists")) entry.extraStats["assists"] = entryData[
                    "assists"].get<int>();
                if (entryData.contains("crates")) entry.extraStats["crates"] = entryData["crates"].
                    get<int>();
                if (entryData.contains("deathmatches")) entry.extraStats["deathmatches"] = entryData
                    ["deathmatches"].get<int>();
                if (entryData.contains("cows")) entry.extraStats["cows"] = entryData["cows"].get<
                    int>();
                if (entryData.contains("teleporters_used")) entry.extraStats["teleporters_used"] =
                    entryData["teleporters_used"].get<int>();
                if (entryData.contains("launchpads_used")) entry.extraStats["launchpads_used"] =
                    entryData["launchpads_used"].get<int>();
                if (entryData.contains("flares_used")) entry.extraStats["flares_used"] = entryData[
                    "flares_used"].get<int>();

                result.entries.push_back(std::move(entry));
            }
        }
        catch (...)
        {
            result.code = 3;
        }

        return result;
    }

    inline PlayerStats GetStats(const std::string& gameId, const std::string& username,
                                const bool monthly = false, const int monthlyYear = 0,
                                const int monthlyMonth = 0)
    {
        PlayerStats stats;

        try
        {
            const std::string encodedUsername = encodeUrlComponent(username);
            std::string url;
            if (monthly)
            {
                url = "https://api.playhive.com/v0/game/monthly/player/" + gameId + "/" +
                    encodedUsername;
                if (monthlyYear > 0 && monthlyMonth > 0)
                {
                    url += "/" + std::to_string(monthlyYear) + "/" + std::to_string(monthlyMonth);
                }
            }
            else
            {
                url = "https://api.playhive.com/v0/game/all/" + gameId + "/" + encodedUsername;
            }

            httpResponse response = GetString(url);
            std::string jsonResponse = response.response;

            // Logger::info("{}", response.httpCode);
            if (response.httpCode != 200)
            {
                stats.setCode(response.httpCode == 404 ? 1 : 3);
                return stats;
            }

            nlohmann::json jsonData = nlohmann::json::parse(jsonResponse);

            // Logger::info("{}", jsonResponse);
            if (jsonData.contains("first_played"))
                stats.setFirstPlayed(jsonData["first_played"].get<long long>());

            // Helper to safely extract an int field, returning fallback if missing
            auto safeInt = [&](const char* key, int fallback = 0) -> int {
                return jsonData.contains(key) ? jsonData[key].get<int>() : fallback;
            };

            const int apiXp = safeInt("xp", -1);
            if (apiXp >= 0) stats.setExtraStat("xp", apiXp);

            const int humanIndex = safeInt("human_index", -1);
            if (humanIndex >= 0 && humanIndex < std::numeric_limits<int>::max())
            {
                stats.setExtraStat("rank", humanIndex);
            }
            else
            {
                const int index = safeInt("index", -1);
                if (index >= 0 && index < std::numeric_limits<int>::max() - 1)
                {
                    stats.setExtraStat("rank", index + 1);
                }
            }

            if (gameId == "bed")
            {
                int finalKills = safeInt("final_kills");
                int xp = safeInt("xp");
                int kills = safeInt("kills");
                int deaths = safeInt("deaths");
                int victories = safeInt("victories");
                int played = safeInt("played");

                int finalDeaths = played - victories;

                stats.setFKDR(roundToSecond(
                    static_cast<float>(safeDivideStat(finalKills, finalDeaths,
                                                      static_cast<double>(finalKills)))));
                stats.setKD(roundToSecond(
                    static_cast<float>(safeDivideStat(kills, deaths, static_cast<double>(kills)))));
                stats.setWinRate(std::round(safeDivideStat(victories, played) * 100.0));
                stats.setLevel(getLevelFromXP(xp));
                stats.setPlayed(played);
                stats.setVictories(victories);
                stats.setLosses(played - victories);
                stats.setKills(kills);
                stats.setDeaths(deaths);
                stats.setExtraStat("beds_destroyed", safeInt("beds_destroyed"));
                stats.setExtraStat("final_kills", finalKills);
                if (jsonData.contains("prestige") && jsonData["prestige"].is_number_integer())
                {
                    stats.setPrestige(jsonData["prestige"].get<int>());
                }
            }

            if (gameId == "sky")
            {
                int kills = safeInt("kills");
                int deaths = safeInt("deaths");
                int victories = safeInt("victories");
                int played = safeInt("played");
                int xp = safeInt("xp");

                stats.setFKDR(roundToSecond((float)-1.f));
                stats.setKD(roundToSecond(
                    static_cast<float>(safeDivideStat(kills, deaths, static_cast<double>(kills)))));
                stats.setWinRate(std::round(safeDivideStat(victories, played) * 100.0));
                stats.setLevel(getLevelFromXP(xp));
                stats.setPlayed(played);
                stats.setVictories(victories);
                stats.setLosses(played - victories);
                stats.setKills(kills);
                stats.setDeaths(deaths);
                if (jsonData.contains("ores_mined")) stats.setExtraStat(
                    "ores_mined", jsonData["ores_mined"].get<int>());
                if (jsonData.contains("mystery_chests_destroyed")) stats.setExtraStat(
                    "mystery_chests_destroyed", jsonData["mystery_chests_destroyed"].get<int>());
                if (jsonData.contains("spells_used")) stats.setExtraStat(
                    "spells_used", jsonData["spells_used"].get<int>());

                if (jsonData.contains("prestige")) stats.setPrestige(
                    jsonData["prestige"].get<int>());
            }

            if (gameId == "murder")
            {
                int murders = safeInt("murders");
                int eliminations = safeInt("murderer_eliminations");
                int deaths = safeInt("deaths");
                int victories = safeInt("victories");
                int played = safeInt("played");
                int xp = safeInt("xp");

                stats.setFKDR(roundToSecond(-1.0f));
                stats.setKD(roundToSecond(
                    static_cast<float>(
                        safeDivideStat(murders, deaths, static_cast<double>(murders)))));
                stats.setWinRate(std::round(safeDivideStat(victories, played) * 100.0));
                stats.setLevel(getLevelFromXP(xp));
                stats.setPlayed(played);
                stats.setVictories(victories);
                stats.setLosses(played - victories);
                stats.setKills(eliminations);
                stats.setDeaths(deaths);
                stats.setExtraStat("murders", murders);
                stats.setExtraStat("murderer_eliminations", eliminations);
                if (jsonData.contains("coins")) stats.setExtraStat(
                    "coins", jsonData["coins"].get<int>());

                if (jsonData.contains("prestige")) stats.setPrestige(
                    jsonData["prestige"].get<int>());
            }

            if (gameId == "ctf")
            {
                int kills = safeInt("kills");
                int deaths = safeInt("deaths");
                int victories = safeInt("victories");
                int played = safeInt("played");
                int xp = safeInt("xp");

                stats.setKD(roundToSecond(
                    static_cast<float>(safeDivideStat(kills, deaths, static_cast<double>(kills)))));
                stats.setFKDR(roundToSecond(-1.0f));
                stats.setWinRate(std::round(safeDivideStat(victories, played) * 100.0));
                stats.setLevel(getLevelFromXP(xp, 50));
                stats.setPlayed(played);
                stats.setVictories(victories);
                stats.setLosses(played - victories);
                stats.setKills(kills);
                stats.setDeaths(deaths);
                if (jsonData.contains("flags_captured")) stats.setExtraStat(
                    "flags_captured", jsonData["flags_captured"].get<int>());
                if (jsonData.contains("flags_returned")) stats.setExtraStat(
                    "flags_returned", jsonData["flags_returned"].get<int>());
                if (jsonData.contains("assists")) stats.setExtraStat(
                    "assists", jsonData["assists"].get<int>());
            }

            if (gameId == "sg")
            {
                int kills = safeInt("kills");
                int deaths = safeInt("deaths");
                int played = safeInt("played");
                int xp = safeInt("xp");
                int victories = safeInt("victories");

                stats.setFKDR(roundToSecond(-1.0f));
                stats.setKD(roundToSecond(
                    static_cast<float>(safeDivideStat(kills, deaths, static_cast<double>(kills)))));
                stats.setWinRate(std::round(safeDivideStat(victories, played) * 100.0));
                stats.setLevel(getLevelFromXP(xp));
                stats.setPlayed(played);
                stats.setVictories(victories);
                stats.setLosses(played - victories);
                stats.setKills(kills);
                stats.setDeaths(deaths);
                if (jsonData.contains("crates")) stats.setExtraStat(
                    "crates", jsonData["crates"].get<int>());
                if (jsonData.contains("deathmatches")) stats.setExtraStat(
                    "deathmatches", jsonData["deathmatches"].get<int>());
                if (jsonData.contains("cows")) stats.setExtraStat(
                    "cows", jsonData["cows"].get<int>());
                if (jsonData.contains("teleporters_used")) stats.setExtraStat(
                    "teleporters_used", jsonData["teleporters_used"].get<int>());
                if (jsonData.contains("launchpads_used")) stats.setExtraStat(
                    "launchpads_used", jsonData["launchpads_used"].get<int>());
                if (jsonData.contains("flares_used")) stats.setExtraStat(
                    "flares_used", jsonData["flares_used"].get<int>());
            }


            return stats;
        }
        catch (...)
        {
            return stats;
        }
    }
}
