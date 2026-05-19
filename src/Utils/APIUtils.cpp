#include "APIUtils.hpp"

#include <json/json.hpp>

#include <wininet.h>
#include <Utils/Utils.hpp>
#include <Utils/Logger/Logger.hpp>

#include <miniz/miniz.h>
#include <curl/curl/curl.h>
#include <curl/curl/easy.h>

#include <chrono>
#include <mutex>
#include <thread>

#include "SDK/SDK.hpp"

namespace {
struct VIPStreamState {
    std::string message;
};

void ensureCurlInitialized() {
    static std::once_flag curlInitFlag;
    std::call_once(curlInitFlag, []() {
        if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
            LOG_ERROR("failed curl_global_init");
        }
    });
}

int VIPStreamProgressCallback(void*, curl_off_t, curl_off_t, curl_off_t, curl_off_t) {
    return APIUtils::vipStreamRunning ? 0 : 1;
}

void handleVIPWebSocketMessage(const std::string& message) {
    try {
        if (!nlohmann::json::accept(message)) {
            return;
        }

        auto eventJson = nlohmann::json::parse(message);
        if (eventJson.value("type", "") == "vip_update" && eventJson.contains("vips")) {
            APIUtils::applyVips(eventJson["vips"]);
        }
    } catch (const std::exception& e) {
        Logger::warn("VIP websocket message rejected: {}", e.what());
    }
}
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::pair<long, std::string> APIUtils::Request(
    const std::string& url,
    const std::string& method,
    const std::string& data,
    struct curl_slist* headers
) {
    long responseCode = 0;
    std::string responseBody;

    static bool curlInitialized = false;
    if (!curlInitialized) {
        if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
            LOG_ERROR("failed curl_global_init");
            return { 0, "" };
        }
        curlInitialized = true;
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        LOG_ERROR("failed curl_easy_init");
        return { 0, "" };
    }

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl");
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");

    // Ensure headers are initialized
    if (!headers) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 10 second timeout
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L); // 5 second connection timeout

    if (method == "GET") {
        // No need for CURLOPT_CUSTOMREQUEST, GET is default
    }
    else if (method == "POST") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    }
    else if (method == "PUT") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    }
    else if (method == "DELETE") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    }
    else {
        LOG_ERROR("invalid http method: {}", method);
        curl_easy_cleanup(curl);
        return { 0, "" };
    }

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        LOG_ERROR("failed curl request: {}", curl_easy_strerror(res));
        responseBody = "[ERROR] cURL failed";
    }
    else {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    }

    // Free headers only if they were newly created inside this function
    if (headers) {
        curl_slist_free_all(headers);
    }

    curl_easy_cleanup(curl);
    return { responseCode, responseBody };
}

std::pair<long, std::string> APIUtils::POST_Simple(const std::string& url, const std::string& postData) {
    long responseCode = 0;
    std::string responseBody;

    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        LOG_ERROR("curl_global_init failed in POST_SIMPLE");
        return {0, ""};
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        LOG_ERROR("curl_easy_init failed in POST_SIMPLE");
        curl_global_cleanup();
        return {0, ""};
    }

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Samsung Smart Fridge");

    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 10 second timeout
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L); // 5 second connection timeout

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        LOG_ERROR("curl_easy_perform failed in POST_SIMPLE: {}", curl_easy_strerror(res));
    } else {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return {responseCode, responseBody};
}

