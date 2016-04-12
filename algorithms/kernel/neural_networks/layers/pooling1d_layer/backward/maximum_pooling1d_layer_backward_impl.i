/* file: maximum_pooling1d_layer_backward_impl.i */
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
//  Implementation of backward pooling layer
//--
*/

#ifndef __MAXIMUM_POOLING1D_LAYER_BACKWARD_IMPL_I__
#define __MAXIMUM_POOLING1D_LAYER_BACKWARD_IMPL_I__

#include "service_memory.h"
#include "service_blas.h"

using namespace daal::services;

namespace daal
{
namespace algorithms
{
namespace neural_networks
{
namespace layers
{
namespace maximum_pooling1d
{
namespace backward
{
namespace internal
{

template<typename algorithmFPType, Method method, CpuType cpu>
void PoolingKernel<algorithmFPType, method, cpu>::compute(
    const maximum_pooling1d::backward::Input *input, const maximum_pooling1d::Parameter *parameter,
    maximum_pooling1d::backward::Result *result)
{
    const algorithmFPType zero = 0.0;

    SharedPtr<Tensor> inputTensor = input->get(layers::backward::inputGradient);
    SharedPtr<Tensor> maskTensor = input->get(auxMask);
    SharedPtr<Tensor> gradTensor = result->get(layers::backward::gradient);

    const Collection<size_t> &inputDims = inputTensor->getDimensions();
    const Collection<size_t> &gradDims = maskTensor->getDimensions();
    SubtensorDescriptor<algorithmFPType> inputBlock, maskBlock, gradBlock;

    inputTensor->getSubtensor(0, 0, 0, inputDims[0], readOnly, inputBlock);
    maskTensor->getSubtensor(0, 0, 0, gradDims[0], readOnly, maskBlock);
    gradTensor->getSubtensor(0, 0, 0, gradDims[0], writeOnly, gradBlock);

    algorithmFPType *inputGrad = inputBlock.getPtr();
    algorithmFPType *mask = maskBlock.getPtr();
    algorithmFPType *grad = gradBlock.getPtr();

    size_t gradSize = gradBlock.getSize();
    daal::services::internal::service_memset<algorithmFPType, cpu>(grad, zero, gradSize);

    size_t firstIndex    = parameter->indices.size[0];
    size_t firstPadding  = parameter->padding.size[0];
    size_t firstStride   = parameter->stride.size[0];
    MKL_INT firstKernelSize  = (MKL_INT)(parameter->kernelSize.size[0]);

    /*
     * Mask tensor is viewed by this method as a 5-dimensional tensor of size:
     * offsetBefore * firstSize * offsetAfter
     */
    size_t offsetBefore = 1;
    for (size_t i = 0; i < firstIndex; i++)
    {
        offsetBefore *= gradDims[i];
    }
    size_t firstSize = gradDims[firstIndex];
    size_t firstOutSize = inputDims[firstIndex];
    size_t offsetAfter = 1;
    for (size_t i = firstIndex + 1; i < gradDims.size(); i++)
    {
        offsetAfter *= gradDims[i];
    }

    for (MKL_INT i = 0; i < (MKL_INT)offsetBefore; i++)
    {
        /*
         * Loop by the first kernel dimension
         * f - index of the left upper corner of the kernel
         * fo - index of the output value
         */
        for (MKL_INT f = -firstPadding, fo = 0; fo < (MKL_INT)firstOutSize; f += firstStride, fo++)
        {
            for (MKL_INT j = 0; j < (MKL_INT)offsetAfter; j++)
            {
                /*
                 * Input value index
                 */
                MKL_INT inputIndex = j + offsetAfter * (fo + firstOutSize * i);

                /*
                 * Loops over the kernel
                 */
                for (MKL_INT fi = f; fi < f + firstKernelSize; fi++)
                {
                    MKL_INT gradIndex = j + offsetAfter * (fi + firstSize * i);
                    bool paddingFlag = (fi < 0) || (fi >= (MKL_INT)firstSize);

                    if (!paddingFlag && (mask[gradIndex] > zero))
                    {
                        grad[gradIndex] += inputGrad[inputIndex];
                    }
                }
            }
        }
    }

    inputTensor->releaseSubtensor(inputBlock);
    maskTensor->releaseSubtensor(maskBlock);
    gradTensor->releaseSubtensor(gradBlock);
}

} // namespace internal
} // namespace backward
} // namespace maximum_pooling1d
} // namespace layers
} // namespace neural_networks
} // namespace algorithms
} // namespace daal

#endif
