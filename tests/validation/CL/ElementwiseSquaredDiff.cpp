/*
 * Copyright (c) 2018-2019 ARM Limited.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "arm_compute/core/Types.h"
#include "arm_compute/runtime/CL/CLTensor.h"
#include "arm_compute/runtime/CL/CLTensorAllocator.h"
#include "arm_compute/runtime/CL/functions/CLElementwiseOperations.h"
#include "tests/CL/CLAccessor.h"
#include "tests/PaddingCalculator.h"
#include "tests/datasets/ConvertPolicyDataset.h"
#include "tests/datasets/ShapeDatasets.h"
#include "tests/framework/Asserts.h"
#include "tests/framework/Macros.h"
#include "tests/framework/datasets/Datasets.h"
#include "tests/validation/Validation.h"
#include "tests/validation/fixtures/ElementwiseOperationsFixture.h"

namespace arm_compute
{
namespace test
{
namespace validation
{
namespace
{
RelativeTolerance<float> tolerance_fp32(0.000001f);
RelativeTolerance<float> tolerance_fp16(0.001f);

constexpr unsigned int num_elems_processed_per_iteration = 16;
/** Input data sets **/
const auto ElementwiseSquaredDiffU8Dataset = combine(combine(framework::dataset::make("DataType", DataType::U8), framework::dataset::make("DataType", DataType::U8)),
                                                     framework::dataset::make("DataType",
                                                                              DataType::U8));
const auto ElementwiseSquaredDiffQASYMM8Dataset = combine(combine(framework::dataset::make("DataType", DataType::QASYMM8), framework::dataset::make("DataType", DataType::QASYMM8)),
                                                          framework::dataset::make("DataType",
                                                                                   DataType::QASYMM8));
const auto ElementwiseSquaredDiffS16Dataset = combine(combine(framework::dataset::make("DataType", { DataType::U8, DataType::S16 }), framework::dataset::make("DataType", DataType::S16)),
                                                      framework::dataset::make("DataType", DataType::S16));
const auto ElementwiseSquaredDiffFP16Dataset = combine(combine(framework::dataset::make("DataType", DataType::F16), framework::dataset::make("DataType", DataType::F16)),
                                                       framework::dataset::make("DataType", DataType::F16));
const auto ElementwiseSquaredDiffFP32Dataset = combine(combine(framework::dataset::make("DataType", DataType::F32), framework::dataset::make("DataType", DataType::F32)),
                                                       framework::dataset::make("DataType", DataType::F32));
} // namespace

TEST_SUITE(CL)
TEST_SUITE(ElementwiseSquaredDiff)

// *INDENT-OFF*
// clang-format off
DATA_TEST_CASE(Validate, framework::DatasetMode::ALL, zip(zip(zip(
               framework::dataset::make("Input1Info", { TensorInfo(TensorShape(32U, 13U, 2U), 1, DataType::U8),
                                                        TensorInfo(TensorShape(32U, 13U, 2U), 1, DataType::U8),
                                                        TensorInfo(TensorShape(27U, 13U, 2U), 1, DataType::U8),      // Window shrink
                                                        TensorInfo(TensorShape(32U, 13U, 2U), 1, DataType::U8),      // Invalid data type combination
                                                        TensorInfo(TensorShape(32U, 13U, 2U), 1, DataType::F32),     // Mismatching shapes
                                                      }),
               framework::dataset::make("Input2Info",{ TensorInfo(TensorShape(32U, 13U, 2U), 1, DataType::U8),
                                                       TensorInfo(TensorShape(32U, 13U, 2U), 1, DataType::U8),
                                                       TensorInfo(TensorShape(27U, 13U, 2U), 1, DataType::U8),
                                                       TensorInfo(TensorShape(32U, 13U, 2U), 1, DataType::S16),
                                                       TensorInfo(TensorShape(48U, 11U, 2U), 1, DataType::F32),
                                                     })),
               framework::dataset::make("OutputInfo",{ TensorInfo(TensorShape(32U, 13U, 2U), 1, DataType::S16),
                                                       TensorInfo(TensorShape(32U, 13U, 2U), 1, DataType::U8),
                                                       TensorInfo(TensorShape(27U, 13U, 2U), 1, DataType::U8),
                                                       TensorInfo(TensorShape(32U, 13U, 2U), 1, DataType::U8),
                                                       TensorInfo(TensorShape(48U, 11U, 2U), 1, DataType::F32),
                                                     })),
               framework::dataset::make("Expected", { true, true, false, false, false})),
               input1_info, input2_info, output_info, expected)
{
    ARM_COMPUTE_EXPECT(bool(CLElementwiseSquaredDiff::validate(&input1_info.clone()->set_is_resizable(false), &input2_info.clone()->set_is_resizable(false), &output_info.clone()->set_is_resizable(false))) == expected, framework::LogLevel::ERRORS);
}
// clang-format on
// *INDENT-ON*

