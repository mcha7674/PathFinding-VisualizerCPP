#pragma once

#include <math.h>
#include "Body.h"
#include "Trail.h"

//#include <vector>

// UNIVERSAL CONSTANTS
#define PI static_cast<float>(3.14159265)
//static const float G_M = 4 * PI * PI; // G * sunMass constant
//static const float sunMass = 1.989e30; // in kg

const float earthMass = 3.00273e-6; // in Solar Masses
const float G = 39.478; // AU^3yr^-2Ms^-1
// Force Constant predicted by General Relativity
#define ALPHA_FACTOR static_cast<float>(1.1e-8) // AU^2 - correction factor as predected by general relativity
#define FCONST(M,m,r,B) static_cast<float>(( (G * M * m) / pow(r, B + 1.0f) ) * (1+ (ALPHA_FACTOR/pow(r,2))))

class Orbit
{
public:
    // Body, its star's mass, and Body Trail
    Body* body;
    Trail* bodyTrail;
    float starMass; // in Solar Mass units

    // Position vars (AU units)
    float x0;
    float y0;
    float x;
    float y;
    float r;
    // Velocity vars (AU/yr units)
    float vx;
    float vy;
    float v;
    float v_minus1;
    float v0;
    float vx0;
    float vy0;
    // Acceleration
    float ax = 0;
    float ay = 0;
    // Force vars (MAGNITUDES)
    float fx;
    float fy;
    float f;
    float B; // beta constant for inverse law
    // ENERGY vars
    float KE;
    float PE;
    float E;
    // time related vars (yr units)
    float t;
    float dt;
    // Orbit Statistics
    float aphelion;   // max radius
    float perihelion; // min radius
    bool aphelionReached;
    bool perihelionReached;
    float period;     // period of the orbit
    bool finishedPeriod;
    
    
    unsigned int iterations = 0;
public:
    // PARAMETERIZED CONSTRUCTOR
    Orbit(float starMass, float bodyRadius, float bodyMass, float initx, float inity, float initvx,
        float initvy, float beta, float t, float dt);
    // DATA CREATION
    void Update(float t, float dt, bool rkIntegration = false);
    void Reset();
    void ResetTrail();

    bool isAphelion(float v0, float v);
    bool isPerihelion(float v0, float v);
    inline bool isPeriodComplete() { return finishedPeriod; };

private:
    

    float PartialStep(float& f, float df, float scale);
    
};