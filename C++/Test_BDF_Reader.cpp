#include <fstream>
#include <string>
#include <stdio.h>
#include <iomanip>   
#include <cstring>
#include <math.h> 
#include <vector>

#include "BDF_Reader.h"

using namespace std;

int main() {

    BDF_R::BDF_Reader rbd;

    //std::vector<std::vector<double>> data_eeg = rbd.read_data_bdf("Newtest17-256.bdf");
    std::vector<std::vector<double>> data_eeg = rbd.read_data_bdf("Newtest17-2048.bdf");

    std::vector<std::vector<double>> triggers = rbd.read_triggers();	//Now extract the latency and trigger code for each event;

    std::string labels = rbd.return_labels_sensors();

    //Write the output of the filter to a file
    std::ofstream myfile_I;
    myfile_I.open("Triggers_I.txt");
    for (int kk = 0; kk < triggers.size(); kk++)
    {

        myfile_I << triggers[kk][0];
        myfile_I << "\t";
        myfile_I << triggers[kk][1];
        myfile_I << "\n";

    }

    myfile_I.close();

    std::ofstream myfile_II;
    myfile_II.open("EEG_I.txt");
    for (int ll = 0; ll < data_eeg.size(); ll++)
    {

        for (int kk = 0; kk < data_eeg[0].size(); kk++)//122880; kk++)//15360; kk++)
        {

            myfile_II << data_eeg[ll][kk];

            myfile_II << "\n";

        }

    }

    myfile_II.close();

    //system("pause"); 

//return 0;

}