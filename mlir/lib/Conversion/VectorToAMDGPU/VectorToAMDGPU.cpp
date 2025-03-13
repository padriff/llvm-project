//===- VectorToAMDGPU.cpp - Vector to AMDGPU dialect conversion ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/Conversion/VectorToAMDGPU/VectorToAMDGPU.h"

#include "mlir/Dialect/AMDGPU/IR/AMDGPUDialect.h"
#include "mlir/Dialect/Vector/IR/VectorOps.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/IR/TypeUtilities.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Support/LogicalResult.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

namespace mlir {
#define GEN_PASS_DEF_CONVERTVECTORTOAMDGPUPASS
#include "mlir/Conversion/Passes.h.inc"
} // namespace mlir

using namespace mlir;

namespace {
struct ConvertVectorToAMDGPUPass final
    : impl::ConvertVectorToAMDGPUPassBase<ConvertVectorToAMDGPUPass> {
  using impl::ConvertVectorToAMDGPUPassBase<
      ConvertVectorToAMDGPUPass>::ConvertVectorToAMDGPUPassBase;

  void runOnOperation() override;
};
} // namespace

void mlir::arith::populateVectorToAMDGPUConversionPatterns(
    RewritePatternSet &patterns) {
  //patterns.add<ExtfOnFloat8RewritePattern, TruncfToFloat8RewritePattern>(
  //    patterns.getContext());
}

void ConvertVectorToAMDGPUPass::runOnOperation() {
  Operation *op = getOperation();
  RewritePatternSet patterns(op->getContext());
  arith::populateVectorToAMDGPUConversionPatterns(patterns);
  if (failed(applyPatternsGreedily(op, std::move(patterns))))
    return signalPassFailure();
}
