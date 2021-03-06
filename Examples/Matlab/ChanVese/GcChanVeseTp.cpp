//
// For given image and initial interface calculates a topology preserving Chan-Vese segmentation 
// using grap-cut based energy minimization. 
//
// How to compile: 
//   mex -Ipath_to_gc_headers -Lpath_to_gc_lib -lgc_lib_filename GcChanVeseTp.cpp
//
// Usage:
//   [seg energy iter c1 c2] = GcChanVeseTp(img, interface, lambda1, lambda2, c1, c2, convergence, 
//      max_iter, nb, log, [mask]);
//

#include "../GcMatlab.h"
#include "../GcMatlabTools.h"
#include <Gc/Energy/Neighbourhood.h>
#include <Gc/Flow/Grid/ZengDanek.h>
#include <Gc/Algo/Segmentation/ChanVese.h>

/*************************************************************************************/

// Segment image
template <Gc::Size N, class T>
static void Segment(const mxArray *mx_in, const mxArray *mx_interface, const mxArray *mx_mask, 
    T l1, T l2, T c1, T c2, T conv, Gc::Size max_iter, const char *str_nb, bool log, 
    mxArray *plhs[])
{
    // Turn on logging
    Gc::Examples::Matlab::LogTarget mlt;
    Gc::System::Log::SetTarget(&mlt);
    Gc::System::Time::StopWatch::EnableOutput(log);
    
    // Get image from matlab
    Gc::Data::Image<N,T,T> img;
    Gc::Examples::Matlab::GetImage<N,T,T>(mx_in, img);

    // Set image resolution - unknown
    img.SetSpacing(Gc::Math::Algebra::Vector<N,T>(1));

    // Get initial interface from matlab
    Gc::System::Collection::Array<N,bool> iface;
    Gc::Examples::Matlab::GetImage<N,bool,bool>(mx_interface, iface);

    if (iface.Dimensions() != img.Dimensions())
    {
        mexErrMsgTxt("Initial interface and image must have the same dimensions.");
    }

    // Get mask from matlab
    Gc::System::Collection::Array<N,Gc::Uint8> mask;
    if (mx_mask != NULL)
    {
        Gc::Examples::Matlab::GetImage<N,Gc::Uint8,Gc::Uint8>(mx_mask, mask);

        if (mask.Dimensions() != img.Dimensions())
        {
            mexErrMsgTxt("Mask and image must have the same dimensions.");
        }
    }

    // Create neighbourhood object
    Gc::Energy::Neighbourhood<N,Gc::Int32> nb;
    Gc::Examples::Matlab::CreateNeighbourhood(str_nb, nb);

    // Segment
    Gc::System::Collection::Array<N,bool> seg;
    T energy;

    // Topology preserving max-flow
    if (mx_mask == NULL)
    {
        Gc::Flow::Grid::ZengDanek<N,T,T,T,false> mf;
        mf.SetInitialLabelingRef(iface);

        energy = Gc::Algo::Segmentation::ChanVese::Compute(img, l1, l2, c1, c2, 
            conv, max_iter, nb, mf, seg);
    }
    else
    {
        Gc::Flow::Grid::ZengDanek<N,T,T,T,true> mf;
        mf.SetInitialLabelingRef(iface);

        energy = Gc::Algo::Segmentation::ChanVese::ComputeMasked(img, mask, l1, 
            l2, c1, c2, conv, max_iter, nb, mf, seg);
    }

    // Save result to matlab
    plhs[0] = mxCreateLogicalArray(N, mxGetDimensions(mx_in));
    Gc::Examples::Matlab::SetImage<N,bool,bool>(seg, plhs[0]);

    plhs[1] = mxCreateDoubleScalar((double)energy);
    plhs[2] = mxCreateDoubleScalar((double)max_iter);
    plhs[3] = mxCreateDoubleScalar((double)c1);
    plhs[4] = mxCreateDoubleScalar((double)c2);
}

