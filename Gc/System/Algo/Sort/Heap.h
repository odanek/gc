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
    Heap sort algorithm.

    @author Ondrej Danek <ondrej.danek@gmail.com>
    @date 2010
*/

#ifndef GC_SYSTEM_ALGO_SORT_HEAP_H
#define GC_SYSTEM_ALGO_SORT_HEAP_H

#include "../../../Type.h"
#include "../Iterator.h"

namespace Gc
{
    namespace System
    {
        namespace Algo
        {
            /** %Data sorting algorithms. */
            namespace Sort
            {
                /** Heap sort algorithm. 
                
                    Sorts elements from the smallest to the largest using heap sort
                    algorithm.

                    @param[in] begin Random access iterator pointing to the first element.
                    @param[in] end Random access iterator pointing to the end of the sequence.
                */
                template <class RndAccIter>
                void Heap(RndAccIter begin, RndAccIter end)
                {
                    Size n = (end - begin), parent = n/2, index, child;
                    typename IteratorTraits<RndAccIter>::ValueType temp; 

                    while (n > 1)
                    { 
                        if (parent) 
                        { 
                            temp = begin[--parent];
                        } 
                        else 
                        {            
                            n--;
                            temp = begin[n];
                            begin[n] = begin[0];
                        }

                        index = parent;
                        child = index * 2 + 1;
                        while (child < n) 
                        {
                            if (child + 1 < n && begin[child + 1] > begin[child]) 
                            {
                                child++;
                            }

                            if (begin[child] > temp) 
                            {
                                begin[index] = begin[child];
                                index = child;
                                child = index * 2 + 1;
                            } 
                            else 
                            {
                                break;
                            }
                        }

                        begin[index] = temp; 
                    }
                }

                /** Simultaneous heap sort algorithm. 
                
                    Sorts elements of the first array from the smallest to the largest 
                    using heap sort algorithm and simultaneously rearranges elements
                    in the second collection.

                    @param[in] d1Beg Random access iterator pointing to the first element
                        in the first collection (to be sorted).
                    @param[in] d1End Random access iterator to the end of the first sequence.
                    @param[in] d2Beg Random access iterator pointing to the first element
                        in the second collection.
                */
                template <class RndAccIter1, class RndAccIter2>
                void HeapSimultaneous(RndAccIter1 d1Beg, RndAccIter1 d1End, RndAccIter2 d2Beg)
                {
                    // Sort
                    Size n = (d1End - d1Beg), parent = n/2, index, child;
                    typename IteratorTraits<RndAccIter1>::ValueType temp1; 
                    typename IteratorTraits<RndAccIter2>::ValueType temp2;

                    while (n > 1)
                    { 
                        if (parent) 
                        { 
                            parent--;
                            temp1 = d1Beg[parent];
                            temp2 = d2Beg[parent];
                        } 
                        else 
                        {
                            n--;
                            temp1 = d1Beg[n];
                            temp2 = d2Beg[n];
                            d1Beg[n] = d1Beg[0];
                            d2Beg[n] = d2Beg[0];
                        }

                        index = parent;
                        child = index * 2 + 1;
                        while (child < n) 
                        {
                            if (child + 1 < n && d1Beg[child + 1] > d1Beg[child]) 
                            {
                                child++;
                            }

                            if (d1Beg[child] > temp1) 
                            {
                                d1Beg[index] = d1Beg[child];
                                d2Beg[index] = d2Beg[child];
                                index = child;
                                child = index * 2 + 1;
                            } 
                            else 
                            {
                                break;
                            }
                        }

                        d1Beg[index] = temp1; 
                        d2Beg[index] = temp2;
                    }
                }
            }
        }
    }
}

#endif
