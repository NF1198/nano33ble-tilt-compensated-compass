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


#ifndef BUTTERWORTH_2ND_IIR_FILTER_PARAMS_GUARD
#define BUTTERWORTH_2ND_IIR_FILTER_PARAMS_GUARD

template <typename T>
class Butterworth2ndOrderIIRFilterParams
{
private:
  T passband_;
  T samplerate_;
  T gain_;
  T b1_;
  T b2_;
  void calculate_filter_parameters();

public:
  Butterworth2ndOrderIIRFilterParams(T passband, T rate);

  Butterworth2ndOrderIIRFilterParams<T> &passband(T value);
  Butterworth2ndOrderIIRFilterParams<T> &samplerate(T rate);

  // Copy constructor
  Butterworth2ndOrderIIRFilterParams<T>(const Butterworth2ndOrderIIRFilterParams<T> &src) : passband_{src.passband_},
                                                                                            samplerate_{src.samplerate_},
                                                                                            gain_{src.gain_}, b1_{src.b1_},
                                                                                            b2_{src.b2_}
  {
    // empty body
  }

  // Copy-assignment 
  Butterworth2ndOrderIIRFilterParams<T>& operator=(const Butterworth2ndOrderIIRFilterParams<T> &src) {
    passband_ = src.passband_;
    samplerate_ = src.samplerate_;
    gain_ = src.gain_;
    b1_ = src.b1_;
    b2_ = src.b2_;
    return *this;
  }

  T passband();
  T samplerate();
  T gain();
  T B1();
  T B2();
};

#endif /* BUTTERWORTH_2ND_IIR_FILTER_PARAMS_GUARD */