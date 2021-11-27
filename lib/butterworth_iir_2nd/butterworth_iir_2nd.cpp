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

#include "butterworth_2nd_IIR_params.h"
#include "butterworth_iir_2nd.h"
#include <math.h>

template <typename T>
void IIR_2nd_order_temporal_filter<T>::check_init(const T &ref, const T &gain)
{
    for (int idx = 1; idx < NZEROS + 1; idx++)
    {
        X[idx] = ref / gain;
    }
    for (int idx = 1; idx < NPOLES + 1; idx++)
    {
        Y[idx] = ref;
    }
}

template <typename T>
IIR_2nd_order_temporal_filter<T>::IIR_2nd_order_temporal_filter(
    const Butterworth2ndOrderIIRFilterParams<T> &params, const T &frame) : params_{params}
{
    T gain = params_.gain();
    for (int idx = 1; idx < NZEROS + 1; idx++)
    {
        X[idx] = frame / gain;
    }
    for (int idx = 1; idx < NPOLES + 1; idx++)
    {
        Y[idx] = frame;
    }
}

template <typename T>
IIR_2nd_order_temporal_filter<T> &IIR_2nd_order_temporal_filter<T>::reset(Butterworth2ndOrderIIRFilterParams<T> &params)
{
    T old_gain = params_.gain();
    T new_gain = params.gain();
    params_ = params;
    resetgain(old_gain, new_gain);
    return *this;
}

template <typename T>
IIR_2nd_order_temporal_filter<T> &IIR_2nd_order_temporal_filter<T>::update(const T &frame)
{
    // rotate inputs array
    T tempX = X[0];
    X[0] = X[1];
    X[1] = X[2];
    X[2] = tempX;

    // rotate outputs array
    T tempY = Y[0];
    Y[0] = Y[1];
    Y[1] = Y[2];
    Y[2] = tempY;

    // apply input
    X[2] = frame;
    X[2] /= params_.gain();

    // compute output frame
    tempB = Y[0];
    tempB *= params_.B2();
    tempC = Y[1];
    tempC *= params_.B1();
    Y[2] = X[1];
    Y[2] *= 2;
    Y[2] += X[0];
    Y[2] += X[2];
    Y[2] += tempB;
    Y[2] += tempC;

    return *this;
}

template <typename T>
IIR_2nd_order_temporal_filter<T> &IIR_2nd_order_temporal_filter<T>::reset(const T &gain, const T &ref)
{
    for (int idx = 1; idx < NZEROS + 1; idx++)
    {
        X[idx] = ref / gain;
    }
    for (int idx = 1; idx < NPOLES + 1; idx++)
    {
        Y[idx] = ref;
    }
    return *this;
}

template <typename T>
IIR_2nd_order_temporal_filter<T> &IIR_2nd_order_temporal_filter<T>::resetgain(const T &old_gain, const T &new_gain)
{
    if (old_gain != 0 && !std::isnan(old_gain))
    {
        T gain_ratio = old_gain / new_gain;
        X[0] *= gain_ratio;
        X[1] *= gain_ratio;
        X[2] *= gain_ratio;
    }
    return *this;
}

template <typename T>
T IIR_2nd_order_temporal_filter<T>::value() const { return Y[2]; }

template class IIR_2nd_order_temporal_filter<double>;
template class IIR_2nd_order_temporal_filter<float>;

