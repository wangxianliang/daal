/* file: fullyconnected_layer_backward_types.h */
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
//  Implementation of backward fully-connected layer.
//--
*/

#ifndef __NEURAL_NETWORKS__FULLYCONNECTED_LAYER_BACKWARD_TYPES_H__
#define __NEURAL_NETWORKS__FULLYCONNECTED_LAYER_BACKWARD_TYPES_H__

#include "algorithms/algorithm.h"
#include "data_management/data/tensor.h"
#include "data_management/data/homogen_tensor.h"
#include "services/daal_defines.h"
#include "algorithms/neural_networks/layers/layer_backward_types.h"

namespace daal
{
namespace algorithms
{
namespace neural_networks
{
namespace layers
{
namespace fullyconnected
{
/**
 * \brief Contains classes for backward fully-connected layer
 */
namespace backward
{
/**
 * \brief Contains version 1.0 of Intel(R) Data Analytics Acceleration Library (Intel(R) DAAL) interface.
 */
namespace interface1
{

/**
 * <a name="DAAL-CLASS-ALGORITHMS__NEURAL_NETWORKS__LAYERS__BACKWARD__INPUT"></a>
 * \brief %Input objects for the backward fully-connected layer
 */
class Input : public layers::backward::Input
{
public:
    /**
     * Default constructor
     */
    Input() {};

    virtual ~Input() {}

    /**
     * Sets an input object for the backward fully-connected layer
     */
    using layers::backward::Input::get;

    /**
     * Returns an input object for the backward fully-connected layer
    */
    using layers::backward::Input::set;

    /**
     * Returns an input object for backward fully-connected layer
     * \param[in] id    Identifier of the input object
     * \return          %Input object that corresponds to the given identifier
     */
    services::SharedPtr<data_management::Tensor> get(LayerDataId id) const
    {
        services::SharedPtr<layers::LayerData> layerData =
            services::staticPointerCast<layers::LayerData, data_management::SerializationIface>(Argument::get(layers::backward::inputFromForward));
        return services::staticPointerCast<data_management::Tensor, data_management::SerializationIface>((*layerData)[id]);
    }

    /**
     * Sets input for the backward fully-connected layer
     * \param[in] id    Identifier of the input  object
     * \param[in] value Input object to set
     */
    void set(LayerDataId id, const services::SharedPtr<data_management::Tensor> &value)
    {
        services::SharedPtr<layers::LayerData> layerData = get(layers::backward::inputFromForward);
        (*layerData)[id] = value;
    }

    /**
     * Checks an input object of the fully-connected layer
     * \param[in] par       %Parameter of layer
     * \param[in] method    Computation method of the layer
     */
    void check(const daal::algorithms::Parameter *par, int method) const DAAL_C11_OVERRIDE
    {
        layers::backward::Input::check(par, method);
        if( this->_errors->size() > 0 ) { return; }

        services::SharedPtr<services::Error> error;

        const Parameter *param = static_cast<const Parameter*>(par);

        services::SharedPtr<data_management::Tensor> xTensor = get(auxData);

        error = checkTensor(xTensor, "auxData");
        if (error) { this->_errors->add(error); return; }

        const services::Collection<size_t> &xDims = xTensor->getDimensions();
        const services::Collection<size_t> &gDims = get(layers::backward::inputGradient)->getDimensions();

        services::Collection<size_t> gradDims;
        gradDims.push_back(xDims[param->dim]);
        gradDims.push_back(param->nOutputs);

        services::Collection<size_t> wDims = xDims;
        wDims[param->dim] = param->nOutputs;

        error = checkTensor(get(layers::backward::inputGradient), "inputGradient", &gradDims);
        if (error) { this->_errors->add(error); return; }

        error = checkTensor(get(auxWeights), "auxWeights", &wDims);
        if (error) { this->_errors->add(error); return; }
    }
};

/**
 * <a name="DAAL-CLASS-ALGORITHMS__NEURAL_NETWORKS__LAYERS__FULLYCONNECTED__RESULT"></a>
 * \brief Results obtained with the compute() method of the backward fully-connected layer
 */
class Result : public layers::backward::Result
{
public:
    /**
     * Default constructor
     */
    Result() : layers::backward::Result() {}

    virtual ~Result() {}

    /**
     * Returns the result of the backward fully-connected layer
     */
    using layers::backward::Result::get;

    /**
     * Sets the result of the backward fully-connected layer
     */
    using layers::backward::Result::set;

    /**
     * Allocates memory to store the result of backward fully-connected layer
     * \param[in] input     Object containing the input data
     * \param[in] parameter %Parameter of backward fully-connected layer
     * \param[in] method    Computation method
     */
    template <typename algorithmFPType>
    void allocate(const daal::algorithms::Input *input, const daal::algorithms::Parameter *parameter, const int method)
    {
        using daal::data_management::Tensor;
        using daal::data_management::HomogenTensor;

        const Input *in = static_cast<const Input *>(input);
        const Parameter *param =  static_cast<const Parameter * >(parameter);

        services::Collection<size_t> bDims;
        bDims.push_back(param->nOutputs);

        services::SharedPtr<Tensor> valueTable = in->get(auxData);
        services::SharedPtr<Tensor> wTable     = in->get(auxWeights);

        if(valueTable == 0 || wTable == 0) { this->_errors->add(services::ErrorNullInputNumericTable); return; }

        Argument::set(layers::backward::gradient, services::SharedPtr<data_management::SerializationIface>(
                          new HomogenTensor<algorithmFPType>(valueTable->getDimensions(), Tensor::doAllocate)));
        if( get(layers::backward::weightDerivatives) == 0 )
        {
            Argument::set(layers::backward::weightDerivatives, services::SharedPtr<data_management::SerializationIface>(
                              new HomogenTensor<algorithmFPType>(wTable->getDimensions(), Tensor::doAllocate)));
        }
        if( get(layers::backward::biasDerivatives) == 0 )
        {
            Argument::set(layers::backward::biasDerivatives, services::SharedPtr<data_management::SerializationIface>(
                              new HomogenTensor<algorithmFPType>(bDims, Tensor::doAllocate)));
        }
    }

    /**
     * Checks the result of the fully-connected layer
     * \param[in] input   %Input object of the layer
     * \param[in] par     %Parameter of the layer
     * \param[in] method  Computation method of the layer
     */
    void check(const daal::algorithms::Input *input, const daal::algorithms::Parameter *par, int method) const DAAL_C11_OVERRIDE
    {
        layers::backward::Result::check(input, par, method);
        if( this->_errors->size() > 0 ) { return; }

        services::SharedPtr<services::Error> error;

        const Input *algInput = static_cast<const Input*>(input);
        const Parameter *param = static_cast<const Parameter*>(par);

        error = checkTensor(get(layers::backward::gradient), "gradient", &(algInput->get(auxData)->getDimensions()));
        if (error) { this->_errors->add(error); return; }

        error = checkTensor(get(layers::backward::weightDerivatives), "weightDerivatives", &(algInput->get(auxWeights)->getDimensions()));
        if (error) { this->_errors->add(error); return; }

        services::Collection<size_t> bDims;
        bDims.push_back(param->nOutputs);

        error = checkTensor(get(layers::backward::biasDerivatives),   "biasDerivatives",  &bDims);
        if (error) { this->_errors->add(error); return; }
    }
};

} // namespace interface1
using interface1::Input;
using interface1::Result;
} // namespace backward
} // namespace fullyconnected
} // namespace layers
} // namespace neural_networks
} // namespace algorithms
} // namespace daal
#endif
