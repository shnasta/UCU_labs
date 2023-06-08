// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "image_service.hpp"

image_service::image_service(int height, int width, int num_frames, const std::vector<double>& initial_frame) :
    height(height), width(width), img_maker(image_maker(height, width, num_frames)) {
    frames_queue.set_capacity(num_frames);
    frames_queue.push(initial_frame);
};

void image_service::add_frame(const std::vector<double>& frame) {
    frames_queue.push(frame);
}

int image_service::get_height() {
    return height;
}

int image_service::get_width() {
    return width;
}

void image_service::get_frame(int i) {
    std::vector<double> frame;
    frames_queue.pop(frame);
    img_maker.write_image("./img/"+std::to_string(i)+".png", frame);
}

void image_service::write_gif(const std::string &filename) {
    img_maker.write_gif(filename);
}
