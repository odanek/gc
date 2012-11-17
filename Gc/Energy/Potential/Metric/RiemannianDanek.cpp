/*
    This file is part of Graph Cut (Gc) combinatorial optimization library.
    Copyright (C) 2008-2010 Centre for Biomedical Image Analysis (CBIA)
    Copyright (C) 2008-2010 Ondrej Danek

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published 
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Gc is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Graph Cut library. If not, see <http://www.gnu.org/licenses/>.
*/

/**
    @file
    Pairwise clique potentials approximating general Riemannian metric
    (O. Danek approximation).

    @author Ondrej Danek <ondrej.danek@gmail.com>
    @date 2010
*/

#include "../../../Math/Constant.h"
#include "../../../Math/Geometry/Voronoi.h"
#include "RiemannianDanek.h"

namespace Gc
{
    namespace Energy
    {
        namespace Potential
        {
            namespace Metric
            {
                /******************************************************************************/

                // Auxiliary method - right hand side of the cauchy-crofton formulas
                /** @cond */
                template <Size N, class T> static inline T CauchyCroftonCoef();

                template <> inline Float32 CauchyCroftonCoef<2,Float32> ()
                {
                    return 2.0f;
                }

                template <> inline Float64 CauchyCroftonCoef<2,Float64> ()
                {
                    return 2.0;
                }

                template <> inline Float32 CauchyCroftonCoef<3,Float32> ()
                {
                    return float(Math::Constant::Pi);
                }

                template <> inline Float64 CauchyCroftonCoef<3,Float64> ()
                {
                    return Math::Constant::Pi;
                }
                /** @endcond */

                /******************************************************************************/

                template <Size N, class T>
                RiemannianDanek<N,T>& RiemannianDanek<N,T>::SetTransformationMatrix
                    (const Math::Algebra::SquareMatrix<N,T> &mt)
                {
                    // Calculate delta phi of the transformed neighbourhood                    
                    System::Collection::Array<1,Math::Algebra::Vector<N,T> > nv(m_nb.Elements());                    
                    
                    for (Size i = 0; i < m_nb.Elements(); i++)
                    {
                        nv[i] = mt.Mul(m_nb[i]).Normalized();
                    }

                    System::Collection::Array<1,T> dphi;
                    Math::Geometry::HypersphereVoronoiDiagram(nv, dphi);

                    // Calculate delta rho and capacities
                    T coef = CauchyCroftonCoef<N,T>();
                    T cell_area = mt.Determinant();
                    
                    for (Size i = 0; i < m_nb.Elements(); i++)
                    {
                        T drho = cell_area / mt.Mul(m_nb[i]).Length();
                        m_rw[i] = (dphi[i] * drho) / coef;
                    }

                    return *this;
                }

                /******************************************************************************/

                // Explicit instantiations
                /** @cond */
                template class GC_DLL_EXPORT RiemannianDanek<2,Float32>;
                template class GC_DLL_EXPORT RiemannianDanek<2,Float64>;
                template class GC_DLL_EXPORT RiemannianDanek<3,Float32>;
                template class GC_DLL_EXPORT RiemannianDanek<3,Float64>;
                /** @endcond */

                /******************************************************************************/
            }
        }
    }
}
