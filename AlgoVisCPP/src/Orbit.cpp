#include "Orbit.h"


// Constructor Initializes all attribute vars
Orbit::Orbit(float starMass, float bodyRadius,float bodyMass, float initx, float inity, float initvx, float initvy, float beta, float t, float dt)
    : starMass(starMass), body(body), x{ initx }, y{ inity }, vx{ initvx }, vy{ initvy }, B{ beta }, t(t), dt(dt)
{
    // Init Body
    body = new Body(bodyRadius, bodyMass);
    // Init Trail On Heap
    bodyTrail = new Trail();
    // Init Orbit Parameters
    x0 = x;
    y0 = y;
    vx0 = vx;
    vy0 = vy;
    // initialize vector magnitudes:
    r = sqrt(x * x + y * y);     // magnitude of initial position
    v = sqrt(vx * vx + vy * vy); // magnitude of initial velocity
    v0 = v; // set initial var variable
    // Initialize the force (magnitude)
    fx = FCONST(starMass, body->mass,r,B) * x;
    fy = FCONST(starMass, body->mass,r,B) * y;
    f = sqrt(fx * fx + fy * fy);
    // Energy Inits
    PE = ( - 1 * (G * starMass * body->mass) )/ r;
    KE = 0.5 * body->mass * pow(v, 2);
    E = KE + PE;

    // calculate Theoretical Statistics (period, aphelion, perihelion)
    aphelion = NULL;
    perihelion = NULL;
    aphelionReached = false;
    perihelionReached = false;
    period = FLT_MAX;
    finishedPeriod = false;

    std::cout << "ORBIT INITIALIZED" << std::endl;
}



//Update Orbit Positions using EULER CROMER METHOD
void Orbit::Update(float universeTime, float deltaTime, bool rkIntegration)
{
    // Time variables
    t = universeTime;
    dt = deltaTime;
    // Distance from the sun
    r = sqrt((x * x) + (y * y));
    // Force Constant
    
    // Euler Cromer Method //
    if (!rkIntegration)
    {        
        v_minus1 = v;

        // updating the force
        fx = FCONST(starMass, body->mass,r,B) * x;
        fy = FCONST(starMass, body->mass,r,B) * y;
        f = sqrt(fx * fx + fy * fy);
        // updating velocity
        vx = vx - (fx / body->mass) * dt;
        vy = vy - (fy / body->mass) * dt;
        v = sqrt(vx * vx + vy * vy);
        // updating orbit position
        x = x + (vx * dt);
        y = y + (vy * dt);
        r = sqrt(x * x + y * y);
        // Update Energy
        PE = -1 * (G*starMass * body->mass) / r;
        //PE *= 1e6;
        KE = 0.5 * body->mass * pow(v, 2);
        //KE *= 1e6;
        E = KE + PE;
    }
    else { // Runga Kutta 4th Order Method //    
        //k1 - acceleration at current location
        float k1x = -fx;
        float k1y = -fy;
        ////k2 - acceleration 0.5 timesteps in the future based on k1 acceleration value
        float vx_u = PartialStep(vx, k1x, 0.5f);
        float x_u = PartialStep(x, vx_u, 0.5f);
        float k2x = -FCONST(starMass, body->mass,r,B) * x_u;

        float vy_u = PartialStep(vy, k1y, 0.5f);
        float y_u = PartialStep(y, vy_u, 0.5f);
        float k2y = -FCONST(starMass, body->mass,r,B) * y_u;
        ////k3 acceleration 0.5 timesteps in the future using k2 acceleration
        vx_u = PartialStep(vx, k2x, 0.5f);
        x_u = PartialStep(x, vx_u, 0.5f);
        float k3x = -FCONST(starMass, body->mass,r,B) * x_u;

        vy_u = PartialStep(vy, k2y, 0.5f);
        y_u = PartialStep(y, vy_u, 0.5f);
        float k3y = -FCONST(starMass, body->mass,r,B) * y_u;
        ////k4 - location 1 timestep in the future using k3 acceleration
        vx_u = PartialStep(vx, k3x, 1.0f);
        x_u = PartialStep(x, vx_u, 1.0f);
        float k4x = -FCONST(starMass, body->mass,r,B) * x_u;

        vy_u = PartialStep(vy, k3y, 1.0f);
        y_u = PartialStep(y, vy_u, 1.0f);
        float k4y = -FCONST(starMass, body->mass,r,B) * y_u;


        // Final Accelerations, Velocities and Postions
        float ax = (1.0f/body->mass)*(k1x + (2.0f * k2x) + (2.0f * k3x) + k4x) / 6.0f;
        float ay = (1.0f / body->mass) * (k1y + (2.0f * k2y) + (2.0f * k3y) + k4y) / 6.0f;

        vx += ax * dt;
        vy += ay * dt;
        v = sqrt(vx * vx + vy * vy);
        x += vx * dt;
        y += vy * dt;
        r = sqrt(x * x + y * y);
        fx = FCONST(starMass, body->mass,r,B) * x;
        fy = FCONST(starMass, body->mass,r,B) * y;
        f = sqrt(fx * fx + fy * fy);
    }
    
    // Period Calculations //
    /*isAphelion(v_minus1, v);
    isPerihelion(v_minus1, v);
    if (aphelionReached && perihelionReached && !finishedHalfPeriod) { 
        period = 2 * t + 50*dt;
        finishedHalfPeriod = true;
    }
    if (t >= period && !finishedPeriod) { 
        finishedPeriod = true; 
        itersTo1Period = iterations;
    }
    if (iterations == itersTo1Period) { 
        periodCycles++; 
        iterations = 0;
    }*/
    iterations++;
}

float Orbit::PartialStep(float& f, float df, float scale)
{
    return f + (df * dt * scale);
}


void Orbit::Reset()
{
    // reset position, velocities, and energies to their previous values
    x = x0;
    y = y0;
    r = sqrt(x * x + y * y);
    vx = vx0;
    vy = vy0;
    v = sqrt(vx * vx + vy * vy);
    v0 = v;

    t = 0.0f;
    finishedPeriod = false;
    aphelionReached = false;
    perihelionReached = false;
    aphelion = NULL;
    perihelion = NULL;
    period = FLT_MAX;

    iterations = 0;
    
    // ReInitialize the force (magnitude)
    fx = FCONST(starMass, body->mass,r,B);
    fy = FCONST(starMass, body->mass,r,B);
    f = sqrt(fx * fx + fy * fy);

    // Energy Reinits
    PE = -1 * (G*starMass * body->mass) / r;
    KE = 0.5 * body->mass * pow(v, 2);
    E = KE + PE;
    

    std::cout << "ORBIT RESET" << std::endl;
}

// Incorporate a dt correction check 
bool Orbit::isAphelion(float v_minus1, float v)
{
    if ((v - v_minus1) > 0)
    {
        aphelionReached = true;
        return true;
    }
    
    
    return false;
}

bool Orbit::isPerihelion(float v_minus1, float v)
{
    if ((v - v_minus1) < 0)
    {
        perihelionReached = true;
        return true;
    }
    
    
    return false;
}

void Orbit::ResetTrail()
{
    bodyTrail->ResetVertices();
}






   