template <typename T>
using CLElementwiseSquaredDiffFixture = ElementwiseSquaredDiffValidationFixture<CLTensor, CLAccessor, CLElementwiseSquaredDiff, T>;

TEST_SUITE(U8)
DATA_TEST_CASE(Configuration, framework::DatasetMode::ALL, datasets::SmallShapes(),
               shape)
{
    // Create tensors
    CLTensor ref_src1 = create_tensor<CLTensor>(shape, DataType::U8);
    CLTensor ref_src2 = create_tensor<CLTensor>(shape, DataType::U8);
    CLTensor dst      = create_tensor<CLTensor>(shape, DataType::U8);

    // Create and Configure function
    CLElementwiseSquaredDiff add;
    add.configure(&ref_src1, &ref_src2, &dst);

    // Validate valid region
    const ValidRegion valid_region = shape_to_valid_region(shape);
    validate(dst.info()->valid_region(), valid_region);

    // Validate padding
    const PaddingSize padding = PaddingCalculator(shape.x(), num_elems_processed_per_iteration).required_padding();
    validate(ref_src1.info()->padding(), padding);
    validate(ref_src2.info()->padding(), padding);
    validate(dst.info()->padding(), padding);
}

FIXTURE_DATA_TEST_CASE(RunSmall, CLElementwiseSquaredDiffFixture<uint8_t>, framework::DatasetMode::PRECOMMIT, combine(datasets::SmallShapes(), ElementwiseSquaredDiffU8Dataset))
{
    // Validate output
    validate(CLAccessor(_target), _reference);
}
TEST_SUITE_END()

template <typename T>
using CLElementwiseSquaredDiffQuantizedFixture = ElementwiseSquaredDiffValidationQuantizedFixture<CLTensor, CLAccessor, CLElementwiseSquaredDiff, T>;

TEST_SUITE(Quantized)
TEST_SUITE(QASYMM8)
DATA_TEST_CASE(Configuration, framework::DatasetMode::ALL, datasets::SmallShapes(),
               shape)
{
    // Create tensors
    CLTensor ref_src1 = create_tensor<CLTensor>(shape, DataType::QASYMM8);
    CLTensor ref_src2 = create_tensor<CLTensor>(shape, DataType::QASYMM8);
    CLTensor dst      = create_tensor<CLTensor>(shape, DataType::QASYMM8);

    // Create and Configure function
    CLElementwiseSquaredDiff add;
    add.configure(&ref_src1, &ref_src2, &dst);

    // Validate valid region
    const ValidRegion valid_region = shape_to_valid_region(shape);
    validate(dst.info()->valid_region(), valid_region);

    // Validate padding
    const PaddingSize padding = PaddingCalculator(shape.x(), num_elems_processed_per_iteration).required_padding();
    validate(ref_src1.info()->padding(), padding);
    validate(ref_src2.info()->padding(), padding);
    validate(dst.info()->padding(), padding);
}

FIXTURE_DATA_TEST_CASE(RunSmall, CLElementwiseSquaredDiffQuantizedFixture<uint8_t>, framework::DatasetMode::PRECOMMIT, combine(combine(combine(combine(datasets::SmallShapes(),
                       ElementwiseSquaredDiffQASYMM8Dataset),
                       framework::dataset::make("QuantizationInfo", { QuantizationInfo(5.f / 255.f, 20) })),
                       framework::dataset::make("QuantizationInfo", { QuantizationInfo(2.f / 255.f, 10) })),
                       framework::dataset::make("QuantizationInfo", { QuantizationInfo(1.f / 255.f, 5) }))

                      )
{
    // Validate output
    validate(CLAccessor(_target), _reference, tolerance_fp32, 0.01);
}
TEST_SUITE_END()
TEST_SUITE_END()

