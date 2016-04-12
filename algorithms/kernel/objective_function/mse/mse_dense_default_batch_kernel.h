/* file: mse_dense_default_batch_kernel.h */
/*******************************************************************************
* Copyright 2014-2016 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

//++
//  Declaration of template function that calculate mse.
//--


#ifndef __MSE_DENSE_DEFAULT_BATCH_KERNEL_H__
#define __MSE_DENSE_DEFAULT_BATCH_KERNEL_H__

#include "mse_batch.h"
#include "kernel.h"
#include "numeric_table.h"

using namespace daal::data_management;

namespace daal
{
namespace algorithms
{
namespace optimization_solver
{
namespace mse
{
namespace internal
{
/**
 *  \brief Kernel for mse calculation
 *  in case floating point type of intermediate calculations
 *  and method of calculations are different
 */
template<typename algorithmFPType, Method method, CpuType cpu>
class MSEKernel : public Kernel
{
public:
    void compute(Input *input, objective_function::Result *result, Parameter *parameter);
private:
    void computeMSE(
    size_t batchSize, size_t nTheta, bool valueFlag, bool hessianFlag, bool gradientFlag,
    algorithmFPType *data,
    algorithmFPType *theta,
    algorithmFPType *theta0,
    algorithmFPType *dependentVariablesArray,
    algorithmFPType *value,
    algorithmFPType *gradient,
    algorithmFPType *hessian,
    algorithmFPType *x_th);
};

} // namespace daal::internal

} // namespace mse

} // namespace optimization_solver

} // namespace algorithms

} // namespace daal

#endif
