#ifndef PID_H
#define PID_H

class PID
{
public:
    PID(float k_P, float k_I, float k_D, float T)
        : e_last(0.0), e_integral(0.0), e_derivative(0.0), k_P(k_P), k_I(k_I), k_D(k_D), T(T), Max_OverAll(25), Min_OverAll(-25), isnormalize(false), max_integral(125) {}

    void setNormalize(float max_d, float min_d, float max_a, float min_a)
    {
        this->max_d = max_d;
        this->min_d = min_d;
        this->max_a = max_a;
        this->min_a = min_a;
        isnormalize = true;
    };

    float computePID(float error)
    {
        // Compute the PID a_location
        e_derivative = (error - e_last) / T;
        float a_location = k_P * error + k_I * e_integral + k_D * e_derivative;
        e_integral += T * error;
        e_last = error;

        if (a_location > Max_OverAll){
            Max_OverAll = a_location; //This helps calibrate mapping over time
        }
        if (a_location < Min_OverAll){
            Min_OverAll = a_location;
        }

        // Normalize the output to angles
        if (isnormalize)
        {
            a_location = normalize(a_location);
        }

        // if (e_integral > max_integral)
        // {
        //     // Check if the integral is growing too fast
        //     resetI();
        // }

        if (a_location <= max_a && a_location >= min_a)
        {
            return a_location;
        }
        else
        {
            // Add a default return value
            return NAN;
        }

    };

    String getVariables()
    {
        String vars = ","+String(Max_OverAll);
        vars += ","+String(Min_OverAll);
        // vars += "e_derivative: " + String(e_derivative) + "\n";
        // vars += "k_P: " + String(k_P) + "\n";
        // vars += "k_I: " + String(k_I) + "\n";
        // vars += "k_D: " + String(k_D) + "\n";
        // vars += "T: " + String(T) + "\n";
        // // vars += "u_max: " + String(u_max) + "\n";
        // vars += "isnormalize: " + String(isnormalize) + "\n";
        // vars += "max_d: " + String(max_d) + "\n";
        // vars += "min_d: " + String(min_d) + "\n";
        // vars += "max_integral: " + String(max_integral) + "\n";
        return vars;
    }

private:
    // Compute Variables
    float e_last;
    float e_integral;
    float e_derivative;

    // PID Variables
    float k_P;
    float k_I;
    float k_D;

    // Configured Variables
    float T;
    float Max_OverAll;
    float Min_OverAll;

    // Normalize Variables
    bool isnormalize;
    float max_d;
    float min_d;
    float max_a;
    float min_a;

    // Check Integral
    float max_integral;

    float normalize(float a_location)
    {
        return map(a_location, Min_OverAll, Max_OverAll, min_a, max_a);
    };

    void resetI()
    {
        e_integral = 0.0;
    };
};

#endif