//
// Created by geodan on 10/31/25.
//

#include <iostream>
#include "../include/Controller.h"
#include "../include/Model.h"
#include "../include/View.h"
int main() {
    Model model;
    View view;
    Controller controller(model,view);

    std::cout << "Welcome to the Spotify Playlist Splitter CLI" <<std::endl;
    std::cout <<"This project is currently under construction." <<std::endl;
    std::cout <<"More coming soon" <<std::endl;
    return 0;
}