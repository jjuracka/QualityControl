// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file   QcMFTClusterPostProcessing.h
/// \author Jakub Juracka
///

#ifndef QUALITYCONTROL_QCMFTCLUSTERPOSTPROCESSING_H
#define QUALITYCONTROL_QCMFTCLUSTERPOSTPROCESSING_H

#include "QualityControl/PostProcessingInterface.h"
#include <memory>
#include <vector>
#include <TH1F.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TColor.h>

namespace o2::quality_control_modules::mft
{

/// \brief Comparative visualisation of MFT cluster positions across all detector layers.
class QcMFTClusterPostProcessing final : public quality_control::postprocessing::PostProcessingInterface
{
 public:
  /// \brief Constructor
  QcMFTClusterPostProcessing();
  /// \brief Destructor
  ~QcMFTClusterPostProcessing() override;

  // Definition of the methods
  void configure(const boost::property_tree::ptree& config) override;
  void initialize(quality_control::postprocessing::Trigger, framework::ServiceRegistryRef) override;
  void update(quality_control::postprocessing::Trigger, framework::ServiceRegistryRef) override;
  void finalize(quality_control::postprocessing::Trigger, framework::ServiceRegistryRef) override;

 private:
  std::unique_ptr<TCanvas> mCanvas = nullptr;
  std::unique_ptr<THStack> mStack = nullptr;
};

} // namespace o2::quality_control_modules::mft

#endif //QUALITYCONTROL_QCMFTCLUSTERPOSTPROCESSING_H