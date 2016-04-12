/* file: ForwardBatch.java */
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

package com.intel.daal.algorithms.neural_networks.layers.maximum_pooling3d;

import com.intel.daal.algorithms.Precision;
import com.intel.daal.algorithms.ComputeMode;
import com.intel.daal.algorithms.AnalysisBatch;
import com.intel.daal.services.DaalContext;
import com.intel.daal.algorithms.neural_networks.layers.pooling3d.SpatialDimensions;

/**
 * <a name="DAAL-CLASS-ALGORITHMS__NEURAL_NETWORKS__LAYERS__MAXIMUM_POOLING3D__FORWARDBATCH"></a>
 * \brief Class that computes the results of the forward three-dimensional maximum pooling layer in the batch processing mode
 *
 * \par References
 *      - <a href="DAAL-REF-MAXIMUMPOOLING3DFORWARD-ALGORITHM">Forward three-dimensional maximum pooling layer description and usage models</a>
 *      - @ref Method class
 *      - @ref LayerDataId class
 *      - @ref ForwardInput class
 *      - @ref ForwardResult class
 */
public class ForwardBatch extends com.intel.daal.algorithms.neural_networks.layers.ForwardLayer {
    public  ForwardInput input;     /*!< %Input data */
    public  Method       method;    /*!< Computation method for the layer */
    public  Parameter    parameter; /*!< Parameters of the layer */
    private Precision    prec;      /*!< Data type to use in intermediate computations for the layer */

    /** @private */
    static {
        System.loadLibrary("JavaAPI");
    }

    /**
     * Constructs the forward three-dimensional maximum pooling layer by copying input objects of another
     * forward three-dimensional maximum pooling layer
     * @param context    Context to manage the forward three-dimensional maximum pooling layer
     * @param other      A forward three-dimensional maximum pooling layer to be used as the source to
     *                   initialize the input objects of the forward three-dimensional maximum pooling layer
     */
    public ForwardBatch(DaalContext context, ForwardBatch other) {
        super(context);
        this.method = other.method;
        prec = other.prec;

        this.cObject = cClone(other.cObject, prec.getValue(), method.getValue());
        input = new ForwardInput(context, cGetInput(cObject, prec.getValue(), method.getValue()));
        parameter = new Parameter(context, cInitParameter(cObject, prec.getValue(), method.getValue()));
    }

    /**
     * Constructs the forward three-dimensional maximum pooling layer
     * @param context    Context to manage the layer
     * @param cls        Data type to use in intermediate computations for the layer, Double.class or Float.class
     * @param method     The layer computation method, @ref Method
     * @param nDim       Number of dimensions in input data
     */
    public ForwardBatch(DaalContext context, Class<? extends Number> cls, Method method, long nDim) {
        super(context);

        this.method = method;

        if (method != Method.defaultDense) {
            throw new IllegalArgumentException("method unsupported");
        }
        if (cls != Double.class && cls != Float.class) {
            throw new IllegalArgumentException("type unsupported");
        }

        if (cls == Double.class) {
            prec = Precision.doublePrecision;
        }
        else {
            prec = Precision.singlePrecision;
        }

        this.cObject = cInit(prec.getValue(), method.getValue(), nDim);
        input = new ForwardInput(context, cGetInput(cObject, prec.getValue(), method.getValue()));
        parameter = new Parameter(context, cInitParameter(cObject, prec.getValue(), method.getValue()));
    }

    ForwardBatch(DaalContext context, Class<? extends Number> cls, Method method, long cObject, long nDim) {
        super(context);

        this.method = method;

        if (method != Method.defaultDense) {
            throw new IllegalArgumentException("method unsupported");
        }
        if (cls != Double.class && cls != Float.class) {
            throw new IllegalArgumentException("type unsupported");
        }

        if (cls == Double.class) {
            prec = Precision.doublePrecision;
        }
        else {
            prec = Precision.singlePrecision;
        }

        this.cObject = cObject;
        input = new ForwardInput(context, cGetInput(cObject, prec.getValue(), method.getValue()));
        parameter = new Parameter(context, cInitParameter(cObject, prec.getValue(), method.getValue()));
        SpatialDimensions sd = new SpatialDimensions(nDim - 3, nDim - 2, nDim - 1);
        parameter.setSpatialDimensions(sd);
    }

    /**
     * Computes the result of the forward three-dimensional maximum pooling layer
     * @return  Forward three-dimensional maximum pooling layer result
     */
    @Override
    public ForwardResult compute() {
        super.compute();
        ForwardResult result = new ForwardResult(getContext(), cGetResult(cObject, prec.getValue(), method.getValue()));
        return result;
    }

    /**
     * Registers user-allocated memory to store the result of the forward three-dimensional maximum pooling layer
     * @param result    Structure to store the result of the forward three-dimensional maximum pooling layer
     */
    public void setResult(ForwardResult result) {
        cSetResult(cObject, prec.getValue(), method.getValue(), result.getCObject());
    }

    /**
     * Returns the structure that contains result of the forward layer
     * @return Structure that contains result of the forward layer
     */
    @Override
    public ForwardResult getLayerResult() {
        return new ForwardResult(getContext(), cGetResult(cObject, prec.getValue(), method.getValue()));
    }

    /**
     * Returns the structure that contains input object of the forward layer
     * @return Structure that contains input object of the forward layer
     */
    @Override
    public ForwardInput getLayerInput() {
        return input;
    }

    /**
     * Returns the structure that contains parameters of the forward layer
     * @return Structure that contains parameters of the forward layer
     */
    @Override
    public Parameter getLayerParameter() {
        return parameter;
    }

    /**
     * Returns the newly allocated forward three-dimensional maximum pooling layer
     * with a copy of input objects of this forward three-dimensional maximum pooling layer
     * @param context    Context to manage the layer
     *
     * @return The newly allocated forward three-dimensional maximum pooling layer
     */
    @Override
    public ForwardBatch clone(DaalContext context) {
        return new ForwardBatch(context, this);
    }

    private native long cInit(int prec, int method, long nDim);
    private native long cInitParameter(long cAlgorithm, int prec, int method);
    private native long cGetInput(long cAlgorithm, int prec, int method);
    private native long cGetResult(long cAlgorithm, int prec, int method);
    private native void cSetResult(long cAlgorithm, int prec, int method, long cObject);
    private native long cClone(long algAddr, int prec, int method);
}
