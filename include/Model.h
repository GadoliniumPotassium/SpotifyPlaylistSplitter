//
// Created by geodan on 10/31/25.
//

#ifndef SPOTIFYPLAYLISTSPLITTER_MODEL_H
#define SPOTIFYPLAYLISTSPLITTER_MODEL_H

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json_fwd.hpp>

class Model {
public:
    Model();

    std::vector<std::string> getLikedSongs();

    void fetchAudioFeatures(const std::vector<std::string> &trackIds);

    std::map<std::string, std::vector<std::string> > categorizeSongs();

    void setAcccessToken(const std::string &token) noexcept { accessToken = token; }

    [[nodiscard]] const nlohmann::json& getLikedSongsData() const noexcept {return likedSongsData;}

    [[nodiscard]] const nlohmann::json& getAudioFeatures() const noexcept { return audioFeaturesData; }

private:
    std::string accessToken;
    nlohmann::json& likedSongsData;
    nlohmann::json& audioFeaturesData;
};


#endif //SPOTIFYPLAYLISTSPLITTER_MODEL_H
