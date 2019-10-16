#include <iostream>
#include <fstream>
#include "box.h"
#include "ensemble.h"
#include "particle.h"

using namespace std;

int main() {
    //box size: x=30, y=30, z=30
    Box box1(5, 5, 5);

    // ensemble with x particles;
    Ensemble ensemble1(2);

    // define output file name
    ofstream particle_out("particle1.log");
    ofstream ensemble_out("ensemble1.log");

    // define time, which particle to show, box and output file names
    ensemble1.execute(5e5, 1, box1, particle_out, ensemble_out);

    particle_out << flush;
    ensemble_out << flush;

    particle_out.close();
    ensemble_out.close();
}