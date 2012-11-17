#ifndef GC_EXAMPLES_MATLAB_MATLAB_H
#define GC_EXAMPLES_MATLAB_MATLAB_H

#if (_MSC_VER >= 1600)
    // Avoid char16_t conflict with Visual Studio 2010 compiler
    // Can be removed later when MATLAB fixes this
    #include <yvals.h>
    #define CHAR16_T
#endif

#include "mex.h"
#include <Gc/System/Exception.h>
#include <Gc/System/Format.h>
#include <Gc/System/Log.h>
#include <Gc/Data/Image.h>

namespace Gc
{
    namespace Examples
    {
        namespace Matlab
        {
            /*************************************************************************************/

            // Gc log redirection to matlab
            class LogTarget
                : public System::Log::ITarget
            {
            public:
                virtual void Message(const std::string &str)
                {
                    mexPrintf("%s\n", str.c_str());
                }

                virtual void Warning(const std::string &str)
                {
                    mexPrintf("%s\n", str.c_str());
                }

                virtual void Error(const std::string &str)
                {
                    mexPrintf("%s\n", str.c_str());
                }
            };

            /*************************************************************************************/

            // Convert matlab image to the format of the Gc library
            template <Size N, class GC_DATA, class MX_DATA>
            static void GetImage(const mxArray *mx_in, System::Collection::Array<N,GC_DATA> &img)
            {
                // Read image dimensions - MATLAB uses YXZ axis order
                Math::Algebra::Vector<N,Size> img_dim;
                const mwSize *mx_dim = mxGetDimensions(mx_in);

                img_dim[0] = mx_dim[1];
                img_dim[1] = mx_dim[0];
                
                for (Size i = 2; i < N; i++)
                {
                    img_dim[i] = mx_dim[i];
                }
                
                // Create image
                img.Resize(img_dim);
                                
                // Read data - we have to swap X and Y                
                Size slice_sz = img_dim[0] * img_dim[1];
                Size dim_wo_xy = img.Elements() / slice_sz;
                const MX_DATA *din = (const MX_DATA *)mxGetData(mx_in);
                GC_DATA *dout = img.Begin();
                
                for (Size i = 0; i < dim_wo_xy; i++)
                {
                    for (Size x = 0; x < img_dim[0]; x++)
                    {
                        for (Size y = 0; y < img_dim[1]; y++)
                        {
                            *dout = GC_DATA(*din);
                            dout += img_dim[0];
                            din++;
                        }
                        
                        dout -= slice_sz - 1;
                    }
                    
                    dout += slice_sz - img_dim[0];
                }
            }

            /*************************************************************************************/

            // Convert G image to the format of the Gc library
            template <Size N, class GC_DATA, class MX_DATA>
            static void SetImage(const System::Collection::Array<N,GC_DATA> &img, mxArray *mx_out)
            {
                // Write data - we have to swap X and Y
                Size slice_sz = img.Dimensions()[0] * img.Dimensions()[1];
                Size dim_wo_xy = img.Elements() / slice_sz;
                MX_DATA *dout = (MX_DATA *)mxGetData(mx_out);
                const GC_DATA *din = img.Begin();
                
                for (Size i = 0; i < dim_wo_xy; i++)
                {                    
                    for (Size y = 0; y < img.Dimensions()[1]; y++)
                    {
                        for (Size x = 0; x < img.Dimensions()[0]; x++)
                        {
                            *dout = MX_DATA(*din);
                            dout += img.Dimensions()[1];
                            din++;
                        }
                        
                        dout -= slice_sz - 1;
                    }
                    
                    dout += slice_sz - img.Dimensions()[1];
                }
            }
            
            /*************************************************************************************/

            // Get string from matlab
            void GetString(const mxArray*prhs[], size_t idx, char *str, size_t strlen)
            {
                if (!mxIsChar(prhs[idx]))
                {
                    char err_str[200];
                    sprintf(err_str, "Parameter %lu must be a text string.", (unsigned long)(idx + 1));
                    mexErrMsgTxt(err_str);
                }

                mxGetString(prhs[idx], str, strlen);
            }

            /*************************************************************************************/

            // Get scalar number parameter from matlab
            double GetNumber(const mxArray*prhs[], size_t idx)
            {
                if (!mxIsDouble(prhs[idx]))
                {
                    char err_str[200];
                    sprintf(err_str, "Parameter %lu must be of numeric type double.", (unsigned long)(idx + 1));
                    mexErrMsgTxt(err_str);
                }

                return *mxGetPr(prhs[idx]);
            }

            /*************************************************************************************/

            // Get scalar number parameter from matlab
            bool GetLogical(const mxArray*prhs[], size_t idx)
            {
                if (!mxIsLogicalScalar(prhs[idx]))
                {
                    char err_str[200];
                    sprintf(err_str, "Parameter %lu must be a logical value.", (unsigned long)(idx + 1));
                    mexErrMsgTxt(err_str);
                }

                return *((bool *)mxGetData(prhs[idx]));
            }

            /*************************************************************************************/
        }
    }
}

#endif
