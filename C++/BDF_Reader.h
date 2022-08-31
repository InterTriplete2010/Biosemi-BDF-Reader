#pragma once

#include <iostream>
#include <fstream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <iomanip>   
#include <cstring>
#include <math.h> 
#include <vector>

#ifndef BDF_Reader_H
#define BDF_Reader_H

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif

    namespace BDF_R
    {

        class BDF_Reader

        {


        private:

            //Read the info
            void read_info_bdf(char*, int*, char*);

        public:
            //Read the bytes
            std::vector<std::vector<double>> read_data_bdf(std::string name_bdf);

            //Read the triggers
            std::vector<std::vector<double>> read_triggers();

            //Read the labels of the sensors
            std::string return_labels_sensors();
        
        };

    }

#endif

#ifdef __cplusplus

}

#endif