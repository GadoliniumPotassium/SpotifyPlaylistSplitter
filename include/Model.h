//
// Created by geodan on 10/31/25.
//

#ifndef SPOTIFYPLAYLISTSPLITTER_MODEL_H
#define SPOTIFYPLAYLISTSPLITTER_MODEL_H

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <laserpants/dotenv-0.9.3/dotenv.h>
#include <curl/curl.h>

/**
 * @brief The Model class handles the interactions with Spotify playlists.
 *
 * This class provides functionalities to fetch playlists, retrieve songs from a
 * selected playlist, obtain audio features for the tracks, and categorize songs
 * based on their characteristics.
 */
class Model {
public:
    Model();

    /**
     * @brief Fetches all playlists followed or created by a particular account.
     *
     * @return A reference to a JSON object containing playlist data.
     */
    nlohmann::json fetchPlaylistData();

    /**
     * @brief Pulls the songs from the selected playlist.
     *
     * @param playlistId The ID of the playlist from which to fetch songs.
     * @return A reference to a JSON object containing songs data of the specified playlist.
     */
    nlohmann::json fetchPlaylistSongsData(const std::string &playlistId);

    /**
     * @brief Loops through the retrieved songs and gathers their audio features.
     *
     * @param trackIds A vector of track IDs for which to fetch audio features.
     */
    void fetchAudioFeatures(const std::vector<std::string> &trackIds);

    /**
     * @brief Categorizes the songs based on predetermined vibes.
     *
     * @return A map where keys are category names and values are vectors of song IDs.
     */
    std::map<std::string, std::vector<std::string> > categorizeSongs();

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s);

    std::string getAccessToken(const std::string &clientId, const std::string &clientSecret, const std::string &code,
                               const std::string &redirectUri);
};

#endif //SPOTIFYPLAYLISTSPLITTER_MODEL_H
