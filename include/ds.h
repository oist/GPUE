///@cond LICENSE
/*** ds.h - GPUE: Split Operator based GPU solver for Nonlinear
Schrodinger Equation, Copyright (C) 2011-2015, Lee J. O'Riordan
<loriordan@gmail.com>, Tadhg Morgan, Neil Crowley.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
///@endcond
//##############################################################################
/**
 *  @file    ds.h
 *  @author  Lee J. O'Riordan (mlxd)
 *  @date    12/11/2015
 *  @version 0.1
 *
 *  @brief Dastructure for simulation runtime parameters
 *
 *  @section DESCRIPTION
 *  This file keeps track of and generates a parameter file (INI format) of the
 *	simulation parameters used. The resulting file is read in by the Python
 *	post-proc/analysis functions.
 */
 //#############################################################################

#ifndef DS_H
#define DS_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cufft.h>
#include <typeinfo>
#include <iostream>

/*----------------------------------------------------------------------------//
* CLASSES
*-----------------------------------------------------------------------------*/

/**
 * @brief       Class to hold the variable map and grid information
 * @ingroup     data
 */
// NOTE: This is necessary if we ever want to do dynamic grid manipulation.
// NOTE: I do not like this integer double split for retrieval. Find solution.
// NOTE: Consider changing unordered maps to switches for performance
// NOTE: Add FileIO to public functions
class Grid{
    // Here we keep our variable map (unordered for performance)
    // and also grid information. Note that dx dy, and dz are in param_double
    private:
        std::unordered_map<std::string, int> param_int;
        std::unordered_map<std::string, double> param_double;
        std::unordered_map<std::string, double*> param_dstar;

        // List of all strings for parsing into the appropriate param map
        // 1 -> int, 2 -> double, 3 -> double*
        std::unordered_map<std::string, int> id_list;

    // Here we keep the functions to store variables and access grid data
    public:
/*
        // Template for retrieving data
        template <typename arbitrary>
        arbitrary val(std::string id);
*/

        // placing grid parameters in public for now
        double *x, *y, *z, *xp, *yp, *zp;

        // Function to store integer into param_int
        void store(std::string id, int iparam);

        // Function to store double into param_double
        void store(std::string id, double dparam);

        // Function to store double* into param_dstar
        void store(std::string id, double *dsparam);

        // Function to retrieve integer value from param_int
        int ival(std::string id);

        // Function to retrieve double value from param_double
        double dval(std::string id);

        // Function for file writing
        void write(std::string filename);
};
typedef class Grid Grid;

/**
 * @brief        Class to hold CUDA-specific variables and features
 * @ingroup      data
 */
class Cuda{
    private:
        cudaError_t err;
        cufftResult result;
        cufftHandle plan_1d, plan_2d;
        cudaStream_t streamA, streamB, streamC, streamD;
        dim3 grid;
        int threads;
    public:
        // Failed attempt at template functions...
        /*
        // Template for retrieving data
        template <typename arbitrary = int> 
        arbitrary val(std::string id);

        // Function to store data
        template <typename arbitrary = int> 
        void store(std::string id, arbitrary data);
        */

        // Functions to store data
        void store(std::string id, cudaError_t errin);
        void store(std::string id, cufftHandle planin);
        void store(std::string id, cudaStream_t streamin);
        void store(std::string id, dim3 gridin);
        //void store(std::string id, int threadsin);

        // Functions to retrieve data
        cudaError_t cudaError_tval(std::string id);
        cufftResult cufftResultval(std::string id);
        cufftHandle cufftHandleval(std::string id);
        cudaStream_t cudaStream_tval(std::string id);
        dim3 dim3val(std::string id);
        int ival(std::string id);


};
//typedef class Cuda Cuda;

// NOTE: We may need to store the operators as a 3 1d vectors 
//       instead of 1 3d one.

/**
 * @brief       class to hold all necessary information about the operators
 * @ingroup     data
 */
class Op{
    private:
        std::vector<double> potential, kinetic, momentum;
    public:
};
typedef class Op Op;

/**
 * @brief       class to hold all necessary information about the wavefunction
 * @ingroup     data
 */
class Wave{
    public:
        std::vector<double> wfc;
    private:
};
typedef class Wave Wave;

/*----------------------------------------------------------------------------//
* DEPRECATION WARNING
*-----------------------------------------------------------------------------*/
// 
// // Gathers all data from command-line parsing
// struct Param{ 
//     char title[32];
//     double data;
// };
// typedef struct Param Param;
// 
// struct Array{
//     Param *array;
//     size_t length;
//     size_t used;
// };
// typedef struct Array Array;
// 
// /**
// * @brief	Intialises Array to specified length
// * @ingroup	data
// * @param	*arr Pointer to parameter storage Array
// * @param	initLength Length to initialise Array
// */
// void initArr(Array *arr, size_t initLen);
// /**
// * @brief	Adds data to the parameter storage Array
// * @ingroup	data
// * @param	*arr Pointer to parameter storage Array
// * @param	*t Parameter name to be saved
// * @param	d Double value of parameter
// */
// void appendData(Array *arr, std::string t, double d);
// /**
// * @brief	Free all allocated memory from Array
// * @ingroup	data
// * @param	*arr Pointer to parameter storage Array
// */
// void freeArray(Array *arr);
// /**
// * @brief	Allocate new Param. Unused.
// * @ingroup	data
// * @param	*arr Pointer to parameter storage Array
// * @param	*t Parameter name to be saved
// * @param	d Double value of parameter
// */
// Param newParam(std::string t,double d);
// 
/*----------------------------------------------------------------------------//
* END DEPRECATION WARNING
*-----------------------------------------------------------------------------*/

#endif

