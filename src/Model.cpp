//
// Created by geodan on 10/31/25.
//

#include "../include/Model.h"

Model::Model() {
}

// Improved error handling and resource management
nlohmann::json Model::fetchPlaylistData() {
    // Validate environment variables first
    const std::string clientId = dotenv::getenv("SPOTIFY_CLIENT_ID");
    const std::string clientSecret = dotenv::getenv("SPOTIFY_CLIENT_SECRET");
    const std::string user = dotenv::getenv("USER");

    std::cout << clientId << " --" << clientSecret << " // " << user << std::endl;

    if (clientId.empty() || clientSecret.empty() || user.empty()) {
        throw std::runtime_error("Missing required Spotify credentials or user");
    }

    std::string token = getAccessToken(clientId, clientSecret, "", "");

    CURL *curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL");
    }

    std::string readBuffer;
    curl_slist *headers = nullptr;

    try {
        // Add error checking for header creation
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());
        if (!headers) {
            throw std::runtime_error("Failed to create cURL headers");
        }

        std::string url = "https://api.spotify.com/v1/users/" + user + "/playlists";

        // Additional cURL options for robustness
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L); // Fail on HTTP errors
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L); // 30-second timeout

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("cURL request failed: " + std::string(curl_easy_strerror(res)));
        }

        // Cleanup before parsing
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        // Parse and return JSON
        return nlohmann::json::parse(readBuffer);
    } catch (...) {
        // Ensure cleanup in all error scenarios
        if (headers) curl_slist_free_all(headers);
        if (curl) curl_easy_cleanup(curl);
        throw; // Re-throw the original exception
    }
}


nlohmann::json Model::fetchPlaylistSongsData(const std::string &playlistId) {
// Validate environment variables first
    const std::string clientId = dotenv::getenv("SPOTIFY_CLIENT_ID");
    const std::string clientSecret = dotenv::getenv("SPOTIFY_CLIENT_SECRET");
    const std::string user = dotenv::getenv("USER");

    std::cout << clientId << " --" << clientSecret << " // " << user << std::endl;

    if (clientId.empty() || clientSecret.empty() || user.empty()) {
        throw std::runtime_error("Missing required Spotify credentials or user");
    }

    std::string token = getAccessToken(clientId, clientSecret, "", "");

    CURL *curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL");
    }

    std::string readBuffer;
    curl_slist *headers = nullptr;

    try {
        // Add error checking for header creation
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());
        if (!headers) {
            throw std::runtime_error("Failed to create cURL headers");
        }

        std::string url = "https://api.spotify.com/v1/playlists/" + playlistId + "/tracks?limit=50";

        // Additional cURL options for robustness
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L); // Fail on HTTP errors
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L); // 30-second timeout

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("cURL request failed: " + std::string(curl_easy_strerror(res)));
        }

        // Cleanup before parsing
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        // Parse and return JSON
        return nlohmann::json::parse(readBuffer);
    } catch (...) {
        // Ensure cleanup in all error scenarios
        if (headers) curl_slist_free_all(headers);
        if (curl) curl_easy_cleanup(curl);
        throw; // Re-throw the original exception
    }
}

void Model::fetchAudioFeatures(const std::vector<std::string> &trackIds) {
}

std::map<std::string, std::vector<std::string> > Model::categorizeSongs() {
}

size_t Model::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char *) contents, newLength);
        return newLength;
    } catch (std::bad_alloc &e) {
        return 0;
    }
}

std::string Model::getAccessToken(const std::string &clientId, const std::string &clientSecret,
                                  const std::string &code = "",
                                  const std::string &redirectUri = "") {
    CURL *curl = nullptr;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL");
    }

    // Prepare POST fields based on the authentication method
    std::string postFields;
    std::string url;

    // Check if we're doing authorization code flow or client credentials flow
    if (!code.empty() && !redirectUri.empty()) {
        // Authorization Code Flow
        url = "https://accounts.spotify.com/api/token";
        postFields = "grant_type=authorization_code&code=" + code +
                     "&redirect_uri=" + redirectUri +
                     "&client_id=" + clientId +
                     "&client_secret=" + clientSecret;
    } else {
        // Client Credentials Flow
        url = "https://accounts.spotify.com/api/token";
        postFields = "grant_type=client_credentials&client_id=" + clientId +
                     "&client_secret=" + clientSecret;
    }

    // Callback to store response
    auto writeCallback = [](void *contents, size_t size, size_t nmemb, std::string *s) -> size_t {
        size_t newLength = size * nmemb;
        try {
            s->append((char *) contents, newLength);
            return newLength;
        } catch (std::bad_alloc &e) {
            return 0;
        }
    };

    // Set cURL options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    // Perform the request
    res = curl_easy_perform(curl);

    // Clean up
    curl_easy_cleanup(curl);

    // Check for errors
    if (res != CURLE_OK) {
        throw std::runtime_error("cURL request failed: " + std::string(curl_easy_strerror(res)));
    }

    // Parse and return the token from the response
    try {
        const auto jsonResponse = nlohmann::json::parse(readBuffer);
        return jsonResponse.value("access_token", "");
    } catch (const std::exception &e) {
        throw std::runtime_error("Failed to parse access token response: " + std::string(e.what()));
    }
}