/*************************************************************************************/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{ 
    // Check params
    if (nrhs < 10 || nrhs > 11)
    {
        mexErrMsgTxt("Ten or eleven input arguments required.\nUsage: [seg energy iter c1 c2] = "
            "GcChanVeseTp(img, interface, lambda1, lambda2, c1, c2, convergence, max_iter, "
            "nb, log, [mask])"); 
    }

    if (nlhs != 5)
    {
        mexErrMsgTxt("Five output argument required.\nUsage: [seg energy iter c1 c2] = "
            "GcChanVeseTp(img, interface, lambda1, lambda2, c1, c2, convergence, "
            "max_iter, nb, log)");
    }

    // Method parameters
    double l1, l2, c1, c2, conv;
    Gc::Size max_iter;
    char nb[10];
    bool log;

    l1 = Gc::Examples::Matlab::GetNumber(prhs, 2);
    l2 = Gc::Examples::Matlab::GetNumber(prhs, 3);
    c1 = Gc::Examples::Matlab::GetNumber(prhs, 4);
    c2 = Gc::Examples::Matlab::GetNumber(prhs, 5);
    conv = Gc::Examples::Matlab::GetNumber(prhs, 6);
    max_iter = (Gc::Size)Gc::Examples::Matlab::GetNumber(prhs, 7);
    Gc::Examples::Matlab::GetString(prhs, 8, nb, 10);
    log = Gc::Examples::Matlab::GetLogical(prhs, 9);

    if (!(l1 > 0 && l2 > 0 && max_iter > 0))
    {
        mexErrMsgTxt("Parameters lambda1, lambda2 and max_iter must be greater than zero."); 
    }
    if (conv < 0)
    {
        mexErrMsgTxt("Convergence criterion must be greater or equal to zero."); 
    }
    if (c1 >= c2)
    {
        mexErrMsgTxt("Condition c1 < c2 is not satisfied."); 
    }

    // Process input image
    mwSize dim_num = mxGetNumberOfDimensions(prhs[0]);
    
    if (dim_num != mxGetNumberOfDimensions(prhs[1]) || !mxIsLogical(prhs[1]))
    {
        mexErrMsgTxt("Initial interface and image must have the same dimensions. Interface must be of logical type.");
    }

    if (nrhs == 11)
    {
        if (dim_num != mxGetNumberOfDimensions(prhs[10]) || !mxIsUint8(prhs[10]))
        {
            mexErrMsgTxt("Mask and image must have the same dimensions. Mask data type must be uint8.");
        }
    }

    try
    {
        if (dim_num == 2)
        {
            if (mxIsSingle(prhs[0]))
            {
                Segment<2,Gc::Float32>(prhs[0], prhs[1], (nrhs == 11) ? prhs[10] : NULL,
                    Gc::Float32(l1), Gc::Float32(l2), Gc::Float32(c1), 
                    Gc::Float32(c2), Gc::Float32(conv), max_iter, nb, log, plhs);
            }
            else if (mxIsDouble(prhs[0]))
            {
                Segment<2,Gc::Float64>(prhs[0], prhs[1], (nrhs == 11) ? prhs[10] : NULL,
                    l1, l2, c1, c2, conv, max_iter, nb, log, plhs);
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
                Segment<3,Gc::Float32>(prhs[0], prhs[1], (nrhs == 11) ? prhs[10] : NULL,
                    Gc::Float32(l1), Gc::Float32(l2), Gc::Float32(c1), 
                    Gc::Float32(c2), Gc::Float32(conv), max_iter, nb, log, plhs);
            }
            else if (mxIsDouble(prhs[0]))
            {
                Segment<3,Gc::Float64>(prhs[0], prhs[1], (nrhs == 11) ? prhs[10] : NULL,
                    l1, l2, c1, c2, conv, max_iter, nb, log, plhs);
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