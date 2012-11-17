//
// Calculate distance transform.
//
// How to compile: 
//   mex -Ipath_to_gc_headers -Lpath_to_gc_lib -lgc_lib_filename GcDistTransform.cpp
//
// Usage:
//   dmap = GcDistTransform(img, zero_val, metric);
//

#include "../GcMatlab.h"
#include <Gc/Algo/Geometry/DistanceTransform.h>

/*************************************************************************************/

// Do segmentation
template <Gc::Size N, class T>
static void DistTransform(const mxArray *mx_img, const mxArray *mx_zero_val,
    const char *metric, mxArray **mx_out)
{
    // Get image from matlab
    Gc::System::Collection::Array<N,T> img;
    Gc::Examples::Matlab::GetImage<N,T,T>(mx_img, img);
    
    // Get zero val
    T zero_val = *((T *)mxGetData(mx_zero_val));

    // Compute distance transform
    Gc::System::Collection::Array<N,Gc::Uint32> dmap;

    if (!strcmp(metric, "cityblock"))
    {
        Gc::Algo::Geometry::DistanceTransform::CityBlock(img, zero_val, dmap);
    }
    else if (!strcmp(metric, "chessboard"))
    {
        Gc::Algo::Geometry::DistanceTransform::ChessBoard(img, zero_val, dmap);
    }
    else
    {
        mexErrMsgTxt("Unknown metric type.");
    }

    // Save result to matlab
    *mx_out = mxCreateNumericArray(N, mxGetDimensions(mx_img), mxUINT32_CLASS, mxREAL);
    Gc::Examples::Matlab::SetImage<N,Gc::Uint32,Gc::Uint32>(dmap, *mx_out);
}

/*************************************************************************************/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{ 
    // Check params
    if (nrhs != 3)
    {
        mexErrMsgTxt("Three input arguments required.\nUsage: dmap = GcDistTransform(img, zero_val, metric)"); 
    }
    if (nlhs != 1)
    {
        mexErrMsgTxt("One output argument required.\nUsage: dmap = GcDistTransform(img, zero_val, metric)"); 
    }

    // Method parameters
    char metric[30];
    Gc::Examples::Matlab::GetString(prhs, 2, metric, 30);

    // Process input image
    mwSize dim_num = mxGetNumberOfDimensions(prhs[0]);

    try
    {
        if (dim_num == 2)
        {
            if (mxIsLogical(prhs[0]))
            {
                if (!mxIsLogical(prhs[1]))
                {
                    mexErrMsgTxt("Image and zero value types don't match.");
                }
                DistTransform<2,bool>(prhs[0], prhs[1], metric, plhs);
            }
            else if (mxIsUint8(prhs[0]))
            {
                if (!mxIsUint8(prhs[1]))
                {
                    mexErrMsgTxt("Image and zero value types don't match.");
                }
                DistTransform<2,Gc::Uint8>(prhs[0], prhs[1], metric, plhs);
            }
            else
            {
                mexErrMsgTxt("Unsupported image/voxel type."); 
            }
        }
        else if (dim_num == 3)
        {
            if (mxIsLogical(prhs[0]))
            {
                if (!mxIsLogical(prhs[1]))
                {
                    mexErrMsgTxt("Image and zero value types don't match.");
                }
                DistTransform<3,bool>(prhs[0], prhs[1], metric, plhs);
            }
            else if (mxIsUint8(prhs[0]))
            {
                if (!mxIsUint8(prhs[1]))
                {
                    mexErrMsgTxt("Image and zero value types don't match.");
                }
                DistTransform<3,Gc::Uint8>(prhs[0], prhs[1], metric, plhs);
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