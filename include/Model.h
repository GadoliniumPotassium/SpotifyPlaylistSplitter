//
// Created by geodan on 10/31/25.
//

#ifndef SPOTIFYPLAYLISTSPLITTER_MODEL_H
#define SPOTIFYPLAYLISTSPLITTER_MODEL_H

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json_fwd.hpp>
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

    /**
     * @brief Fetches all playlists followed or created by a particular account.
     *
     * @return A reference to a JSON object containing playlist data.
     */
    nlohmann::json &fetchPlaylistData();

    /**
     * @brief Pulls the songs from the selected playlist.
     *
     * @param playlistId The ID of the playlist from which to fetch songs.
     * @return A reference to a JSON object containing songs data of the specified playlist.
     */
    nlohmann::json &fetchPlaylistSongsData(const std::string &playlistId);

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
    std::map<std::string, std::vector<std::string>> categorizeSongs();

    /**
     * @brief Getter for the liked songs data.
     *
     * @return A constant reference to the JSON object containing liked songs data.
     */
    [[nodiscard]] const nlohmann::json &getPlaylistSongsData() const noexcept { return likedSongsData; }

    /**
     * @brief Getter for the audio features data.
     *
     * @return A constant reference to the JSON object containing audio features data.
     */
    [[nodiscard]] const nlohmann::json &getAudioFeatures() const noexcept { return audioFeaturesData; }

    /**
     * @brief Getter for the playlist data.
     *
     * @return A constant reference to the JSON object containing playlist data.
     */
    [[nodiscard]] const nlohmann::json &getPlaylistData() const noexcept { return playlistData; }

private:
    const std::string CLIENT_ID;         ///< The client ID for Spotify API authentication.
    const std::string CLIENT_SECRET;     ///< The client secret for Spotify API authentication.
    nlohmann::json &likedSongsData;      ///< JSON object holding data for liked songs.
    nlohmann::json &audioFeaturesData;   ///< JSON object holding audio features data.
    nlohmann::json &playlistData;        ///< JSON object holding playlist data.
};

#endif //SPOTIFYPLAYLISTSPLITTER_MODEL_H
