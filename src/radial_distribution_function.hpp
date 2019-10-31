#ifndef RADIAL_DISTRIBUTION_FUNCTION_H
#define RADIAL_DISTRIBUTION_FUNCTION_H

#include <fstream>
#include <vector>
#include <cmath>
#include "ensemble.hpp"
#include "utils.hpp"

using namespace std;

class Rdf {

public:
    Rdf(int _nbins, double _BOX): nbins(_nbins), BOX(_BOX), n(0), rdf_output("../output/rdf.csv") {
        g.resize(nbins, 0.0);
        bin_width = BOX / (2.0 * nbins);
    }

    void sample(vector<Particle> &ensemble) {
        ++n;

        // #pragma omp parallel
        {
            vector<double> g_thread(nbins, 0.0);

            // #pragma omp parallel for
            for (unsigned int i = 0; i < ensemble.size() - 1; ++i) {
                for (unsigned int j = i + 1; j < ensemble.size(); ++j) {
                    double d = distance(ensemble[i], ensemble[j], BOX);
                    if (d < BOX / 2.0) {
                        int ig = d / bin_width;
                        g_thread[ig] += 2.0;
                    }
                }
            }

            for (int i = 0; i < nbins; i++) {
                g[i] += g_thread[i];
            }
        }
    }


    void normalize(int natoms) {
        double norm_factor = 4.0 / 3.0 * M_PI * natoms * (natoms-1.0) * n * pow(bin_width, 3) / pow(BOX, 3);

        #pragma omp parallel for
        for (int i = 0; i < nbins; i++)
        {
            double r = (double) i;
            double binvol = pow(r+1.0, 3) - pow(r, 3);
            g[i] /= (binvol * norm_factor);
        }
    }


    void output() {
        for (int i = 0; i < this->nbins; i++) {
            rdf_output << i * bin_width << "    " << g[i] << endl;
        }
        rdf_output.close();
    }

private:
    const int nbins;
    const double BOX;
    double bin_width;
    unsigned n;
    vector<double> g;
    ofstream rdf_output;


};

#endif