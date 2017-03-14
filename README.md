# SVI
AMDP project, Imperial College.

This project is based on a paper wrote by J. Gatheral and A. Jacquier on "Arbitrage free SVI model".
The aim of this application is to provide an user friendly graphical interface to understand how different parametrisations work. A calibration tool is also used to analyse existing data.

Finally, one part of the project is dedicated to butterfly arbitrage removal.


##### Pre requisite #####
Qt : this program must be run on the Qt Framework.
boost : the ublas library is used in this project.
QtCustomPlot : Download the QtCustomPlot header and cpp file and add it to your project.

### Things to be change before running the program ###
Please be aware that a QtFileDialog is called when you try to load a data file. PLease change the parameter :
../../../../testPlot/data  TO  QString()

### Input file format ###
The format is the following :
x1  space  y1
x2  space  y2
...


### IMPORTANT ###
The calibration tool is currently non stable. The robustness will be improved by implementing the quasi explicit calibration detailed in the Zeliade paper.  
