//
// For given image calculates the Rousson-Deriche segmentation using grap-cut based energy
// minimization.
//
// How to compile: 
//   mex -Ipath_to_gc_headers -Lpath_to_gc_lib -lgc_lib_filename GcRoussonDeriche.cpp
//
// Usage:
//   [seg energy iter c] = GcRoussonDeriche(img, lambda, convergence, max_iter, nb, max_flow, log);
//

#include "../GcMatlab.h"
#include "../GcMatlabTools.h"
#include <Gc/Energy/Neighbourhood.h>
#include <Gc/Algo/Segmentation/RoussonDeriche.h>
#include <Gc/System/Time/StopWatch.h>

/*************************************************************************************/

// Segment image
template <Gc::Size N, class T>
static void Segment(const mxArray *mx_in, T lambda, T conv, Gc::Size max_iter, 
    const char *str_nb, const char *str_mf, bool log, mxArray *plhs[])
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

    // Initialization
    Gc::Algo::Segmentation::RoussonDeriche::Params<T> rdpar;
    Gc::Algo::Segmentation::RoussonDeriche::InitialEstimate(img, 50, rdpar);

    // Grid max-flow
    Gc::Flow::IGridMaxFlow<N,T,T,T> *mf = 
        Gc::Examples::Matlab::CreateGridMaxFlow<N,T>(str_mf, false);

    // Segment
    Gc::System::Collection::Array<N,bool> seg;
    T energy = Gc::Algo::Segmentation::RoussonDeriche::Compute(img, lambda, rdpar,
        conv, max_iter, nb, *mf, seg);

    delete mf;

    // Save result to matlab
    plhs[0] = mxCreateLogicalArray(N, mxGetDimensions(mx_in));
    Gc::Examples::Matlab::SetImage<N,bool,bool>(seg, plhs[0]);

    plhs[1] = mxCreateDoubleScalar((double)energy);
    plhs[2] = mxCreateDoubleScalar((double)max_iter);
    plhs[3] = mxCreateDoubleScalar((double)rdpar.m_c1);
    plhs[4] = mxCreateDoubleScalar((double)rdpar.m_v1);
    plhs[5] = mxCreateDoubleScalar((double)rdpar.m_c2);
    plhs[6] = mxCreateDoubleScalar((double)rdpar.m_v2);
}

/*************************************************************************************/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{ 
    // Check params
    if (nrhs != 7)
    {
        mexErrMsgTxt("Seven input arguments required.\nUsage: [seg energy iter c1 v1 c2 v2] = "
            "GcRoussonDeriche(img, lambda, convergence, max_iter, nb, max_flow, log)");
    }

    if (nlhs != 7)
    {
        mexErrMsgTxt("Seven output arguments required.\nUsage: [seg energy iter c1 v1 c2 v2] = "
            "GcRoussonDeriche(img, lambda, convergence, max_iter, nb, max_flow, log)");
    }

    // Method parameters
    double lambda, conv;
    Gc::Size max_iter;
    char nb[10], mf[10];
    bool log;

    lambda = Gc::Examples::Matlab::GetNumber(prhs, 1);
    conv = Gc::Examples::Matlab::GetNumber(prhs, 2);
    max_iter = (Gc::Size)Gc::Examples::Matlab::GetNumber(prhs, 3);
    Gc::Examples::Matlab::GetString(prhs, 4, nb, 10);
    Gc::Examples::Matlab::GetString(prhs, 5, mf, 10);
    log = Gc::Examples::Matlab::GetLogical(prhs, 6);

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
                Segment<2,Gc::Float32>(prhs[0], Gc::Float32(lambda), Gc::Float32(conv), 
                    max_iter, nb, mf, log, plhs);
            }
            else if (mxIsDouble(prhs[0]))
            {
                Segment<2,Gc::Float64>(prhs[0], Gc::Float64(lambda), Gc::Float64(conv), 
                    max_iter, nb, mf, log, plhs);
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
                Segment<3,Gc::Float32>(prhs[0], Gc::Float32(lambda), Gc::Float32(conv), 
                    max_iter, nb, mf, log, plhs);
            }
            else if (mxIsDouble(prhs[0]))
            {
                Segment<3,Gc::Float64>(prhs[0], Gc::Float64(lambda), Gc::Float64(conv), 
                    max_iter, nb, mf, log, plhs);
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