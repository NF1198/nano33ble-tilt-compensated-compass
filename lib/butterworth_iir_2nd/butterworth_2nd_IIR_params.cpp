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
#include <cmath>
#include <iostream>

template <typename T>
Butterworth2ndOrderIIRFilterParams<T>::Butterworth2ndOrderIIRFilterParams(T passband,
                                                              T rate)
    : passband_(passband), samplerate_(rate) {
  this->calculate_filter_parameters();
}

template <typename T>
void Butterworth2ndOrderIIRFilterParams<T>::calculate_filter_parameters() {
  T const fs = this->samplerate();
  T const fc = std::min(static_cast<T>(fs * 0.49), this->passband());

  // std::cerr << "passband: " << fc << ", samplerate: " << fs << "\n";

  // calculate the normalied frequency, wc
  T wc = static_cast<T>(2.0 * M_PI * fc / fs);

  // std::cerr << "wc: " << wc << "\n";

  // calculate w_analog == s
  T w_analog = static_cast<T>(tan(0.5 * wc));

  // std::cerr << "w_analog: " << w_analog << "\n";

  // calculate 1/s
  T s_inv = static_cast<T>(1.0 / w_analog);

  // std::cerr << "s_inv: " << s_inv << "\n";

  // pre-calculate multipliers
  T TT = static_cast<T>(pow(s_inv, 2.0));
  T UU = static_cast<T>(M_SQRT2 * s_inv);

  // std::cerr << "TT: " << TT << "\n";
  // std::cerr << "UU: " << UU << "\n";

  T gain = static_cast<T>(TT + UU + 1.0);
  T b1 = static_cast<T>(-1 * (-2.0 * TT + 2.0) / gain);
  T b2 = static_cast<T>(-1 * (TT - UU + 1.0) / gain);

  // std::cerr << "gain: " << gain << "\n";
  // std::cerr << "b1: " << b1 << "\n";
  // std::cerr << "b2: " << b2 << "\n";

  this->gain_ = gain;
  this->b1_ = b1;
  this->b2_ = b2;
}

template <typename T>
Butterworth2ndOrderIIRFilterParams<T> &
Butterworth2ndOrderIIRFilterParams<T>::passband(T value) {
  this->passband_ = value;
  this->calculate_filter_parameters();
  return *this;
}

template <typename T>
Butterworth2ndOrderIIRFilterParams<T> &
Butterworth2ndOrderIIRFilterParams<T>::samplerate(T value) {
  this->samplerate_ = value;
  this->calculate_filter_parameters();
  return *this;
}

template <typename T>
T Butterworth2ndOrderIIRFilterParams<T>::passband() {
    return this->passband_;
}

template <typename T>
T Butterworth2ndOrderIIRFilterParams<T>::samplerate() {
    return this->samplerate_;
}

template <typename T>
T Butterworth2ndOrderIIRFilterParams<T>::gain() {
    return this->gain_;
}

template <typename T>
T Butterworth2ndOrderIIRFilterParams<T>::B1() {
    return this->b1_;
}

template <typename T>
T Butterworth2ndOrderIIRFilterParams<T>::B2() {
    return this->b2_;
}

template class Butterworth2ndOrderIIRFilterParams<double>;
template class Butterworth2ndOrderIIRFilterParams<float>;