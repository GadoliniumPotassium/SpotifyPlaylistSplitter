//
// Created by geodan on 10/31/25.
//

#include "../include/Model.h"

Model::Model() {
}

nlohmann::json Model::fetchPlaylistData() {
    // Load environment variables
    dotenv::init("../.env");
    const std::string clientId = dotenv::getenv("SPOTIFY_CLIENT_ID");
    const std::string clientSecret = dotenv::getenv("SPOTIFY_CLIENT_SECRET");
    const std::string user = dotenv::getenv("SPOTIFY_USER");

    if (clientId.empty() || clientSecret.empty() || user.empty()) {
        throw std::runtime_error("Missing required Spotify credentials or user");
    }

    // Get access token
    const std::string token = getAccessToken(clientId, clientSecret);

    try {
        // Spotify API endpoint
        const std::string url = "https://api.spotify.com/v1/users/" + user + "/playlists?limit=100&offset=5";

        // Make GET request
        const cpr::Response response = cpr::Get(
            cpr::Url{url},
            cpr::Header{
                {"Authorization", "Bearer " + token},
                {"Content-Type", "application/json"}
            },
            cpr::Timeout{30000}
        );

        if (response.status_code != 200) {
            throw std::runtime_error(
                "Spotify API request failed (" + std::to_string(response.status_code) + "): " + response.text
            );
        }

        return nlohmann::json::parse(response.text);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to fetch playlist data: " + std::string(e.what()));
    }
}



nlohmann::json Model::fetchPlaylistSongsData(const std::string &playlistId) {
    // Load environment variables
    dotenv::init("../.env");
    const std::string clientId = dotenv::getenv("SPOTIFY_CLIENT_ID");
    const std::string clientSecret = dotenv::getenv("SPOTIFY_CLIENT_SECRET");
    const std::string user = dotenv::getenv("SPOTIFY_USER");

    if (clientId.empty() || clientSecret.empty() || user.empty()) {
        throw std::runtime_error("Missing required Spotify credentials or user");
    }

    // Get access token
    const std::string token = getAccessToken(clientId, clientSecret);
    nlohmann::json allTracksJson;
    allTracksJson["tracks"] = nlohmann::json::array();

    try {
        // Spotify API endpoint
        nlohmann::json pageJson;
        int offset = 0;
        const std::string url = "https://api.spotify.com/v1/playlists/" + playlistId + "/tracks?limit=50&offset="+std::to_string(offset);

        do{
            // Make GET request
        const cpr::Response response = cpr::Get(
            cpr::Url{url},
            cpr::Header{
                {"Authorization", "Bearer " + token},
                {"Content-Type", "application/json"}
            },
            cpr::Timeout{30000}
        );

        if (response.status_code != 200) {
            throw std::runtime_error(
                "Spotify API request failed (" + std::to_string(response.status_code) + "): " + response.text
            );
        }

        pageJson = nlohmann::json::parse(response.text);

        for(const auto& track:pageJson["items"]){
            allTracksJson["tracks"].push_back(track);
        }
        offset+=50;

        }while(pageJson["items"].size() == 50);

        return allTracksJson;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to fetch playlist data: " + std::string(e.what()));
    }
}

std::vector<nlohmann::json> Model::fetchAudioFeatures(const std::vector<std::string> &trackIds) {
    // Load environment variables
    dotenv::init("../.env");
    const std::string clientId = dotenv::getenv("SPOTIFY_CLIENT_ID");
    const std::string clientSecret = dotenv::getenv("SPOTIFY_CLIENT_SECRET");
    const std::string user = dotenv::getenv("SPOTIFY_USER");

    if (clientId.empty() || clientSecret.empty() || user.empty()) {
        throw std::runtime_error("Missing required Spotify credentials or user");
    }
    // Get access token
    const std::string token = getAccessToken(clientId, clientSecret);

    std::vector<nlohmann::json> audioFeatures;
    for (auto id: trackIds) {
        try {
            const std::string url = "https://api.spotify.com/v1/audio-features/"+id;
            // Make GET request
            const cpr::Response response = cpr::Get(
                cpr::Url{url},
                cpr::Header{
                    {"Authorization", "Bearer " + token},
                    {"Content-Type", "application/json"}
                },
                cpr::Timeout{30000}
            );

            if (response.status_code != 200) {
                throw std::runtime_error(
                    "Spotify API request failed (" + std::to_string(response.status_code) + "): " + response.text
                );
            }
            audioFeatures.push_back(nlohmann::json::parse(response.text));
        }catch (const std::exception& e) {
            throw std::runtime_error("Failed to fetch track data: " + std::string(e.what()));
        }
    }
    return audioFeatures;
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

// Simple Base64 encoding function (sufficient for client credentials)
static std::string base64Encode(const std::string& input) {
    static const char lookup[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string encoded;
    int val = 0;
    int valb = -6;

    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(lookup[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6)
        encoded.push_back(lookup[((val << 8) >> (valb + 8)) & 0x3F]);

    while (encoded.size() % 4)
        encoded.push_back('=');

    return encoded;
}

std::string Model::getAccessToken(const std::string& clientId, const std::string& clientSecret) {
    // Spotify token endpoint
    const std::string url = "https://accounts.spotify.com/api/token";

    // Body for client credentials flow
    const std::string body = "grant_type=client_credentials";

    // Basic authorization header (base64 of client_id:client_secret)
    const std::string credentials = clientId + ":" + clientSecret;
    const std::string encoded = base64Encode(credentials);

    // Make POST request
    const cpr::Response response = cpr::Post(
        cpr::Url{url},
        cpr::Header{
            {"Authorization", "Basic " + encoded},
            {"Content-Type", "application/x-www-form-urlencoded"}
        },
        cpr::Body{body},
        cpr::Timeout{15000}
    );

    if (response.status_code != 200) {
        throw std::runtime_error(
            "Failed to obtain access token (" + std::to_string(response.status_code) + "): " + response.text
        );
    }

    // Parse token from JSON
    const nlohmann::json json = nlohmann::json::parse(response.text);
    if (!json.contains("access_token")) {
        throw std::runtime_error("Access token missing in response: " + response.text);
    }

    return json["access_token"].get<std::string>();
}

