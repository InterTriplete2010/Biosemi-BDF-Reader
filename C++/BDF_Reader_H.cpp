#include <iostream>
#include <fstream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <iomanip>   
#include <cstring>
#include <math.h> 
#include <vector>
#include "BDF_Reader.h"

using namespace BDF_R;

//*********************************************************************************************//
//Global variables

int srate;
int n_chan;
int s_data;
double res_data;
//double offset;
bool flag_invalid_name = false;

int bytes_data = 3;
std::vector<std::vector<double>> matrix_data;	//Matrix where to save the EEG data;

std::vector<std::vector<double>> matrix_trig;	//Matrix where to save the latency and trigger code for each event;

std::string labels_sensors;	//Store the labels of the sensors;

FILE* file_data;	//Initialize a FILE type variable to open the binary file
//*********************************************************************************************//

//*********************************************************************************************//
//This function reads the information from the header file    
void BDF_Reader::read_info_bdf(char* buffer, int* buff, char* name_file) {

	//Open the binary file			
	file_data = fopen(name_file, "rb");

	if (file_data == 0)
	{

		//std::cout << "\nThe file has not been opened. Check that the name of the file is correct and make sure to include the extension bdf\n" << "\n";

		flag_invalid_name = true;

		return;

	}

	else
	{

		//std::cout << "\nThe files been opened correctly\n";

	}

	//The first byte is an integer number
	fread(buff, 1, 1, file_data);
	buff[1] = '\0';
	
	//Bytes 2-8 are ASCII and they represent the word BIOSEMI
	fread(buffer, 1, 7, file_data);
	
	//80 bytes: Local Subject Identification
	fread(buffer, 1, 80, file_data);
	buffer[80] = '\0';
	
	//80 bytes: Local Recording identification
	fread(buffer, 1, 80, file_data);
	buffer[80] = '\0';
	
	//8 bytes: Start date of recording
	fread(buffer, 1, 8, file_data);
	buffer[8] = '\0';
	
	//8 bytes: Start time of recording
	fread(buffer, 1, 8, file_data);
	buffer[8] = '\0';
	
	//8 bytes: Number of bytes in header record
	fread(buffer, 1, 8, file_data);
	buffer[8] = '\0';
	
	//44 bytes: Version of data format
	fread(buffer, 1, 44, file_data);
	buffer[44] = '\0';
	
	//8 bytes: Number of data record
	fread(buffer, 1, 8, file_data);
	buffer[8] = '\0';
	s_data = atoi(buffer);

	//8 bytes: Duration of data record, in seconds
	fread(buffer, 1, 8, file_data);
	buffer[8] = '\0';
	int dur_data = atoi(buffer);

	//4 bytes: Number of channels in data record
	fread(buffer, 1, 4, file_data);
	buffer[4] = '\0';
	n_chan = atoi(buffer);//(int)buffer[0] - '0';
	
	//Nx16 bytes: Labels of channels
	fread(buffer, n_chan, 16, file_data);
	labels_sensors = buffer;
	buffer[n_chan * 16] = '\0';
	
	//Nx80 bytes: Transducer type
	fread(buffer, n_chan, 80, file_data);
	buffer[n_chan * 80] = '\0';
	
	//Nx8 bytes: Physical dimension of channels
	fread(buffer, n_chan, 8, file_data);
	buffer[n_chan * 8] = '\0';
	
	//Nx8 bytes: Physical minimum in units of physical dimension
	fread(buffer, n_chan, 8, file_data);
	buffer[n_chan * 8] = '\0';
	double min_physic = (double)atoi(buffer);
	
	//Nx8 bytes: Physical maximum in units of physical dimension
	fread(buffer, n_chan, 8, file_data);
	buffer[n_chan * 8] = '\0';
	double max_physic = (double)atoi(buffer);
	
	//Nx8 bytes: Digital minimum
	fread(buffer, n_chan, 8, file_data);
	buffer[n_chan * 8] = '\0';
	double min_dig = (double)atoi(buffer);
	
	//Nx8 bytes: Digital maximum
	fread(buffer, n_chan, 8, file_data);
	buffer[n_chan * 8] = '\0';
	double max_dig = (double)atoi(buffer);
	
	res_data = (max_physic - min_physic) / (max_dig - min_dig);	//Resolution of the data;
	//offset = max_physic - res_data*max_dig;
	//Nx80 bytes: Pre-filtering
	fread(buffer, n_chan, 80, file_data);
	buffer[n_chan * 80] = '\0';
	
	//Nx8 bytes: Number of samples in each data record
	//(Sample-rate if Duration of data record = "1")
	fread(buffer, n_chan, 8, file_data);
	buffer[n_chan * 8] = '\0';
	
	//Store the sampling frequency
	char* srate_char = new char[1000000];
	bool flag_srate = true;
	int track_srate = 0;

	while (flag_srate == 1) {

		srate_char[track_srate] = buffer[track_srate];

		if (srate_char[track_srate] == ' ') {

			flag_srate = false;

		}

		track_srate++;

	}

	srate = atoi(srate_char) / dur_data;	//Convert the array of characters with the sampling frequency into an integer;
	
	delete[] srate_char; //free memory previously allocated for this array;

	//Nx32 bytes: Reserved bytes
	//(Sample-rate if Duration of data record = "1")
	fread(buffer, n_chan, 32, file_data);
	buffer[n_chan * 32] = '\0';
	
}
//*********************************************************************************************//

