// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <mpi.h>
#include "parse_config.hpp"

#include "parse_table.hpp"
#include "table_manager.hpp"
#include "image_maker.hpp"
#include "image_service.hpp"

void parts_combiner(image_service& img_service, int worldSize, int num_iter) {

    for (int j = 0; j < num_iter; ++j) {
        std::vector<double> buf(img_service.get_height() * img_service.get_width());
        auto total_count = 0;
        for (int i = 0; i < worldSize; ++i) {
            MPI_Status status;
            int count;
            MPI_Probe(i, j, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_DOUBLE, &count);
            MPI_Recv(buf.data() + total_count, count, MPI_DOUBLE, i, j, MPI_COMM_WORLD, &status);
            total_count += count;
        }
        img_service.add_frame(buf);
    }
}

void img_generator(image_service& img_service, int num_iter) {
    for (int i = 0; i < num_iter; ++i) {
        img_service.get_frame(i);
    }
    img_service.write_gif("./img/animation.gif");
}



int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int worldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    if (worldSize < 3) {
        std::cerr << "Error: at least 3 processes are required." << std::endl;
        return 2;
    }
    int myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    std::string filename;
    if (argc < 2) {
        std::cerr << "Error: config path is not specified." << std::endl;
        return 1;
    } else {
        filename = argv[1];
    }

    std::ifstream config_stream(filename);
    if (!config_stream.is_open()) {
        std::cerr << "Failed to open configuration file " << filename << std::endl;
        return 3;
    }

    std::unique_ptr<parse_config> config_ptr;
    try {
        config_ptr = std::make_unique<parse_config>(config_stream);
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 5;
    }

    auto width = config_ptr->width;
    auto height = config_ptr->height;
    auto num_iters = config_ptr->num_iters;
    auto iter_gif = config_ptr->iter_gif;

    auto p = parse_table(config_ptr->width, config_ptr->height);
    p.read_table(config_ptr->first_state_path);

    if (myRank != worldSize - 1) {

        auto start_table = p.get_part(myRank, worldSize - 1);
        height = p.get_part_height(myRank, worldSize - 1);
        config_ptr->height = height;
        table_manager table(config_ptr, start_table);

        for (int i = 0; i < num_iters; ++i) {

            table.make_step();
            // send to img service
            if (i % iter_gif == 0) {
                auto starting_point = myRank ? 1 : 0;
                auto pure_height = myRank == worldSize - 2 || myRank == 0 ? height - 1 : height - 2;
                MPI_Send(table.get_content_list().data() + starting_point * width,
                         width * pure_height, MPI_DOUBLE, worldSize - 1, i / iter_gif, MPI_COMM_WORLD);
            }

            // send overlapping lines to neighbours
            if (myRank < worldSize - 2) {
                MPI_Send(table.get_pre_last_line().data(), width, MPI_DOUBLE, myRank + 1, i, MPI_COMM_WORLD);
            }
            if (myRank > 0) {
                std::vector<double> first_line(width);
                MPI_Recv(first_line.data(), width, MPI_DOUBLE, myRank - 1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                table.set_first_line(first_line);
            }
            if (myRank > 0) {
                MPI_Send(table.get_second_line().data(), width, MPI_DOUBLE, myRank - 1, i, MPI_COMM_WORLD);
            }
            // receive overlapping lines from neighbours
            if (myRank < worldSize - 2) {
                std::vector<double> last_line(width);
                MPI_Recv(last_line.data(), width, MPI_DOUBLE, myRank + 1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                table.set_last_line(last_line);
            }

        }


    }

    if (myRank == worldSize - 1) {
        image_service img_service(config_ptr->height, config_ptr->width, config_ptr->num_iters / config_ptr->iter_gif, p.get_table());
        std::thread receiver(parts_combiner, std::ref(img_service), worldSize - 1, config_ptr->num_iters / config_ptr->iter_gif);
        std::thread generator(img_generator, std::ref(img_service), config_ptr->num_iters / config_ptr->iter_gif);
        receiver.join();
        generator.join();
    }

    MPI_Finalize();
    return 0;
}
