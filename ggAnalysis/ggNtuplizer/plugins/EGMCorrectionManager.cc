#include "ggAnalysis/ggNtuplizer/interface/EGMCorrectionManager.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include "FWCore/Framework/interface/Event.h"
// Constructor - just store the year and period, then load the corrections
EGMCorrectionManager::EGMCorrectionManager(int year, const std::string& period, bool useETDependent)
    : year_(year), period_(period), useETDependent_(useETDependent) {
    initializeCorrections();
}

void EGMCorrectionManager::initializeCorrections() {
  
    std::string electronFile = getElectronJSONFile();
    std::string photonFile = getPhotonJSONFile();

    unzipFileIfNeeded(electronFile);
    unzipFileIfNeeded(photonFile);
    
    electronCorrectionSet_ = correction::CorrectionSet::from_file(electronFile);
    photonCorrectionSet_ = correction::CorrectionSet::from_file(photonFile);
    
    setupElectronEvaluators();
    setupPhotonEvaluators();
    
}

std::string EGMCorrectionManager::getElectronJSONFile() {
    std::string fileName = "electronSS";
    if (useETDependent_) {
        fileName += "_EtDependent";
    }
    fileName += ".json"; 
    if (year_ == 2022) {
        if (period_ == "B" || period_ == "C" || period_ == "D" || 
            period_ == "preEE" || period_.empty()) {
            return "/eos/cms/store/group/phys_egamma/ScaleFactors/Data2022/ForRe-recoBCD/SS/" + fileName;
        }
        else if (period_ == "E" || period_ == "F" || period_ == "G" || period_ == "postEE") {
            return "/eos/cms/store/group/phys_egamma/ScaleFactors/Data2022/ForRe-recoE+PromptFG/SS/" + fileName;
        }
    } 
    else if (year_ == 2023) {
        if (period_ == "C" || period_ == "preBPIX") {
            return "/eos/cms/store/group/phys_egamma/ScaleFactors/Data2023/ForPrompt23C/SS/" + fileName;
        }
        else if (period_ == "D" || period_ == "postBPIX") {
            return "/eos/cms/store/group/phys_egamma/ScaleFactors/Data2023/ForPrompt23D/SS/" + fileName;
        }
    }
    
    throw std::runtime_error("Year " + std::to_string(year_) + " period " + period_ + " not supported");
}

std::string EGMCorrectionManager::getPhotonJSONFile() {
    std::string fileName = "photonSS";
    if (useETDependent_) {
        fileName += "_EtDependent";
    }
    fileName += ".json";
    if (year_ == 2022) {
        if (period_ == "B" || period_ == "C" || period_ == "D" || 
            period_ == "preEE" || period_.empty()) {
            return "/eos/cms/store/group/phys_egamma/ScaleFactors/Data2022/ForRe-recoBCD/SS/" + fileName;
        }
        else if (period_ == "E" || period_ == "F" || period_ == "G" || period_ == "postEE") {
            return "/eos/cms/store/group/phys_egamma/ScaleFactors/Data2022/ForRe-recoE+PromptFG/SS/" + fileName;
        }
    } 
    else if (year_ == 2023) {
        if (period_ == "C" || period_ == "preBPIX") {
            return "/eos/cms/store/group/phys_egamma/ScaleFactors/Data2023/ForPrompt23C/SS/" + fileName;
        }

	else if (period_ == "D" || period_ == "postBPIX") {
            return "/eos/cms/store/group/phys_egamma/ScaleFactors/Data2023/ForPrompt23D/SS/" + fileName;
        }
    }
    
    throw std::runtime_error("Year " + std::to_string(year_) + " period " + period_ + " not supported");
}

void EGMCorrectionManager::unzipFileIfNeeded(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.good()) {
        return;
    }
    
    std::string gzFilePath = filePath + ".gz";
    std::string unzipCmd = "gunzip -k " + gzFilePath;
    int result = system(unzipCmd.c_str());
    
    if (result != 0) {
        throw std::runtime_error("Failed to unzip file: " + gzFilePath);
    }
}

void EGMCorrectionManager::setupElectronEvaluators() {
    std::string yearSuffix = getYearSuffix();
    
    std::string scaleName = "EGMScale_Compound_Ele" + yearSuffix;
    std::string smearName = "EGMSmearAndSyst_ElePTsplit" + yearSuffix;
   
    electronScaleEvaluator_ = electronCorrectionSet_->at(scaleName);
    electronSmearEvaluator_ = electronCorrectionSet_->at(smearName);
}

void EGMCorrectionManager::setupPhotonEvaluators() {
    std::string yearSuffix = getYearSuffix();
    
    std::string scaleName = "EGMScale_Compound_Pho" + yearSuffix;
    std::string smearName = "EGMSmearAndSyst_PhoETsplit" + yearSuffix;
    photonScaleEvaluator_ = photonCorrectionSet_->at(scaleName);
    photonSmearEvaluator_ = photonCorrectionSet_->at(smearName);
}

