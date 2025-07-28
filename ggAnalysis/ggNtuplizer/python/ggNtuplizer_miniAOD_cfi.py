import FWCore.ParameterSet.Config as cms
from RecoJets.Configuration.RecoPFJets_cff import *
from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets, ak8GenJets
from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *
#from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.tools.jetTools import *
#from PhysicsTools.PatAlgos.patSequences_cff import *
#from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.slimming.metFilterPaths_cff import *
# add by me                                                                                                                                                                    
from PhysicsTools.PatAlgos.cleaningLayer1.jetCleaner_cfi import cleanPatJets 

ggNtuplizer = cms.EDAnalyzer("ggNtuplizer",
                             doGenParticles       = cms.bool(True),
                             runOnParticleGun     = cms.bool(False),
                             runOnSherpa          = cms.bool(False),
                             runL1ECALPrefire     = cms.bool(False),
                             dumpPFPhotons        = cms.bool(True), 
                             dumpJets             = cms.bool(False),
                             dumpAK8Jets          = cms.bool(False),
                             dumpTaus             = cms.bool(False),
                             dumpPDFSystWeight    = cms.bool(False),
                             dumpHFElectrons      = cms.bool(True),
                             development          = cms.bool(False),
                             addFilterInfoMINIAOD = cms.bool(False),
                             doNoHFMET            = cms.bool(False),

                             trgFilterDeltaPtCut  = cms.double(0.5),
                             trgFilterDeltaRCut   = cms.double(0.3),

                             triggerEvent         = cms.InputTag("slimmedPatTrigger", "", ""),
                             triggerResults       = cms.InputTag("TriggerResults", "", "HLT"),
                             patTriggerResults    = cms.InputTag("TriggerResults", "", "RECO"),
                             genParticleSrc       = cms.InputTag("prunedGenParticles"),
                             generatorLabel       = cms.InputTag("generator"),
                             LHEEventLabel        = cms.InputTag("externalLHEProducer"),
                             newParticles         = cms.vint32(1000006, 1000021, 1000022, 1000024, 1000025, 1000039, 3000001, 3000002, 35),
                             pileupCollection     = cms.InputTag("slimmedAddPileupInfo"),
                             VtxLabel             = cms.InputTag("offlineSlimmedPrimaryVertices"),
                             VtxBSLabel           = cms.InputTag("offlinePrimaryVerticesWithBS"),
                             rhoLabel             = cms.InputTag("fixedGridRhoFastjetAll"),
                             rhoCentralLabel      = cms.InputTag("fixedGridRhoFastjetCentralNeutral"),
                             pfMETLabel           = cms.InputTag("slimmedMETs"),
                             puppiMETlabel        = cms.InputTag("slimmedMETsPuppi"),
                             electronSrc          = cms.InputTag("slimmedElectrons"),
                             calibelectronSrc     = cms.InputTag("Electrons"),
                             photonSrc            = cms.InputTag("slimmedPhotons"),
                             calibphotonSrc       = cms.InputTag("Photons"),
                             Muons              = cms.InputTag("cleanedMu"),
                             gsfTrackSrc          = cms.InputTag("reducedEgamma", "reducedGsfTracks"),
                             reducedEcalRecHitsEB = cms.InputTag("reducedEgamma", "reducedEBRecHits"),
                             reducedEcalRecHitsEE = cms.InputTag("reducedEgamma", "reducedEERecHits"),
                             reducedEcalRecHitsES = cms.InputTag("reducedEgamma", "reducedESRecHits"),

                             recoPhotonSrc             = cms.InputTag("reducedEgamma", "reducedGedPhotonCores"),
                             TrackLabel                = cms.InputTag("generalTracks"),
                             gsfElectronLabel          = cms.InputTag("gsfElectrons"),
                             PFAllCandidates           = cms.InputTag("particleFlow"),
                             ak8JetSrc                 = cms.InputTag("slimmedJetsAK8"),
                             PFJetsAK4                 = cms.InputTag("slimmedJets"),
                             tauSrc                    = cms.InputTag("slimmedTaus"),
                             packedPFCands             = cms.InputTag("packedPFCandidates"),
                             elePFClusEcalIsoProducer  = cms.InputTag("electronEcalPFClusterIsolationProducer"),
                             elePFClusHcalIsoProducer  = cms.InputTag("electronHcalPFClusterIsolationProducer"),
                             ecalBadCalibReducedMINIAODFilter = cms.InputTag("ecalBadCalibReducedMINIAODFilter"),
                             phoWP80MapToken = cms.InputTag("egmPhotonIDs:mvaPhoID-RunIIIWinter22-v1-wp80"),
                             phoWP90MapToken = cms.InputTag("egmPhotonIDs:mvaPhoID-RunIIIWinter22-v1-wp90"),
                             minJetPt = cms.untracked.double(25.),
                             maxEta = cms.untracked.double(3.),
                             
                             PFJetAK4Collection = cms.InputTag("slimmedJetsPuppi"),



                             year                 = cms.int32(2022),
                             

                             store_electrons = cms.untracked.bool(True),
                             store_muons = cms.untracked.bool(True),
                             store_photons = cms.untracked.bool(True),
                             store_ak4jets = cms.untracked.bool(True),
                             store_CHS_met = cms.untracked.bool(True),
                             store_PUPPI_met = cms.untracked.bool(True),
                             store_electron_scalnsmear =  cms.untracked.bool(True),
                             store_photon_scalnsmear =  cms.untracked.bool(True),
                             store_electron_idSF = cms.untracked.bool(True),
                             store_photon_idSF = cms.untracked.bool(True),

                             
                             
)
