//
// Created by geodan on 10/31/25.
//

#include "../include/Model.h"

nlohmann::json Model::fetchPlaylistData() {
    std::string token = getAccessToken(std::dotenv(SPOTIFY_CLIENT_ID), std::dotenv(SPOTIFY_CLIENT_SECRET));

    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL");
    }

    std::string readBuffer;
    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());

    try {
        std::string url = "https://api.spotify.com/v1/users/"+std::dotenv(USER)+"/playlists";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("cURL request failed: " + std::string(curl_easy_strerror(res)));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        return nlohmann::json::parse(readBuffer);
    }
    catch (...) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        throw;
    }
}


nlohmann::json &Model::fetchPlaylistSongsData(const std::string &playlistId) {
    std::string token = getAccessToken(std::dotenv(SPOTIFY_CLIENT_ID), std::dotenv(SPOTIFY_CLIENT_SECRET));

    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL");
    }

    std::string readBuffer;
    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());
    try {
        std::string url = "https://api.spotify.com/v1/playlists/"+playlistId+"/tracks?limit=50";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("cURL request failed: " + std::string(curl_easy_strerror(res)));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        return nlohmann::json::parse(readBuffer);

    }catch (...) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        throw;
    }
}

void Model::fetchAudioFeatures(const std::vector<std::string> &trackIds) {
}

std::map<std::string, std::vector<std::string> > Model::categorizeSongs() {
}

size_t Model::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
        return newLength;
    }
    catch(std::bad_alloc &e) {
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
    auto writeCallback = [](void* contents, size_t size, size_t nmemb, std::string* s) -> size_t {
        size_t newLength = size * nmemb;
        try {
            s->append((char*)contents, newLength);
            return newLength;
        }
        catch(std::bad_alloc &e) {
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
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse access token response: " + std::string(e.what()));
    }
}