std::string APIUtils::legacyGet(const std::string &URL) {
    try {
        HINTERNET interwebs = InternetOpenA("Samsung Smart Fridge", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!interwebs) {
            return "";
        }

        std::string rtn;
        HINTERNET urlFile = InternetOpenUrlA(interwebs, URL.c_str(), "Content-Type: application/json\r\n", -1, INTERNET_FLAG_RELOAD, 0);
        if (urlFile) {
            char buffer[2000];
            DWORD bytesRead;
            while (InternetReadFile(urlFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
                rtn.append(buffer, bytesRead);
            }
            InternetCloseHandle(urlFile);
        }

        InternetCloseHandle(interwebs);
        return String::replaceAll(rtn, "|n", "\r\n");
    } catch (const std::exception &e) {
        LOG_ERROR(e.what());
    }
    return "";
}

     std::string APIUtils::get(const std::string &link) {
        try {
            HINTERNET interwebs = InternetOpenA("curl", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
            if (!interwebs) {
                return "";
            }

            DWORD http_decoding = TRUE;
            if (InternetSetOption(interwebs, INTERNET_OPTION_HTTP_DECODING, &http_decoding, sizeof(http_decoding)) == FALSE) {
                InternetCloseHandle(interwebs);
                LOG_ERROR("InternetSetOption(INTERNET_OPTION_HTTP_DECODING) failed");
                return "";
            }


            std::string rtn;
            HINTERNET urlFile = InternetOpenUrlA(interwebs, link.c_str(), "Accept-Encoding: gzip\r\nUser-Agent: Samsung Smart Fridge\r\nContent-Type: application/json\r\nshould-compress: 1\r\n", -1, INTERNET_FLAG_RELOAD, 0);
            if (urlFile) {
                char buffer[2000];
                DWORD bytesRead;
                std::stringstream compressedData;


                while (InternetReadFile(urlFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
                    compressedData.write(buffer, bytesRead);
                }


                char encodingBuffer[256];
                DWORD encodingBufferSize = sizeof(encodingBuffer);
                bool isGzipEncoded = false;
                if (HttpQueryInfoA(urlFile, HTTP_QUERY_CONTENT_ENCODING, &encodingBuffer, &encodingBufferSize, NULL)) {
                    std::string contentEncoding(encodingBuffer, encodingBufferSize > 0 ? encodingBufferSize -1 : 0);
                    if (String::replaceAll(contentEncoding, " ", "") == "gzip") {
                        isGzipEncoded = true;
                    }
                } else {
                    // hi
                    DWORD lastError = GetLastError();
                    //LOG_ERROR("HttpQueryInfoA(HTTP_QUERY_CONTENT_ENCODING) failed, assuming no gzip or plain text. LastError: " + FlarialGUI::cached_to_string(lastError));
                }


                InternetCloseHandle(urlFile);

                std::string compressedString = compressedData.str();


                if (isGzipEncoded && !compressedString.empty()) {
                    // Decompress using miniz

                    size_t uncompressedSizeGuess = compressedString.length() * 5;
                    std::vector<unsigned char> uncompressedBuffer(uncompressedSizeGuess);
                    mz_ulong finalUncompressedSize = static_cast<mz_ulong>(uncompressedSizeGuess);

                    int status = mz_uncompress(uncompressedBuffer.data(), &finalUncompressedSize, (const unsigned char*)compressedString.data(), compressedString.length());
                    if (status == Z_OK) {
                        rtn = std::string(reinterpret_cast<const char*>(uncompressedBuffer.data()), finalUncompressedSize);
                    } else {
                        InternetCloseHandle(interwebs);
                        LOG_ERROR("mz_uncompress failed with status: " + FlarialGUI::cached_to_string(status));
                        return ""; // Decompression failed
                    }
                } else {
                    rtn = compressedString;
                }


            } else {
                InternetCloseHandle(interwebs);
                return "";
            }

            InternetCloseHandle(interwebs);
            return String::replaceAll(rtn, "|n", "\r\n");
        } catch (const std::exception &e) {
            LOG_ERROR(e.what());
        }
        return "";
    }

nlohmann::json APIUtils::getVips() {
    try {
        std::string users = get("https://api.flarial.xyz/vips");

        if (users.empty()) {
            Logger::warn("Unable to fetch vips, API is down or you are not connected to the internet.");
            return nlohmann::json::object();
        }

        if (nlohmann::json::accept(users)) {
            return nlohmann::json::parse(users);
        }

        Logger::warn("VIP JSON rejected: {}", users);
        return nlohmann::json::object();
    }
    catch (const nlohmann::json::parse_error& e) {
        LOG_ERROR("An error occurred while parsing vip users: {}", e.what());
        return nlohmann::json::object();
    }
    catch (const std::exception& e) {
        LOG_ERROR("An unexpected error occurred: {}", e.what());
        return nlohmann::json::object();
    }
}

void APIUtils::applyVips(const nlohmann::json& vipsJson) {
    decltype(vipUserToRole) updatedVips;

    for (const auto& [role, users] : vipsJson.items()) {
        if (!users.is_array()) {
            continue;
        }

        for (const auto& user : users) {
            if (user.is_string()) {
                updatedVips[user.get<std::string>()] = role;
            }
        }
    }

    std::unique_lock lock(rolesMutex);
    vipUserToRole = std::move(updatedVips);
}

void APIUtils::startVipUpdates() {
    bool expected = false;
    if (!vipStreamRunning.compare_exchange_strong(expected, true)) {
        return;
    }

    vipStreamThread = std::thread([]() {
        ensureCurlInitialized();

        while (vipStreamRunning) {
            CURL* curl = curl_easy_init();
            if (!curl) {
                Logger::warn("VIP stream failed to initialize curl");
                std::this_thread::sleep_for(std::chrono::seconds(15));
                continue;
            }

            curl_easy_setopt(curl, CURLOPT_URL, "wss://api.flarial.xyz/ws/vips");
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Samsung Smart Fridge");
            // curl 8.x defaults CURLOPT_PROTOCOLS to HTTP/HTTPS/FTP/FTPS only — ws/wss must be opted in
            curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "wss,ws");
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
            curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
            curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 90L);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, VIPStreamProgressCallback);
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);

            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                char buffer[65536];
                VIPStreamState state;

                while (vipStreamRunning) {
                    size_t received = 0;
                    const curl_ws_frame* frame = nullptr;
                    res = curl_ws_recv(curl, buffer, sizeof(buffer), &received, &frame);

                    // CONNECT_ONLY=2 leaves the socket non-blocking; CURLE_AGAIN just means
                    // no frame has arrived yet. Idle until the next read instead of reconnecting.
                    if (res == CURLE_AGAIN) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(250));
                        continue;
                    }

                    if (res != CURLE_OK) {
                        break;
                    }

                    if (!frame || received == 0) {
                        continue;
                    }

                    if ((frame->flags & CURLWS_CLOSE) != 0) {
                        break;
                    }

                    if ((frame->flags & CURLWS_TEXT) == 0 && (frame->flags & CURLWS_CONT) == 0) {
                        continue;
                    }

                    state.message.append(buffer, received);
                    if (frame->bytesleft == 0) {
                        handleVIPWebSocketMessage(state.message);
                        state.message.clear();
                    }
                }
            }

            curl_easy_cleanup(curl);

            if (!vipStreamRunning) {
                break;
            }

            if (res != CURLE_OK && res != CURLE_ABORTED_BY_CALLBACK) {
                Logger::debug("VIP stream disconnected: {}", curl_easy_strerror(res));
            }

            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    });
}

