# distutils: language = c++
# distutils: include_dirs = ['include', 'include/af/include',]
# distutils: libraries = ['cdi', 'afcuda',  'config++',]
# distutils: library_dirs = lib

from libcpp.vector cimport vector
from libcpp.string cimport string

cdef extern from "../include/bridge.hpp":
    cdef cppclass Bridge:
        Bridge() except +
        void StartCalcWithGuess(vector[float], vector[float], vector[float], vector[int], string)
        void StartCalc(vector[float], vector[int], string)
        void StartCalcMultiple(vector[float], vector[int], string, int)
        vector[float] GetImageR();
        vector[float] GetImageI();
        vector[float] GetErrors();


cdef class PyBridge:
    cdef Bridge *thisptr
    def __cinit__(self):
        self.thisptr = new Bridge()
    def __dealloc__(self):
        del self.thisptr
    def start_calc_with_guess(self, data_r, guess_r, guess_i, dims, config):
        self.thisptr.StartCalcWithGuess(data_r, guess_r, guess_i, dims, config)
    def start_calc(self, data_r, dims, config):
        self.thisptr.StartCalc(data_r, dims, config)
    def start_calc_multiple(self, data_r, dims, config, no_threads):
        self.thisptr.StartCalcMultiple(data_r, dims, config, no_threads)
    def get_image_r(self):
        return self.thisptr.GetImageR()
    def get_image_i(self):
        return self.thisptr.GetImageI()
    def get_errors(self):
        return self.thisptr.GetErrors()

