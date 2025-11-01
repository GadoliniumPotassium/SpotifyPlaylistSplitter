//
// Created by geodan on 10/31/25.
//

#ifndef SPOTIFYPLAYLISTSPLITTER_CONTROLLER_H
#define SPOTIFYPLAYLISTSPLITTER_CONTROLLER_H

#include "Model.h"
#include "View.h"

class Controller {
public:
    Controller(Model &model, View &view);

    void run();

private:
    Model &model;
    View &view;
};


#endif //SPOTIFYPLAYLISTSPLITTER_CONTROLLER_H
