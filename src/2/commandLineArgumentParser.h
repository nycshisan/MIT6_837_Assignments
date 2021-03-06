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

    void parse(int argc, char *argv[]);
};

#endif //ASSIGNMENTS_COMMANDLINEARGUMENTPARSER_H
