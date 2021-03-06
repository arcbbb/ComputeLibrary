/*
 * Copyright (c) 2018 ARM Limited.
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
#include "arm_compute/runtime/CL/functions/CLSobel3x3.h"
#include "arm_compute/runtime/CL/functions/CLSobel5x5.h"
#include "arm_compute/runtime/CL/functions/CLSobel7x7.h"
#include "tests/CL/CLAccessor.h"
#include "tests/benchmark/fixtures/SobelFixture.h"
#include "tests/datasets/BorderModeDataset.h"
#include "tests/datasets/GradientDimensionDataset.h"
#include "tests/datasets/ShapeDatasets.h"
#include "tests/framework/Macros.h"
#include "tests/framework/datasets/Datasets.h"
#include "utils/TypePrinter.h"

namespace arm_compute
{
namespace test
{
namespace benchmark
{
using CLSobel3x3Fixture = SobelFixture<CLTensor, CLSobel3x3, CLAccessor>;

TEST_SUITE(CL)
TEST_SUITE(Sobel)

// *INDENT-OFF*
// clang-format off
TEST_SUITE(W3x3)
REGISTER_FIXTURE_DATA_TEST_CASE(RunSmall, CLSobel3x3Fixture, framework::DatasetMode::PRECOMMIT,
                                combine(combine(combine(
                                datasets::Small2DShapes(),
                                datasets::BorderModes()),
                                datasets::GradientDimensions()),
                                framework::dataset::make("Format", Format::U8)));

REGISTER_FIXTURE_DATA_TEST_CASE(RunLarge, CLSobel3x3Fixture, framework::DatasetMode::NIGHTLY,
                                combine(combine(combine(
                                datasets::Large2DShapes(),
                                datasets::BorderModes()),
                                datasets::GradientDimensions()),
                                framework::dataset::make("Format", Format::U8)));
TEST_SUITE_END() // W3x3

using CLSobel5x5Fixture = SobelFixture<CLTensor, CLSobel5x5, CLAccessor>;

TEST_SUITE(W5x5)
REGISTER_FIXTURE_DATA_TEST_CASE(RunSmall, CLSobel5x5Fixture, framework::DatasetMode::PRECOMMIT,
                                combine(combine(combine(
                                datasets::Small2DShapes(),
                                datasets::BorderModes()),
                                datasets::GradientDimensions()),
                                framework::dataset::make("Format", Format::U8)));

REGISTER_FIXTURE_DATA_TEST_CASE(RunLarge, CLSobel5x5Fixture, framework::DatasetMode::NIGHTLY,
                                combine(combine(combine(
                                datasets::Large2DShapes(),
                                datasets::BorderModes()),
                                datasets::GradientDimensions()),
                                framework::dataset::make("Format", Format::U8)));
TEST_SUITE_END() // W5x5

using CLSobel7x7Fixture = SobelFixture<CLTensor, CLSobel7x7, CLAccessor>;

TEST_SUITE(W7x7)
REGISTER_FIXTURE_DATA_TEST_CASE(RunSmall, CLSobel7x7Fixture, framework::DatasetMode::PRECOMMIT,
                                combine(combine(combine(
                                datasets::Small2DShapes(),
                                datasets::BorderModes()),
                                datasets::GradientDimensions()),
                                framework::dataset::make("Format", Format::U8)));

REGISTER_FIXTURE_DATA_TEST_CASE(RunLarge, CLSobel7x7Fixture, framework::DatasetMode::NIGHTLY,
                                combine(combine(combine(
                                datasets::Large2DShapes(),
                                datasets::BorderModes()),
                                datasets::GradientDimensions()),
                                framework::dataset::make("Format", Format::U8)));
TEST_SUITE_END() // W7x7
// clang-format on
// *INDENT-ON*

TEST_SUITE_END() // Sobel
TEST_SUITE_END() // CL
} // namespace benchmark
} // namespace test
} // namespace arm_compute
