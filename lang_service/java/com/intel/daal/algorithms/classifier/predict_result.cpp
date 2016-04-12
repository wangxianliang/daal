/* file: predict_result.cpp */
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

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>/* Header for class com_intel_daal_algorithms_classifier_prediction_PredictionResult */

#include "daal.h"
#include "classifier/prediction/JPredictionResult.h"
#include "classifier/prediction/JPredictionResultId.h"

#include "common_helpers.h"

USING_COMMON_NAMESPACES();
using namespace daal::algorithms::classifier;
using namespace daal::algorithms::classifier::prediction;

#define Prediction com_intel_daal_algorithms_classifier_prediction_PredictionResultId_Prediction

/*
 * Class:     com_intel_daal_algorithms_classifier_prediction_PredictionResult
 * Method:    cNewResult
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_classifier_prediction_PredictionResult_cNewResult
(JNIEnv *env, jobject thisObj)
{
    return jniArgument<classifier::prediction::Result>::newObj();
}

/*
 * Class:     com_intel_daal_algorithms_classifier_prediction_PredictionResult
 * Method:    cGetResult
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_classifier_prediction_PredictionResult_cGetResult
(JNIEnv *env, jobject thisObj, jlong algAddr )
{
    services::SharedPtr<SerializationIface> *ptr = new services::SharedPtr<SerializationIface>();

    //    if(cmode == jBatch)
    //    {
    SharedPtr<Batch> alg =
        staticPointerCast<Batch, AlgorithmIface>
            (*(SharedPtr<AlgorithmIface> *)algAddr);
    *ptr = alg->getResult();
    //    }
    return (jlong)ptr;
}
/*
 * Class:     com_intel_daal_algorithms_classifier_prediction_PredictionResult
 * Method:    cGetMinimum
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_classifier_prediction_PredictionResult_cGetResultTable
(JNIEnv *env, jobject thisObj, jlong resAddr, jint id)
{
    if (id == Prediction)
    {
        return jniArgument<classifier::prediction::Result>::
            get<classifier::prediction::ResultId, NumericTable>(resAddr, classifier::prediction::prediction);
    }

    return (jlong)0;
}

/*
 * Class:     com_intel_daal_algorithms_classifier_prediction_PredictionResult
 * Method:    cSetResultTable
 * Signature: (JIJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_daal_algorithms_classifier_prediction_PredictionResult_cSetResultTable
(JNIEnv *env, jobject thisObj, jlong resAddr, jint id, jlong ntAddr)
{
    jniArgument<classifier::prediction::Result>::
        set<classifier::prediction::ResultId, NumericTable>(resAddr, id, ntAddr);
}
