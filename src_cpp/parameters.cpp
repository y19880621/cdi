//
//  parameters.cpp
//  ArrayFire-OpenCL
//
//  Created by Barbara Frosik on 8/15/16.
//  Copyright © 2016 ArrayFire. All rights reserved.
//

#include "map"
#include "parameters.hpp"
#include "common.h"
#include "iostream"
#include "libconfig.h++"

// maps algorithm name to algorithm number
std::map<std::string, int> algorithm_map;
// an array holding algoritms iterations.
std::vector<int> algorithm_sequence;

// amplitude threshold
d_type amp_threshold;
bool amp_threshold_fill_zeros;

d_type phase_min = 120;
d_type phase_max = 10;
float beta = .9;

// support
std::vector<int> roi;
int support_update_step = 5;
float support_threshold;
int support_type;
bool d_type_precision;

// when to start averaging iterates
int iterate_avg_start;
// number of iterates to average

using namespace libconfig;

Params::Params(const char* config_file)
{
    BuildAlgorithmMap();
    Config cfg;
    
    // Read the file. If there is an error, report.
    try
    {
        cfg.readFile(config_file);
    }
    catch(const FileIOException &fioex)
    {
        printf("file I/O exception");
    }
    catch(const ParseException &pex)
    {
        printf("parsing exception");
    }
    
    try {
        const Setting& root = cfg.getRoot();
        const Setting &tmp = root["alg"];
        int count = tmp.getLength();
        
        for (int i = 0; i < count; ++i)
        {            
            int repeat = tmp[i][0];
            for (int j = 0; j < repeat; ++j)
            {
                for (int k = 1; k < tmp[i].getLength(); ++k)
                {
                    int iterations = tmp[i][k][1];
                    for (int l = 0; l < iterations; ++l)
                    {
                        algorithm_sequence.push_back(algorithm_map[tmp[i][k][0]]);
                    }
                }
            }
        }
    }
    catch ( const SettingNotFoundException &nfex)
    {
        printf("No 'algorithm_sequence' parameter in configuration file.");
    }
    
    try {
        const Setting& root = cfg.getRoot();
        const Setting &tmp = root["roi"];
        int count = tmp.getLength();
        
        for (int i = 0; i < count; ++i)
        {
            roi.push_back(tmp[i]);
        }
    }
    catch ( const SettingNotFoundException &nfex)
    {
        printf("No 'roi' parameter in configuration file.");
    }
    
    try
    {
        amp_threshold = cfg.lookup("amp_threshold");
    }
    catch(const SettingNotFoundException &nfex)
    {
        printf("No 'amp_threshold' parameter in configuration file.");
    }

    try {
        amp_threshold_fill_zeros = cfg.lookup("amp_threshold_fill_zeros");
    }
    catch (const SettingNotFoundException &nfex)
    {
        printf("No 'amp_threshold_fill_zeros' parameter in configuration file.");
    }
    
    try {
        phase_min = cfg.lookup("phase_min");
    }
    catch (const SettingNotFoundException &nfex)
    {
        printf("No 'phase_min' parameter in configuration file.");
    }

    try {
        phase_max = cfg.lookup("phase_max");
    }
    catch (const SettingNotFoundException &nfex)
    {
        printf("No 'phase_max' parameter in configuration file.");
    }

    try
    {
        beta = cfg.lookup("beta");
    }
    catch (const SettingNotFoundException &nfex)
    {
        printf("No 'beta' parameter in configuration file.");
    }

    try
    {
        support_update_step = cfg.lookup("support_update_step");
    }
    catch (const SettingNotFoundException &nfex)
    {
        printf("No 'support_update_step' parameter in configuration file.");
    }

    try
    {
        support_threshold = cfg.lookup("support_threshold");
    } catch (const SettingNotFoundException &nfex)
    {
        printf("No 'support_threshold' parameter in configuration file.");
    }

    try
    {
        support_type = cfg.lookup("support_type");
    } catch (const SettingNotFoundException &nfex) {
        printf("No 'support_type' parameter in configuration file.");
    }

    try
    {
        iterate_avg_start = cfg.lookup("iterate_avg_start");
    } catch (const SettingNotFoundException &nfex) {
        printf("No 'iterate_avg_start' parameter in configuration file.");
    }
        
}

void Params::BuildAlgorithmMap()
{
    // hardcoded
    algorithm_map.insert(std::pair<char*,int>("ER", ALGORITHM_ER));
    algorithm_map.insert(std::pair<char*,int>("HIO", ALGORITHM_HIO));
}

int Params::GetIterationsNumber()
{
    return algorithm_sequence.size();
}

std::vector<int> Params::GetAlgorithmSequence()
{
    return algorithm_sequence;
}

int Params::GetSuportUpdateStep()
{
    return support_update_step;
}

d_type Params::GetAmpThreshold()
{
    return amp_threshold;
}

bool Params::IsAmpThresholdFillZeros()
{
    return amp_threshold_fill_zeros;
}

d_type Params::GetPhaseMin()
{
    return phase_min;
}

d_type Params::GetPhaseMax()
{
    return phase_max;
}

float Params::GetBeta()
{
    return beta;
}

std::vector<int> Params::GetRoi()
{
    return roi;
}

int Params::GetIterateAvgStart()
{
    return iterate_avg_start;
}


