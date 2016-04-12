/* file: em_gmm_init_dense_default_batch_container.h */
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

/*
//++
//  Implementation of EM calculation algorithm container.
//--
*/

#ifndef __EM_GMM_INIT_DENSE_DEFAULT_BATCH_CONTAINER_H__
#define __EM_GMM_INIT_DENSE_DEFAULT_BATCH_CONTAINER_H__

#include "em_gmm_init_dense_default_batch_kernel.h"

namespace daal
{
namespace algorithms
{
namespace em_gmm
{
namespace init
{

/**
 *  \brief Initialize list of em default init kernels with implementations for supported architectures
 */
template<typename algorithmFPType, Method method, CpuType cpu>
BatchContainer<algorithmFPType, method, cpu>::BatchContainer(daal::services::Environment::env *daalEnv)
{
    __DAAL_INTIALIZE_KERNELS(internal::EMInitKernel, algorithmFPType, method);
}

template<typename algorithmFPType, Method method, CpuType cpu>
BatchContainer<algorithmFPType, method, cpu>::~BatchContainer()
{
    __DAAL_DEINTIALIZE_KERNELS();
}

template<typename algorithmFPType, Method method, CpuType cpu>
void BatchContainer<algorithmFPType, method, cpu>::compute()
{
    Input  *input = static_cast<Input *>(_in);
    Result *pRes   = static_cast<Result *>(_res);

    Parameter *emPar = static_cast<Parameter *>(_par);

    services::SharedPtr<NumericTable>   inputData        = input->get(data);
    services::SharedPtr<NumericTable>   inputWeights     = pRes->get(weights);
    services::SharedPtr<NumericTable>   inputMeans       = pRes->get(means);
    services::SharedPtr<data_management::DataCollection> inputCovariances = pRes->get(covariances);

    daal::services::Environment::env &env = *_env;

    __DAAL_CALL_KERNEL(env, internal::EMInitKernel, __DAAL_KERNEL_ARGUMENTS(algorithmFPType, method),
                       compute, inputData, inputWeights, inputMeans, inputCovariances, emPar);

}

} // namespace init

} // namespace em_gmm

} // namespace algorithms

} // namespace daal

#endif