//*********************************************************************************************//
//Extract the EEG and the triggers
std::vector<std::vector<double>> BDF_Reader::read_data_bdf(std::string name_bdf) {

	const int length_data = 10000;	//This is an arbitrary number to ensure that the buffer is long enough to read the data  
	int* buff_int = new int[length_data];	//Reads information saved as integer values;  
	char* buff_char = new char[length_data];	//Reads information saved in ASCII format; 

	//std::string name_bdf;
	char name_bdf_char[length_data];

	//std::cout << "\nType the name of the bdf file that you want to open: ";
	//getline(std::cin, name_bdf);

	strcpy(name_bdf_char, name_bdf.c_str());	//Convert string to char. c_str() converts a C++ string into a C-style string which is essentially a null terminated array of bytes.

	read_info_bdf(buff_char, buff_int, name_bdf_char);

	
	if (s_data == -1 || flag_invalid_name)
	{

		//std::cout << "\nThe variable representing the 'Number of data record' was not read correctly and returned a value of -1. It is likely that the file is too big for the RAM and it cannot be opened on this computer. The operation has been aborted\n";

		return matrix_data;

	}


	int col_data = srate * bytes_data * s_data / 3;	//Number of samples per channel
	
	//Data in Biosemi are stored in 3 bytes
	uint8_t temp_data; 		//Variable to use to temporarily store a byte;
	uint8_t byte_1;
	uint8_t byte_2;
	uint8_t byte_3;

	int track_byte;			//Variable used to keep track on the byte read for each sample
	double temp_sample;	//Variable used to temporarily store a sample
	int track_index_chan;	//Variable used to keep track of the sample read for each channel;
	int step_data;

	//Building the matrix to store the EEG data;
	std::vector<double> temp_v;
	for (int kk = 0; kk < col_data; kk++)
	{

		temp_v.push_back(0);

	}

	//The last channel is the time domain
	matrix_data.clear();
	for (int kk = 0; kk < n_chan; kk++)
	{

		matrix_data.push_back(temp_v);

	}

//Start extracting the data		
	for (int kk = 1; kk < n_chan; kk++)
	{

		fseek(file_data, (n_chan + 1) * 256 + (kk - 1) * srate * bytes_data, SEEK_SET);	//Positioning the pointer to read the data for each channel
		//fseek (file_data,(chan_n + 1)*256 + (1 - 1)*sf*3,SEEK_SET);	//Positioning the pointer to read the data for each channel

		//Looping through the data for the selected channel. Data are read every second.
		step_data = 1;
		track_byte = 0;
		track_index_chan = 0;

		for (int count_samples = 0; (count_samples < srate * bytes_data * s_data && ((temp_data = fgetc(file_data)) != EOF)); count_samples++) {

			switch (track_byte)
			{

			case 0:

				byte_1 = temp_data;
				track_byte++;

				break;

			case 1:

				byte_2 = temp_data;
				track_byte++;

				break;

			case 2:

				byte_3 = temp_data;
				track_byte = 0;

				break;

			}

			if (track_byte == 0)
			{

				temp_sample = byte_1 + byte_2 * pow(2, 8) + byte_3 * pow(2, 16);

				if (temp_sample >= pow(2, 23))
				{

					matrix_data[kk - 1][track_index_chan] = (temp_sample - pow(2, 24)) * res_data; //+ offset;
					track_index_chan++;

				}

				else
				{

					matrix_data[kk - 1][track_index_chan] = temp_sample * res_data; //+ offset;
					track_index_chan++;

				}

			}

			if (count_samples == step_data * srate * bytes_data - 1)
			{

				fseek(file_data, (n_chan + 1) * 256 + (kk - 1) * srate * bytes_data + step_data * n_chan * srate * bytes_data, SEEK_SET);	//Skip the next second for each channel;
				step_data++;

			}

		}
	}

	//fclose(file_data);	//Close the file;

	//Now save the time domain
	for (int kk = 0; kk < matrix_data[0].size(); kk++)
	{

		matrix_data[matrix_data.size() - 1][kk] = (double)kk / srate;

	}

	return matrix_data;

}

