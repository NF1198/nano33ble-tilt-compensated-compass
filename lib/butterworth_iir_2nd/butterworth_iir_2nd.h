// Copyright (c) 2020,2021,2022 Nicholas Folse
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Overview - butterworth_iir_2nd.h
// This library defines a run-time configurable 2nd order butterworth low-pass filter.
// 
// Note: You can use this filter with user defined types (besides float or double) as
//       long as your custom type overrides numeric operators. You will also need to 
//       instantiate a instance of the template using your class as a parameter as in:
//         template class IIR_2nd_order_temporal_filter<your_custom_type>;
//         template class Butterworth2ndOrderIIRFilterParams<your_custom_type>;
//
// Genral Usage
//
// # First, create filter parameters with a PASSBAND and SAMPLE_RATE. Sample rate is typically
//   determined by your hardware sample rate and, more specifically, is the rate
//   at which data is fed to the filter. Theh passband is the filter cutoff frequency 
//   specified in Hz. This value should be less than SAMPLE_RATE / 2.
//
// Butterworth2ndOrderIIRFilterParams<float> my_filter_params(PASSBAND, SAMPLE_RATE);
//
// # Initialize a filter using filter params and an initial value.
//   The initial value is typically 0, but may be the first value measured by your device.
//
// IIR_2nd_order_temporal_filter<float> my_value_filter(my_filter_params, initial_value);
//
// # Update filter with raw data. This is typically done in the measurement loop.
//
// my_value_filter << measured_value;
// 
// # Read value of the filter.
// 
// printf("raw value: %f, filtered value: %f", measured_value, (float)filtered_value);
//
// # Optional: Update filter paramters
//   If your need to change the passband or sample-rate at runtime, you need to reset
//   the filter with new filter parameters. The public reset() method allows you to 
//   provide new filter parameters while attempting to preserve the state of the 
//   filter. This method may not be numerically stable, espeically if using floats.
//
//  Butterworth2ndOrderIIRFilterParams<float> my_new_filter_params(PASSBAND, SAMPLE_RATE);
//  my_value_filter.reset(my_new_filter_params);
// 


#ifndef BUTTERWORTH_2ND_IIR_GUARD
#define BUTTERWORTH_2ND_IIR_GUARD

#include "butterworth_2nd_IIR_params.h"

#define NZEROS 2
#define NPOLES 2

template <typename T>
class IIR_2nd_order_temporal_filter
{
private:
    Butterworth2ndOrderIIRFilterParams<T> params_;
    T X[NZEROS + 1], Y[NPOLES + 1];
    T tempB, tempC;

    void check_init(const T &ref, const T &gain);
    
    IIR_2nd_order_temporal_filter<T> &reset(const T &gain, const T &ref);

    IIR_2nd_order_temporal_filter<T> &resetgain(const T &old_gain, const T &new_gain);

public:
    // Constructor: Initialize a new low-pass filter.
    IIR_2nd_order_temporal_filter<T>(const Butterworth2ndOrderIIRFilterParams<T> &params, const T &frame);

    // Method (experimental): Reset an existing filter with new parameters (sample rate and passband)
    //         Note: this method may not be numerically stable, especially if using floats.
    IIR_2nd_order_temporal_filter<T> &reset(Butterworth2ndOrderIIRFilterParams<T> &params);                                      
 
    // Method: Update the filter with a new datapoint.
    IIR_2nd_order_temporal_filter<T> &update(const T &frame);

    // Method: return the latest value from the filter.
    T value() const;

    // Implicit cast to float
    operator float() const
    {
        return (float)value();
    }

    // Implicit cast to double
    operator double() const
    {
        return (double)value();
    }

    // Operator << is an alias for calling update()
    friend IIR_2nd_order_temporal_filter<T>& operator<<( IIR_2nd_order_temporal_filter<T>& tgt, const T& frame) {
        return tgt.update(frame);
    }
};

#endif /* BUTTERWORTH_2ND_IIR_GUARD */