//
// Created by Nycshisan on 2018/7/23.
//

#include "commandLineArgumentParser.h"

#include <cstring>
#include <cassert>
#include <cstdio>

#define NEXT_ARG(STR) (!strcmp(argv[i], STR))

/* Sphere arguments */
extern int g_theta_steps, g_phi_steps;
extern bool g_gouraud;

void CommandLineArgumentParser::parse(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (NEXT_ARG("-input")) {
            i++; assert (i < argc);
            input_file = argv[i];
        } else if (NEXT_ARG("-size")) {
            i++; assert (i < argc);
            width = atoi(argv[i]);
            i++; assert (i < argc);
            height = atoi(argv[i]);
        } else if (NEXT_ARG("-output")) {
            i++; assert (i < argc);
            output_file = argv[i];
        } else if (NEXT_ARG("-depth")) {
            i++; assert (i < argc);
            depth_min = atof(argv[i]);
            i++; assert (i < argc);
            depth_max = atof(argv[i]);
            i++; assert (i < argc);
            depth_file = argv[i];
        } else if (NEXT_ARG("-normals")) {
            i++; assert(i < argc);
            normal_file = argv[i];
        } else if (NEXT_ARG("-shade_back")) {
            shade_back = true;
        } else if (NEXT_ARG("-gui")) {
            use_gui = true;
        } else if (NEXT_ARG("-tessellation")) {
            i++; assert(i < argc);
            g_theta_steps = atoi(argv[i]);
            i++; assert(i < argc);
            g_phi_steps = atoi(argv[i]);
        } else if (NEXT_ARG("-gouraud")) {
            g_gouraud = true;
        } else if (NEXT_ARG("-shadows")) {
            cast_shadow = true;
        } else if (NEXT_ARG("-bounces")) {
            i++; assert(i < argc);
            max_bounces = atoi(argv[i]);
        } else if (NEXT_ARG("-weight")) {
            i++; assert(i < argc);
            cutoff_weight = atof(argv[i]);
        } else if (NEXT_ARG("-visualize_grid")) {
            visualize_grid = true;
        } else if (NEXT_ARG("-grid")) {
            i++; assert(i < argc);
            grid_nx = atoi(argv[i]);
            i++; assert(i < argc);
            grid_ny = atoi(argv[i]);
            i++; assert(i < argc);
            grid_nz = atoi(argv[i]);
        } else if (NEXT_ARG("-stats")) {
            stats = true;
        } else if (NEXT_ARG("-random_samples") || NEXT_ARG("-uniform_samples") || NEXT_ARG("-jittered_samples")) {
            sampler_strategy = argv[i];
            i++; assert(i < argc);
            samples_num = atoi(argv[i]);
        } else if (NEXT_ARG("-render_samples")) {
            i++; assert(i < argc);
            samples_file = argv[i];
            i++; assert(i < argc);
            samples_zoom = atoi(argv[i]);
        } else if (NEXT_ARG("-render_filter")) {
            i++; assert(i < argc);
            filter_file = argv[i];
            i++; assert(i < argc);
            filter_zoom = atoi(argv[i]);
        } else if (NEXT_ARG("-box_filter") || NEXT_ARG("-tent_filter") || NEXT_ARG("-gaussian_filter")) {
            filter_strategy = argv[i];
            i++; assert(i < argc);
            filter_argument = atof(argv[i]);
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

}