std::string EGMCorrectionManager::getYearSuffix() {
    if (year_ == 2022) {
        if (period_ == "B" || period_ == "C" || period_ == "D" || 
            period_ == "preEE" || period_.empty()) {
            return "_2022preEE";
        }
        else if (period_ == "E" || period_ == "F" || period_ == "G" || period_ == "postEE") {
            return "_2022postEE";
        }
    } 
    else if (year_ == 2023) {
        if (period_ == "C" || period_ == "preBPIX") {
            return "_2023preBPIX";
        }
        else if (period_ == "D" || period_ == "postBPIX") {
            return "_2023postBPIX";
        }
    }
    
    throw std::runtime_error("Year " + std::to_string(year_) + " period " + period_ + " not supported");
}

double EGMCorrectionManager::getElectronScale(int run, double scEta, double r9, double pt, int seedGain) {
    return electronScaleEvaluator_->evaluate({"scale", run, scEta, r9, std::abs(scEta), pt, seedGain});
}

double EGMCorrectionManager::getElectronSmear(double pt, double r9, double absScEta) {
    return electronSmearEvaluator_->evaluate({"smear", pt, r9, absScEta});
}

double EGMCorrectionManager::getElectronScaleUnc(double pt, double r9, double absScEta) {
    return electronSmearEvaluator_->evaluate({"escale", pt, r9, absScEta});
}

double EGMCorrectionManager::getElectronSmearUnc(double pt, double r9, double absScEta) {
    return electronSmearEvaluator_->evaluate({"esmear", pt, r9, absScEta});
}

double EGMCorrectionManager::getPhotonScale(int run, double scEta, double r9, double pt, int seedGain) {
  return photonScaleEvaluator_->evaluate({"scale", run, scEta, r9, std::abs(scEta), pt, seedGain});
}

double EGMCorrectionManager::getPhotonSmear(double pt, double r9, double absScEta) {
    return photonSmearEvaluator_->evaluate({"smear", pt, r9, absScEta});
}

double EGMCorrectionManager::getPhotonScaleUnc(double pt, double r9, double absScEta) {
    return photonSmearEvaluator_->evaluate({"escale", pt, r9, absScEta});
}

double EGMCorrectionManager::getPhotonSmearUnc(double pt, double r9, double absScEta) {
    return photonSmearEvaluator_->evaluate({"esmear", pt, r9, absScEta});
}

double EGMCorrectionManager::applyCorrectedElectronPt(double originalPt, int run, double scEta, double r9, int seedGain, bool isData, double randomNum) {
    if (isData) {
        double scale = getElectronScale(run, scEta, r9, originalPt, seedGain);
        return scale * originalPt;
    } else {
        double smear = getElectronSmear(originalPt, r9, std::abs(scEta));
        return originalPt * (1.0 + smear * randomNum);
    }
}

double EGMCorrectionManager::applyCorrectedPhotonPt(double originalPt, int run, double scEta, double r9, int seedGain, bool isData, double randomNum) {
    if (isData) {
      double scale = getPhotonScale(run, scEta, r9, originalPt, seedGain);
        return scale * originalPt;
    } else {
      double smear = getPhotonSmear(originalPt, r9, std::abs(scEta));
      return originalPt * (1.0 + smear * randomNum);
    }
}

//Getting the seedGain
int EGMCorrectionManager::GetSeedGain(const DetId& seedDetId, const edm::Event& e,
                                     const edm::EDGetTokenT<EcalRecHitCollection>& ebToken,
                                     const edm::EDGetTokenT<EcalRecHitCollection>& eeToken) {
    int gain = 12; // Default ECAL gain
    try {
        if (seedDetId.subdetId() == EcalBarrel) {
            edm::Handle<EcalRecHitCollection> ebRecHits;
            e.getByToken(ebToken, ebRecHits);
            
            if (ebRecHits.isValid()) {
                EcalRecHitCollection::const_iterator rechit = ebRecHits->find(seedDetId);
                if (rechit != ebRecHits->end()) {
                    if (rechit->checkFlag(EcalRecHit::kHasSwitchToGain1)) {
                        gain = 1;
                    } else if (rechit->checkFlag(EcalRecHit::kHasSwitchToGain6)) {
                        gain = 6;
                    } else {
                        gain = 12;
                    }
                }
            }
        }
        else if (seedDetId.subdetId() == EcalEndcap) {
            edm::Handle<EcalRecHitCollection> eeRecHits;
            e.getByToken(eeToken, eeRecHits);
            
            if (eeRecHits.isValid()) {
                EcalRecHitCollection::const_iterator rechit = eeRecHits->find(seedDetId);
                if (rechit != eeRecHits->end()) {
                    if (rechit->checkFlag(EcalRecHit::kHasSwitchToGain1)) {
                        gain = 1;
                    } else if (rechit->checkFlag(EcalRecHit::kHasSwitchToGain6)) {
                        gain = 6;
                    } else {
                        gain = 12;
                    }
                }
            }
        }
    }
    catch (const std::exception& ex) {
        gain = 12;
    }
    
    return gain;
}

