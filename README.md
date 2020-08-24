These files read Biosemi data in BDF format. There are 3 options:

1) Matlab: this folder contains the matlab function "extract_data_biosemi". This function takes two input: 1) the name of the BDF file to open and 2) the refernce channel(s) expressed in number (position). If more than one reference channel is used, the function will average the N channels entered. The function handles the special module 9 "ABR", which does not require a reference channel. In this case, the user needs to enter "0". The polarity will also be adjusted to have Cz - Average channeles

2) MEX: this folder contains the matlab function "extract_data_biosemi_mex", which uses the mex function "Read_BDF_MEX". The C++ source code can be found in the file "Read_BDF_MEX.cpp". The function "extract_data_biosemi_mex" takes two input: 1) the name of the BDF file to open and 2) the refernce channel(s) expressed in number (position). If more than one reference channel is used, the function will average the N channels entered. The function handles the special module 9 "ABR", which does not require a reference channel. In this case, the user needs to enter "0". The polarity will also be adjusted to have Cz - Average channeles

3) C++: this folder contains the file "Read_BDF.cpp", the executable file "Read_BDF" and the TDS file "Read_BDF".

These files are part of a large GUI that I am using to analyze EEG and MEG data.

If you have any question and/or want to report bugs, please e-mail me (Alex) at: pressalex@hotmail.com