void APIUtils::stopVipUpdates() {
    vipStreamRunning = false;
    if (vipStreamThread.joinable()) {
        vipStreamThread.join();
    }
}

nlohmann::json APIUtils::getUsers() {
    try {
        std::string users = get("https://api.flarial.xyz/allOnlineUsers");

        if (users.empty()) {
            Logger::warn("Unable to fetch users, API is down or you are not connected to the internet.");
            return nlohmann::json::object();
        }

        if (nlohmann::json::accept(users)) {
            return nlohmann::json::parse(users);
        }

        Logger::warn("Users JSON rejected: {}", users);
        return nlohmann::json::object();
    }
    catch (const nlohmann::json::parse_error& e) {
        LOG_ERROR("An error occurred while parsing online users: {}", e.what());
        return nlohmann::json::object();
    }
    catch (const std::exception& e) {
        LOG_ERROR("An unexpected error occurred: {}", e.what());
        return nlohmann::json::object();
    }
}

bool APIUtils::hasRole(std::string_view role, std::string_view name) {
    std::shared_lock lock(rolesMutex);

    const auto vipIt = vipUserToRole.find(name);
    const auto isVip = (vipIt != vipUserToRole.cend()) && (vipIt->second == role);

    if (isVip) {
        return true;
    }

    const auto isOnline = onlineUsersSet.contains(name);
    return isOnline && (role == "Regular");
}

