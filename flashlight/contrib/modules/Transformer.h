/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "flashlight/nn/modules/Container.h"
#include "flashlight/nn/modules/LayerNorm.h"
#include "flashlight/nn/modules/Linear.h"
#include "flashlight/nn/modules/Module.h"

namespace fl {

/**
 * A module which implements a Transformer.
 *
 * For details, see [Vaswani et al
 * (2017)](https://arxiv.org/abs/1706.03762).
 *
 * This module also supports layer drop regularization, as introduced in
 * [Fan et al (2019)](https://arxiv.org/abs/1909.11556).
 *
 * Input dimension at forward is assumed to be CxTxBx1, where C is the
 * number of features, T the sequence length and B the batch size.
 *
 */
class Transformer : public Container {
 public:
  Transformer(
      int32_t modelDim,
      int32_t headDim,
      int32_t mlpDim,
      int32_t nHeads,
      int32_t bptt,
      float pDropout,
      float pLayerdrop,
      bool useMask = false,
      bool preLN = false);

  std::vector<Variable> forward(const std::vector<Variable>& input) override;
  std::string prettyString() const override;

 private:
  int32_t nHeads_;
  int32_t bptt_;
  double pDropout_;
  double pLayerdrop_;
  bool useMask_;
  bool preLN_;
  std::shared_ptr<Linear> w1_, w2_, wq_, wk_, wv_, wf_;
  std::shared_ptr<LayerNorm> norm1_, norm2_;

  Variable mlp(const Variable& input);
  Variable getMask(int32_t n, bool cache = false);
  Variable selfAttention(const std::vector<Variable>& input);

  FL_SAVE_LOAD_WITH_BASE(
      Container,
      w1_,
      w2_,
      wq_,
      wk_,
      wv_,
      wf_,
      norm1_,
      norm2_,
      nHeads_,
      pDropout_,
      pLayerdrop_,
      bptt_,
      useMask_,
      preLN_)

  Transformer();
};

} // namespace fl

CEREAL_REGISTER_TYPE(fl::Transformer);
