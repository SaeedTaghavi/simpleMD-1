#ifndef ENSEMBLE_H
#define ENSEMBLE_H

#include <vector>
#include <fstream>
// #include <cstdlib>
// #include <ctime>
#include "box.h"
#include "particle.h"

using namespace std;

class Ensemble {

public:
    Ensemble(const unsigned _particle_number): particle_number(_particle_number) {
        for (unsigned i = 0; i < particle_number; ++i) {
            ensemble.push_back(Particle((i + 1) * 0.001, (i + 1) * 0.001, (i + 1) * 0.001, i + 1, i + 1, i + 1, 5, 10, 2, 1e-5));
        }
    }

    // use ensemble[index] to call particle's method
    Particle& operator[] (const int index) {
        return ensemble[index];
    }

    // calculate the total acceleration from other particles (ensemble version)
    // to save the resource, the potential of current partcile is calculated at the same time
    void interact(Particle& particle) {
        particle.a_x = 0;
        particle.a_y = 0;
        particle.a_z = 0;
        double distance_value = 0;
        particle.potential_value = 0;
        for (auto particle_ptr = ensemble.begin(); particle_ptr != ensemble.end(); ++particle_ptr) {
            // exclude the current particle
            if (&(*particle_ptr) != &particle) {
                // distance_value = sqrt(
                //     pow((particle_ptr->pos_x - particle.pos_x), 2) + 
                //     pow((particle_ptr->pos_y - particle.pos_y), 2) +
                //     pow((particle_ptr->pos_z - particle.pos_z), 2));
                // particle.a_x += -4 * particle.epsilon * (12 * pow(particle.sigma, 12) / pow(distance_value, 13) - 
                //     6 * pow(particle.sigma, 6) / pow(distance_value, 7)) * (particle_ptr->pos_x - particle.pos_x) / particle.mass;
                // particle.a_y += -4 * particle.epsilon * (12 * pow(particle.sigma, 12) / pow(distance_value, 13) - 
                //     6 * pow(particle.sigma, 6) / pow(distance_value, 7)) * (particle_ptr->pos_y - particle.pos_y) / particle.mass;
                // particle.a_z += -4 * particle.epsilon * (12 * pow(particle.sigma, 12) / pow(distance_value, 13) - 
                //     6 * pow(particle.sigma, 6) / pow(distance_value, 7)) * (particle_ptr->pos_z - particle.pos_z) / particle.mass;

                // another version of F(r) function to see if the speed is faster than before
                particle.calculate_distance_value(*particle_ptr);
                particle.a_x += -4 * particle.epsilon * (12 * pow(particle.sigma / particle.distance_value, 12) / particle.distance_value - 
                    6 * pow(particle.sigma / particle.distance_value, 6) / particle.distance_value ) * (particle_ptr->pos_x - particle.pos_x) / particle.mass;
                particle.a_y += -4 * particle.epsilon * (12 * pow(particle.sigma / particle.distance_value, 12) / particle.distance_value - 
                    6 * pow(particle.sigma / particle.distance_value, 6) / particle.distance_value ) * (particle_ptr->pos_y - particle.pos_y) / particle.mass;
                particle.a_z += -4 * particle.epsilon * (12 * pow(particle.sigma / particle.distance_value, 12) / particle.distance_value - 
                    6 * pow(particle.sigma / particle.distance_value, 6) / particle.distance_value ) * (particle_ptr->pos_z - particle.pos_z) / particle.mass;

                // caluculate potential value
                particle.potential_value += 4 * particle.epsilon * ( pow(particle.sigma / particle.distance_value, 12) - pow(particle.sigma / particle.distance_value, 6) );
            }
        }
    }

    vector<Particle> ensemble;

    void output(ofstream& fout) {
        fout << ensemble_potential << "\t" << ensemble_kinetic << "\t" << (ensemble_potential + ensemble_kinetic) << "\n";
    }

    void execute(const unsigned time, const unsigned index, Box& box, ofstream& particle_out, ofstream& ensemble_out) {
        int i = 0;
        while (i <= time) {
            ensemble_kinetic = 0;
            ensemble_potential = 0;
            ensemble[index].output(particle_out);
            for (auto particle_ptr = ensemble.begin(); particle_ptr != ensemble.end(); ++particle_ptr) {
                interact(*particle_ptr);
                particle_ptr->kinetic();
                ensemble_potential += particle_ptr->potential_value;
                ensemble_kinetic += particle_ptr->kinetic_value;
                particle_ptr->movement();
                particle_ptr->rebounce(box);
            }
            output(ensemble_out);
            ++i;
        }
    }

private:

    unsigned particle_number;

    double ensemble_potential;

    double ensemble_kinetic;

};

#endif