bool APIUtils::isOnlineUser(std::string_view name) {
    std::shared_lock lock(rolesMutex);
    return onlineUsersSet.contains(name);
}

std::vector<std::string> APIUtils::getOnlineUsersSnapshot() {
    std::shared_lock lock(rolesMutex);
    return onlineUsers;
}

std::vector<std::string> APIUtils::ListToVector(const std::string& commandListStr) {
    std::vector<std::string> commands;
    std::stringstream ss(commandListStr);
    char delimiter = ',';
    std::string segment;

    std::string trimmedStr = commandListStr;
    std::string prefix = "{\"players\": [";
    std::string suffix = "]}";

    if (trimmedStr.rfind(prefix, 0) == 0) {
        trimmedStr.erase(0, prefix.length());
    }

    if (trimmedStr.rfind(suffix) == trimmedStr.length() - suffix.length()) {
        trimmedStr.erase(trimmedStr.length() - suffix.length());
    }

    std::stringstream trimmed_ss(trimmedStr);


    while (std::getline(trimmed_ss, segment, delimiter)) {
        std::string trimmedSegment = segment;

        trimmedSegment.erase(trimmedSegment.begin(), std::find_if(trimmedSegment.begin(), trimmedSegment.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        trimmedSegment.erase(std::find_if(trimmedSegment.rbegin(), trimmedSegment.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), trimmedSegment.end());


        if (trimmedSegment.length() >= 2 && trimmedSegment.front() == '"' && trimmedSegment.back() == '"') {
            trimmedSegment = trimmedSegment.substr(1, trimmedSegment.length() - 2);
        }

        if (!trimmedSegment.empty()) {
            commands.push_back(trimmedSegment);
        }
    }
    return commands;
}


std::string APIUtils::VectorToList(const std::vector<std::string>& vec) {
    std::stringstream ss;
    ss << "{\"players\": [";

    for (size_t i = 0; i < vec.size(); ++i) {
        ss << std::quoted(vec[i]);
        if (i < vec.size() - 1) {
            ss << ',';
        }
    }

    ss << "]}";
    return ss.str();
}


std::vector<std::string> APIUtils::UpdateVector(
    const std::vector<std::string>& currentVec,
    const std::vector<std::string>& commands, std::string localPlayerName) {

    std::vector<std::string> result = currentVec;

    for (const auto &command: commands) {
        if (command.empty()) {
            continue;
        }

        char prefix = command[0];
        std::string item = command.substr(1);

        if (prefix == '+') {
            if (std::find(result.begin(), result.end(), item) == result.end()) {
                result.push_back(item);
            }
        } else if (prefix == '-') {
            auto it = std::find(result.begin(), result.end(), item);
            if (it != result.end()) {
                result.erase(it);
            }
        }
    }

    try {
        std::string name = localPlayerName;
        std::string clearedName = String::removeNonAlphanumeric(String::removeColorCodes(name));

        if (clearedName.empty()) {
            clearedName = String::removeColorCodes(name);
        }

        if (std::find(result.begin(), result.end(), clearedName) == result.end()) {
            result.push_back(clearedName);
        }
    } catch (const std::exception &e) {
        LOG_ERROR("Error processing local player name: {}", e.what());
    }

    return result;
}

std::vector<std::string> APIUtils::UpdateVector(
    const std::vector<std::string>& currentVec,
    const std::string& commandListStr, std::string localPlayerName) {

    std::vector<std::string> commands = ListToVector(commandListStr);
    return UpdateVector(currentVec, commands, localPlayerName);
}

std::vector<std::string> APIUtils::UpdateVectorFast(
    const std::vector<std::string>& currentVec,
    const std::vector<std::string>& commands) {

    std::unordered_set<std::string> itemSet(currentVec.begin(), currentVec.end());

    for (const auto& command : commands) {
        if (command.empty()) {
            continue;
        }

        char prefix = command[0];
        std::string item = command.substr(1);

        if (prefix == '+') {
            itemSet.insert(item);
        }
        else if (prefix == '-') {
            itemSet.erase(item);
        }
    }

    return std::vector<std::string>(itemSet.begin(), itemSet.end());
}
