#ifndef GC_EXAMPLES_MATLAB_MATLABTOOLS_H
#define GC_EXAMPLES_MATLAB_MATLABTOOLS_H

#include "GcMatlab.h"
#include <Gc/Flow/General/FordFulkerson.h>
#include <Gc/Flow/General/EdmondsKarp.h>
#include <Gc/Flow/General/Dinitz.h>
#include <Gc/Flow/General/BoykovKolmogorov.h>
#include <Gc/Flow/General/Kohli.h>
#include <Gc/Flow/General/PushRelabel/Fifo.h>
#include <Gc/Flow/General/PushRelabel/HighestLevel.h>
#include <Gc/Flow/Grid/Kohli.h>
#include <Gc/Flow/Grid/PushRelabel/Fifo.h>
#include <Gc/Flow/Grid/PushRelabel/HighestLevel.h>

namespace Gc
{
    namespace Examples
    {
        namespace Matlab
        {
            /*************************************************************************************/

            // Create general max-flow algorithm corresponging to given string identifier
            template <class T>
            static Gc::Flow::IMaxFlow<T,T,T> *CreateGeneralMaxFlow(const char *name)
            {
                if (!strcmp(name, "GEN-FF"))
                {
                    return new Gc::Flow::General::FordFulkerson<T,T>;
                }
                else if (!strcmp(name, "GEN-EK"))
                {
                    return new Gc::Flow::General::EdmondsKarp<T,T>;
                }
                else if (!strcmp(name, "GEN-DI"))
                {
                    return new Gc::Flow::General::Dinitz<T,T>;
                }
                if (!strcmp(name, "GEN-BK"))
                {
                    return new Gc::Flow::General::BoykovKolmogorov<T,T,T>;
                }
                else if (!strcmp(name, "GEN-KO"))
                {
                    return new Gc::Flow::General::Kohli<T,T,T>;        
                }
                else if (!strcmp(name, "GEN-PRF"))
                {
                    return new Gc::Flow::General::PushRelabel::Fifo<T,T>;
                }
                else if (!strcmp(name, "GEN-PRH"))
                {
                    return new Gc::Flow::General::PushRelabel::HighestLevel<T,T>;
                }

                mexErrMsgTxt("Unknown general max-flow algorithm.");
                return NULL;
            }

            /*************************************************************************************/

            // Create general max-flow algorithm corresponging to given string identifier
            template <Gc::Size N, class T>
            static Gc::Flow::IGridMaxFlow<N,T,T,T> *CreateGridMaxFlow(const char *name, bool mask)
            {
                if (!strcmp(name, "GRD-KO"))
                {
                    if (mask)
                    {
                        return new Gc::Flow::Grid::Kohli<N,T,T,T,true>;
                    }
                    else
                    {
                        return new Gc::Flow::Grid::Kohli<N,T,T,T,false>;
                    }
                }
                else if (!strcmp(name, "GRD-PRF"))
                {
                    if (mask)
                    {
                        return new Gc::Flow::Grid::PushRelabel::Fifo<N,T,T,true>;
                    }
                    else
                    {
                        return new Gc::Flow::Grid::PushRelabel::Fifo<N,T,T,false>;
                    }
                }
                else if (!strcmp(name, "GRD-PRH"))
                {
                    if (mask)
                    {
                        return new Gc::Flow::Grid::PushRelabel::HighestLevel<N,T,T,true>;
                    }
                    else
                    {
                        return new Gc::Flow::Grid::PushRelabel::HighestLevel<N,T,T,false>;
                    }
                }

                mexErrMsgTxt("Unknown grid max-flow algorithm.");
                return NULL;
            }

            /*************************************************************************************/
        }
    }
}

#endif