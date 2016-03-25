/*
The MIT License (MIT)

Copyright (c) 2006-2016 Thomas Fussell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <ctime>

#include "simulation/random_number_generator.hpp"

namespace cort {

random_number_generator::random_number_generator()
{
  std::time_t time = std::time(0);
  seed(static_cast<int>(time));
}

random_number_generator::random_number_generator(int seed)
{
  this->seed(seed);
}

random_number_generator::~random_number_generator()
{

}

unsigned int random_number_generator::rand_uint()
{
  unsigned long long int sum;
  sum = (unsigned long long int)2111111111UL * (unsigned long long int)x[3] +
    (unsigned long long int)1492 * (unsigned long long int)(x[2]) +
    (unsigned long long int)1776 * (unsigned long long int)(x[1]) +
    (unsigned long long int)5115 * (unsigned long long int)(x[0]) +
    (unsigned long long int)x[4];
  x[3] = x[2];
  x[2] = x[1];
  x[1] = x[0];
  x[4] = (unsigned int)(sum >> 32);
  x[0] = (unsigned int)sum;
  return x[0];
}

double random_number_generator::rand_double()
{
  return (double)rand_uint() * (1. / (65536. * 65536.));
}

double random_number_generator::rand_double(double max)
{
  return rand_double() * max;
}

double random_number_generator::rand_double(double min, double max)
{
  return min + rand_double() * (max - min);
}

int random_number_generator::rand_int(int max)
{
  return rand_int(0, max);
}

int random_number_generator::rand_int(int min, int max)
{
  if(max <= min)
    {
      if(max == min)
        {
	  return min;
        }
      else
        {
	  return 0x80000000;
        }
    }

  unsigned int interval = (unsigned int)(max - min);
  return ((int)(interval * rand_double())) + min;
}

void random_number_generator::seed(int seed)
{
  int i;
  unsigned int s = seed;

  for(i = 0; i < 5; i++)
    {
      s = s * 29943829 - 1;
      x[i] = s;
    }

  for(i = 0; i < 19; i++)
    {
      rand_uint();
    }
}

std::istream &operator>>(std::istream &in, random_number_generator &rng)
{
  for(int i = 0; i < 5; i++)
    {
      in >> rng.x[i];
    }

  return in;
}

std::ostream &operator<<(std::ostream &out, const random_number_generator &rng)
{
  out << rng.x[0] << " " << rng.x[1] << " " << rng.x[2] << " " << rng.x[3] << " " << rng.x[4];
  return out;
}

} // namespace cort
