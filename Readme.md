# Simple MD

This is a simple program for calculating molecular dynamics of particles under lennard jones potential.

## Complie
Using C++11 standard
```
g++ --std=c++11 src/main.cpp -o src/main
```
## Install python dependencies
```
cd script
pip3 install -r requirements.txt
```
## Input parameter
All parameters are preset in input/input.txt
```
Molecule                 Argon(Ar)
Particle_number          512
Sigma(Angstrom)          3.41
Epsilon(KJ/mol)          0.996078441772
Mass(g/mol)              39.948
Init_Temperatrue(K)      130
Temperature(K)           130
Time_interval(fs)        0.1
Equilibration_time(ns)   0.005
Total_time(ns)           0.05
Box_size(Angstrom)       30
```
## Usage
```
cd src
./main ../input/input.txt && python3 ../script/make_plot.py
```
