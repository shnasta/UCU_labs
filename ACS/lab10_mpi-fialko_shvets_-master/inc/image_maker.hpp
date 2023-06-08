// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef MPI_IMAGE_MAKER_H
#define MPI_IMAGE_MAKER_H

#include <vector>
#include <string>

class image_maker {
private:
    int width;
    int height;
    int num_frames = 0;
    std::vector<std::string> images;

public:
    image_maker(int height, int width, int num_frames);
    void write_image(const std::string& filename, const std::vector<double>& table);
    void write_gif(const std::string& filename);
};


#endif //MPI_IMAGE_MAKER_H