//*********************************************************************************************//
//Extract the latency and the trigger code for each event	
std::vector<std::vector<double>> BDF_Reader::read_triggers()
{

	uint8_t temp_data; 		//Variable to use to temporarily store a byte;

	//Bytes used for the triggers
	uint8_t byte_1[2];	//Array to store bits 0 - 7;
	uint8_t byte_2[2];	//Array to store bits 8 - 15;
	uint8_t byte_3[2];	//Array to store bits 16 - 23. These bits are the status bits;
	int track_bytes_trig;
	int track_count_triggers;	//Variable keeping truck of the latency of the triggers

	int track_pointer;	//Keep track of the dimension of the matrix of the triggers
	
	std::vector<double> temp_v;
	for (int kk = 0; kk < 2; kk++)
	{

		temp_v.push_back(0);

	}
	std::vector<uint8_t> vec;
	//Extract the triggers saved in the last channel
	fseek(file_data, (n_chan + 1) * 256 + (n_chan - 1) * srate * bytes_data, SEEK_SET);

		int step_data = 1;
		int track_byte = 0;
		track_bytes_trig = 0;
		track_pointer = 0;
		track_count_triggers = 0;

		for (int count_samples = 0; (count_samples < srate * bytes_data * s_data && ((temp_data = fgetc(file_data)) != EOF)); count_samples++)
		{

			if (track_bytes_trig == 2)
			{

				track_bytes_trig = 1;
				byte_1[0] = byte_1[1];

			}

			vec.push_back(temp_data);
			switch (track_byte)
			{

			case 0:

				byte_1[track_bytes_trig] = temp_data;
				track_byte++;

				break;

			case 1:

				byte_2[track_bytes_trig] = temp_data;
				track_byte++;

				break;

			case 2:

				byte_3[track_bytes_trig] = temp_data;
				track_byte = 0;
				track_bytes_trig++;
				track_count_triggers++;

				break;

			}
			
			if (count_samples > 2 && track_byte == 0)
			{

				if ((byte_1[1] != 0 || byte_2[1] != 0) && (((byte_1[1] - byte_1[0]) > 0) || (byte_2[1] - byte_1[0]) > 0)) {

					matrix_trig.push_back(temp_v);

					matrix_trig[track_pointer][0] = (double)(track_count_triggers) / srate - 1/(double)srate;	//The latency is saved in samples. One has been added to compensate for the fact that C++ starts the first index from 0;
					matrix_trig[track_pointer][1] = byte_1[1] + byte_2[1] * pow(2, 8);	//Trigger code;

					track_pointer++;

				}

			}


			else 
			{

				if ((byte_1[0] != 0 || byte_2[0] != 0) && track_byte == 0)
				{

					matrix_trig.push_back(temp_v);

					matrix_trig[track_pointer][0] = (double)(track_count_triggers) / srate - 1/(double)srate;	//The latency is saved in samples;
					matrix_trig[track_pointer][1] = byte_1[0] + byte_2[0] * pow(2, 8);	//Trigger code;
					track_pointer++;

				}


			}


			if (count_samples == step_data * srate * bytes_data - 1)
			{

				fseek(file_data, (n_chan + 1) * 256 + (n_chan - 1) * srate * bytes_data + step_data * n_chan * srate * bytes_data, SEEK_SET);	//Skip to the next second for each channel;
				step_data++;

			}

		}

		//fclose(file_data);	//Close the file;

	return matrix_trig;

}
//*********************************************************************************************//

//*********************************************************************************************//
//Returns the labels of the sensors	
std::string BDF_Reader::return_labels_sensors()
{

	return labels_sensors;

}

//*********************************************************************************************//