//
// Created by geodan on 10/31/25.
//

#ifndef SPOTIFYPLAYLISTSPLITTER_VIEW_H
#define SPOTIFYPLAYLISTSPLITTER_VIEW_H

#include <map>
#include <string>
#include <vector>

class View {
public:
    void displayGeneratedPlaylists(const std::map<std::string, std::vector<std::string> > &playlists);

    void displayError(const std::string &message);
};

#endif //SPOTIFYPLAYLISTSPLITTER_VIEW_H