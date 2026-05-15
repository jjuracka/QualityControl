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
/// \file   QcMFTClusterPostProcessing.cxx
/// \author Jakub Juracka
///

#include "MFT/QcMFTClusterPostProcessing.h"
#include "QualityControl/QcInfoLogger.h"
#include "QualityControl/DatabaseInterface.h"
// are these needed?
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string/case_conv.hpp>

using namespace o2::quality_control::postprocessing;
using namespace o2::quality_control::core;

namespace o2::quality_control_modules::mft
{

QcMFTClusterPostProcessing::~QcMFTClusterPostProcessing()
{
}

void QcMFTClusterPostProcessing::configure(const boost::property_tree::ptree& config)
{
  // ILOG(Debug, Devel) << "configuring QcMFTClusterPostProcessing" << ENDM;
}

void QcMFTClusterPostProcessing::initialize(Trigger, framework::ServiceRegistryRef)
{
  ILOG(Debug, Devel) << "initializing QcMFTClusterPostProcessing" << ENDM;
  // define objects
  mCanvas = std::make_unique<TCanvas>("mClusterRinAllLayers", "CLuster radial position in all MFT layers");
  getObjectsManager()->startPublishing(mCanvas.get());
  mStack = std::make_unique<THStack>("mStack", "Cluster Radial Position in All MFT Layers; r (cm); # entries");
  mLegend = std::make_unique<TLegend>(0.83, 0.50, 0.90, 0.90);
  mLegend->SetBorderSize(0);
  mLegend->SetFillStyle(0);
}

void QcMFTClusterPostProcessing::update(Trigger t, framework::ServiceRegistryRef)
{
  auto& qcdb = services.get<quality_control::repository::DatabaseInterface>();
  // path: qc/MFT/MO/Clusters/ClusterRinLayer
  for (auto nMFTLayer = 0; nMFTLayer < 10; nMFTLayer++) {
    auto moHist = qcdb.retrieveMO("MFT/MO/Clusters/ClusterRinLayer", Form("mClusterRinLayer%d", nMFTLayer), t.timestamp, t.activity);
    if (moHist == nullptr) {
      ILOG(Warning, Support) << "Could not retrieve histogram for layer " << nMFTLayer << " at timestamp " << t.timestamp << ENDM;
      continue;
    }
    auto mHist = static_cast<TH1*>(moHist->getObject()->Clone());
    mHist->SetDirectory(nullptr);
    mHist->SetLineColor(TColor::GetColor(mColors[nMFTLayer]));
    mStack->Add(mHist);
    mLegend->AddEntry(mHist, Form("D%dF%d", static_cast<int>(std::floor(nMFTLayer / 2.)), nMFTLayer % 2 == 0 ? 0 : 1), "l");
  }
  mCanvas->cd();
  mStack->Draw("nostack hist");
  mLegend->Draw();
  mCanvas->Update();
}

void QcMFTClusterPostProcessing::finalize(Trigger, framework::ServiceRegistryRef)
{
  ILOG(Debug, Devel) << "finalizing QcMFTClusterPostProcessing" << ENDM;
  // reset objects
  mCanvas->Clear();
  mStack->Clear();
  mLegend->Clear();
}

} // namespace o2::quality_control_modules::mft