TEST_SUITE(S16)
DATA_TEST_CASE(Configuration, framework::DatasetMode::ALL, combine(datasets::SmallShapes(), framework::dataset::make("DataType", { DataType::U8, DataType::S16 })),
               shape, data_type)
{
    // Create tensors
    CLTensor ref_src1 = create_tensor<CLTensor>(shape, data_type);
    CLTensor ref_src2 = create_tensor<CLTensor>(shape, DataType::S16);
    CLTensor dst      = create_tensor<CLTensor>(shape, DataType::S16);

    // Create and Configure function
    CLElementwiseSquaredDiff add;
    add.configure(&ref_src1, &ref_src2, &dst);

    // Validate valid region
    const ValidRegion valid_region = shape_to_valid_region(shape);
    validate(dst.info()->valid_region(), valid_region);

    // Validate padding
    const PaddingSize padding = PaddingCalculator(shape.x(), num_elems_processed_per_iteration).required_padding();
    validate(ref_src1.info()->padding(), padding);
    validate(ref_src2.info()->padding(), padding);
    validate(dst.info()->padding(), padding);
}

FIXTURE_DATA_TEST_CASE(RunSmall, CLElementwiseSquaredDiffFixture<int16_t>, framework::DatasetMode::ALL, combine(datasets::SmallShapes(), ElementwiseSquaredDiffS16Dataset))
{
    // Validate output
    validate(CLAccessor(_target), _reference);
}
TEST_SUITE_END()

TEST_SUITE(Float)
TEST_SUITE(FP16)
FIXTURE_DATA_TEST_CASE(RunSmall, CLElementwiseSquaredDiffFixture<half>, framework::DatasetMode::ALL, combine(datasets::SmallShapes(), ElementwiseSquaredDiffFP16Dataset))
{
    // Validate output
    validate(CLAccessor(_target), _reference, tolerance_fp16, 0.01);
}
TEST_SUITE_END()

TEST_SUITE(FP32)
DATA_TEST_CASE(Configuration, framework::DatasetMode::ALL, datasets::SmallShapes(),
               shape)
{
    // Create tensors
    CLTensor ref_src1 = create_tensor<CLTensor>(shape, DataType::F32);
    CLTensor ref_src2 = create_tensor<CLTensor>(shape, DataType::F32);
    CLTensor dst      = create_tensor<CLTensor>(shape, DataType::F32);

    // Create and Configure function
    CLElementwiseSquaredDiff add;
    add.configure(&ref_src1, &ref_src2, &dst);

    // Validate valid region
    const ValidRegion valid_region = shape_to_valid_region(shape);
    validate(dst.info()->valid_region(), valid_region);

    // Validate padding
    const PaddingSize padding = PaddingCalculator(shape.x(), num_elems_processed_per_iteration).required_padding();
    validate(ref_src1.info()->padding(), padding);
    validate(ref_src2.info()->padding(), padding);
    validate(dst.info()->padding(), padding);
}

FIXTURE_DATA_TEST_CASE(RunSmall, CLElementwiseSquaredDiffFixture<float>, framework::DatasetMode::ALL, combine(datasets::SmallShapes(), ElementwiseSquaredDiffFP32Dataset))
{
    // Validate output
    validate(CLAccessor(_target), _reference, tolerance_fp32);
}
template <typename T>
using CLElementwiseSquaredDiffBroadcastFixture = ElementwiseSquaredDiffBroadcastValidationFixture<CLTensor, CLAccessor, CLElementwiseSquaredDiff, T>;

FIXTURE_DATA_TEST_CASE(RunSmallBroadcast, CLElementwiseSquaredDiffBroadcastFixture<float>, framework::DatasetMode::ALL, combine(datasets::SmallShapesBroadcast(),
                       ElementwiseSquaredDiffFP32Dataset))
{
    // Validate output
    validate(CLAccessor(_target), _reference, tolerance_fp32);
}
TEST_SUITE_END()
TEST_SUITE_END()

TEST_SUITE_END()
TEST_SUITE_END()
} // namespace validation
} // namespace test
} // namespace arm_compute
