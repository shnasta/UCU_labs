// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef MPI_IMAGE_SERVICE_HPP
#define MPI_IMAGE_SERVICE_HPP

#include <oneapi/tbb/concurrent_queue.h>
#include "image_maker.hpp"




class image_service {
private:
    int height, width;
    oneapi::tbb::concurrent_bounded_queue<std::vector<double>> frames_queue;
    image_maker img_maker;
public:
    image_service(int height, int width, int num_frames, const std::vector<double>& initial_frame);
    void add_frame(const std::vector<double>& frame);
    void get_frame(int i);
    void combine_parts(std::vector<std::vector<double>> parts);
    int get_height();
    int get_width();
    void generate_img();
    void write_gif(const std::string& filename);
};


#endif //MPI_IMAGE_SERVICE_HPP
