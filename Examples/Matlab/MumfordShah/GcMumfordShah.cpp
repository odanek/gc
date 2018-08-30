//
// For given image calculates the Mumford-Shah segmentation using grap-cut based energy
// minimization.
//
// How to compile: 
//   mex -Ipath_to_gc_headers -Lpath_to_gc_lib -lgc_lib_filename GcMumfordShah.cpp
//
// Usage:
//   [seg energy iter c] = GcMumfordShah(img, k, lambda, convergence, max_iter, nb, log);
//

#include "../GcMatlab.h"
#include "../GcMatlabTools.h"
#include <Gc/Energy/Neighbourhood.h>
#include <Gc/Flow/Grid/Kohli.h>
#include <Gc/Algo/Clustering/KMeans.h>
#include <Gc/Algo/Segmentation/MumfordShah.h>
#include <Gc/System/Time/StopWatch.h>

/*************************************************************************************/

// Segment image
template <Gc::Size N, class T>
static void Segment(const mxArray *mx_in, Gc::Size k, T lambda, T conv, 
    Gc::Size max_iter, const char *str_nb, bool log, mxArray *plhs[])
{
    // Turn on logging
    Gc::Examples::Matlab::LogTarget mlt;
    Gc::System::Log::SetTarget(&mlt);
    Gc::System::Time::StopWatch::EnableOutput(log);
    
    // Get image from matlab
    Gc::Data::Image<N,T,T> img;
    Gc::Examples::Matlab::GetImage<N,T,T>(mx_in, img);

    // Set image resolution - unknown, isotropic
    img.SetSpacing(Gc::Math::Algebra::Vector<N,T>(1));

    // Create neighbourhood object
    Gc::Energy::Neighbourhood<N,Gc::Int32> nb((Gc::Size)atoi(str_nb + 1), false);

    // Weights - same weight lambda for all partitions
    Gc::System::Collection::Array<1,T> l(k, lambda);

    // Initialization
    Gc::System::Collection::Array<1,T> c;
    Gc::Algo::Clustering::KMeans::Lloyd(img, k, l, conv, 50, c);

    // Segment
    Gc::System::Collection::Array<N,Gc::Uint8> seg;
    Gc::Flow::Grid::Kohli<N,T,T,T,true> mf;
    T energy = Gc::Algo::Segmentation::MumfordShah::ComputePiecewiseConstant(img, k, l, c,
        conv, max_iter, nb, mf, seg);

    // Save result to matlab
    plhs[0] = mxCreateNumericArray(N, mxGetDimensions(mx_in), mxUINT8_CLASS, mxREAL);
    Gc::Examples::Matlab::SetImage<N,Gc::Uint8,Gc::Uint8>(seg, plhs[0]);

    plhs[1] = mxCreateDoubleScalar((double)energy);
    plhs[2] = mxCreateDoubleScalar((double)max_iter);
    
    plhs[3] = mxCreateDoubleMatrix(1, k, mxREAL);
    double *cv = (double *)mxGetData(plhs[3]);
    for (Gc::Size i = 0; i < k; i++)
    {
        cv[i] = c[i];
    }
}

/*************************************************************************************/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{ 
    // Check params
    if (nrhs != 7)
    {
        mexErrMsgTxt("Seven input arguments required.\nUsage: [seg energy iter c] = "
            "GcMumfordShah(img, k, lambda, convergence, max_iter, nb, log)");
    }

    if (nlhs != 4)
    {
        mexErrMsgTxt("Four output arguments required.\nUsage: [seg energy iter c] = "
            "GcMumfordShah(img, k, lambda, convergence, max_iter, nb, log)");
    }

    // Method parameters
    double lambda, conv;
    Gc::Size max_iter, k;
    char nb[10];
    bool log;

    k = (Gc::Size)Gc::Examples::Matlab::GetNumber(prhs, 1);
    lambda = Gc::Examples::Matlab::GetNumber(prhs, 2);
    conv = Gc::Examples::Matlab::GetNumber(prhs, 3);
    max_iter = (Gc::Size)Gc::Examples::Matlab::GetNumber(prhs, 4);
    Gc::Examples::Matlab::GetString(prhs, 5, nb, 10);
    log = Gc::Examples::Matlab::GetLogical(prhs, 6);

    if (k < 2 || k > 254)
    {
        mexErrMsgTxt("Condition 1 < k < 255 not satisfied."); 
    }
    if (!(lambda > 0 && max_iter > 0))
    {
        mexErrMsgTxt("Parameters lambda and max_iter must be greater than zero."); 
    }
    if (conv < 0)
    {
        mexErrMsgTxt("Convergence criterion must be greater or equal to zero."); 
    }

    // Process input image
    mwSize dim_num = mxGetNumberOfDimensions(prhs[0]);

    try
    {
        if (dim_num == 2)
        {
            if (mxIsSingle(prhs[0]))
            {
                Segment<2,Gc::Float32>(prhs[0], k, Gc::Float32(lambda), Gc::Float32(conv), 
                    max_iter, nb, log, plhs);
            }
            else if (mxIsDouble(prhs[0]))
            {
                Segment<2,Gc::Float64>(prhs[0], k, Gc::Float64(lambda), Gc::Float64(conv), 
                    max_iter, nb, log, plhs);
            }
            else
            {
                mexErrMsgTxt("Unsupported image/voxel type."); 
            }
        }
        else if (dim_num == 3)
        {
            if (mxIsSingle(prhs[0]))
            {
                Segment<3,Gc::Float32>(prhs[0], k, Gc::Float32(lambda), Gc::Float32(conv), 
                    max_iter, nb, log, plhs);
            }
            else if (mxIsDouble(prhs[0]))
            {
                Segment<3,Gc::Float64>(prhs[0], k, Gc::Float64(lambda), Gc::Float64(conv), 
                    max_iter, nb, log, plhs);
            }
            else
            {
                mexErrMsgTxt("Unsupported image/voxel type."); 
            }
        }
        else
        {
            mexErrMsgTxt("Only 2D and 3D images are supported."); 
        }
    }
    catch (const Gc::System::Exception &e)
    {
        mexErrMsgTxt(e.Message().c_str());
    }
}