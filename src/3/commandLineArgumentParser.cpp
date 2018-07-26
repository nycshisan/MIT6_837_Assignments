//
// Created by Nycshisan on 2018/7/23.
//

#include "commandLineArgumentParser.h"

#include <cstring>
#include <cassert>
#include <cstdio>

/* Sphere arguments */
extern int g_theta_steps, g_phi_steps;
extern bool g_gouraud;

void CommandLineArgumentParser::parse(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc);
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc);
            width = atoi(argv[i]);
            i++; assert (i < argc);
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc);
            output_file = argv[i];
        } else if (!strcmp(argv[i],"-depth")) {
            i++; assert (i < argc);
            depth_min = atof(argv[i]);
            i++; assert (i < argc);
            depth_max = atof(argv[i]);
            i++; assert (i < argc);
            depth_file = argv[i];
        } else if (!strcmp(argv[i],"-normals")) {
            i++; assert(i < argc);
            normal_file = argv[i];
        } else if (!strcmp(argv[i],"-shade_back")) {
            shade_back = true;
        } else if (!strcmp(argv[i],"-gui")) {
            use_gui = true;
        } else if (!strcmp(argv[i],"-tessellation")) {
            i++; assert(i < argc);
            g_theta_steps = atoi(argv[i]);
            i++; assert(i < argc);
            g_phi_steps = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-gouraud")) {
            g_gouraud = true;
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

}
