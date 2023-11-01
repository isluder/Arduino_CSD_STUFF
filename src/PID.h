#ifndef PID_H
#define PID_H
#include <Arduino.h>

class PIDController
{
public:
    PIDController(float k_P, float k_I, float k_D, float u_max, float T)
        : k_P(k_P), k_I(k_I), k_D(k_D), T(T), e_last(0.0), y_last(0.0), e_integral(0.0), e_derivative(0.0), u_max(u_max) {}

    int computePID(float e)
    {
        // Compute the (first-order) derivative of the error
        e_derivative = (e - e_last) / T;

        // Compute the control signal
        float u = k_P * e + k_I * e_integral + k_D * e_derivative;

        // Check for control signal saturation
        if (u >= fabs(u_max))
        {
            // Saturate the control signal
            u = copysign(u_max, u);
        }
        else
        {
            // Only integrate if the control signal is not saturated
            e_integral += T * e;
        }

        // Save the latest error
        e_last = e;

        return static_cast<int>(u);
    }

private:
    float k_P;
    float k_I;
    float k_D;
    float T;
    float e_last;
    float y_last;
    float e_integral;
    float e_derivative;
    float u_max;
};

#endif
