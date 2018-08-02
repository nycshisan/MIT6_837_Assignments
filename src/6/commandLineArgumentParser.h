//
// Created by Nycshisan on 2018/7/23.
//

#ifndef ASSIGNMENTS_COMMANDLINEARGUMENTPARSER_H
#define ASSIGNMENTS_COMMANDLINEARGUMENTPARSER_H

class CommandLineArgumentParser {
public:
    char *input_file = nullptr;
    int width = 100;
    int height = 100;
    char *output_file = nullptr;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = nullptr;
    char *normal_file = nullptr;
    bool shade_back = false;
    bool use_gui = false;
    bool cast_shadow = false;
    int max_bounces = 0;
    float cutoff_weight = 0;
    bool visualize_grid = false;
    int grid_nx = 0, grid_ny = 0, grid_nz = 0;

    void parse(int argc, char *argv[]);
};

#endif //ASSIGNMENTS_COMMANDLINEARGUMENTPARSER_H
