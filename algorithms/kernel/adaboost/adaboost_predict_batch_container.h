/* file: adaboost_predict_batch_container.h */
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
//  Implementation of Ada Boost prediction algorithm container --
//  a class that contains Fast Ada Boost kernels for supported architectures.
//--
*/

#ifndef __ADABOOST_PREDICT_BATCH_CONTAINER__
#define __ADABOOST_PREDICT_BATCH_CONTAINER__

#include "adaboost_predict.h"
#include "adaboost_predict_kernel.h"

namespace daal
{
namespace algorithms
{
namespace adaboost
{
namespace prediction
{
template<typename algorithmFPType, Method method, CpuType cpu>
PredictionContainer<algorithmFPType, method, cpu>::PredictionContainer(daal::services::Environment::env *daalEnv)
{
    __DAAL_INTIALIZE_KERNELS(internal::AdaBoostPredictKernel, method, algorithmFPType);
}

template<typename algorithmFPType, Method method, CpuType cpu>
PredictionContainer<algorithmFPType, method, cpu>::~PredictionContainer()
{
    __DAAL_DEINTIALIZE_KERNELS();
}

template<typename algorithmFPType, Method method, CpuType cpu>
void PredictionContainer<algorithmFPType, method, cpu>::compute()
{
    classifier::prediction::Input *input = static_cast<classifier::prediction::Input *>(_in);
    classifier::prediction::Result *result = static_cast<classifier::prediction::Result *>(_res);

    services::SharedPtr<NumericTable> a = input->get(classifier::prediction::data);
    adaboost::Model *m = static_cast<adaboost::Model *>(input->get(classifier::prediction::model).get());
    services::SharedPtr<NumericTable> r = result->get(classifier::prediction::prediction);
    adaboost::Parameter *par = static_cast<adaboost::Parameter *>(_par);

    daal::services::Environment::env &env = *_env;
    __DAAL_CALL_KERNEL(env, internal::AdaBoostPredictKernel, __DAAL_KERNEL_ARGUMENTS(method, algorithmFPType), compute, a, m, r, par);
}

} // namespace daal::algorithms::adaboost::prediction
}
}
} // namespace daal

#endif // __ADABOOST_PREDICT_BATCH_CONTAINER__
