These files read Biosemi data in BDF format. Please, keep in mind that I am following the current format of the BDF file, as reported in Biosemi's website. This means that my code assumes that the data are read every 1 second and are in a 24 bit format. If the format changes and I am not aware of that, please contact me and I will make the appropriate changes. Since I have never worked with data different from EEG, I have not been able to test the code with other input, such as Ergo input. 

There are 3 options:

1) Matlab: this folder contains the matlab function "extract_data_biosemi". This function takes two input: 1) the name of the BDF file to open and 2) the refernce channel(s) expressed in number (position). If more than one reference channel is used, the function will average the N channels entered. The function handles the special module 9 "ABR", which does not require a reference channel. In this case, the user needs to enter "0". The polarity will also be adjusted to have Cz - Average channeles. The function will return one ouput (e.g. EEG), which stores all the key info data read from the bdf file, such as the EEG data, the sampling frequency, etc.

2) MEX: this folder contains the matlab function "extract_data_biosemi_mex", which uses the mex function "Read_BDF_MEX". The C++ source code can be found in the file "Read_BDF_MEX.cpp". The function "extract_data_biosemi_mex" takes two input: 1) the name of the BDF file to open and 2) the refernce channel(s) expressed in number (position). If more than one reference channel is used, the function will average the N channels entered. The function handles the special module 9 "ABR", which does not require a reference channel. In this case, the user needs to enter "0". The polarity will also be adjusted to have Cz - Average channeles. The function will return one ouput (e.g. EEG), which stores all the key info data read from the bdf file, such as the EEG data, the sampling frequency, etc.

3) C++: this folder contains the filee BDF_Reader.h, BDF_Reader_H.cpp and Test_BDF_Reader.cpp, which can be used to test the code. There are 3 public methods that need to be executed in the following order:

    A) std::vector<std::vector<double>> rbd.read_data_bdf(File_Name): it returns the EEG data and add an additional channel at the end that can be used as a time domain.         "File_Name" needs to be replaced with the name of the bdf file that you want to open;

    B) std::vector<std::vector<double>> rbd.read_triggers(): it returns the latency expressed in seconds (1st column) and code (2nd column) of the triggers 

    C) std::string rbd.return_labels_sensors(): it returns the name of the sensors;

The Matlab and MEX files are part of a large Toolbox that I wrote and that I am using to analyze EEG and MEG data.

If you have any questions and/or want to report bugs, please e-mail me (Ale) at: pressalex@hotmail.com
