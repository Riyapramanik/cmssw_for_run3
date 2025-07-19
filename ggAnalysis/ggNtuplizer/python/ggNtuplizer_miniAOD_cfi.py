import FWCore.ParameterSet.Config as cms
from RecoJets.Configuration.RecoPFJets_cff import *
from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets, ak8GenJets
#from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
#from RecoJets.JetProducers.PFJetParameters_cfi import *
#from RecoJets.JetProducers.GenJetParameters_cfi import *
#from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.patSequences_cff import *
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.slimming.metFilterPaths_cff import *


# add by me                                                                                                                                                                    
from PhysicsTools.PatAlgos.cleaningLayer1.jetCleaner_cfi import cleanPatJets                                                                                                  
process.cleanJets = cms.EDProducer("PATJetCleaner",
                                   src = cms.InputTag("selectedUpdatedPatJetsWithPNetInfo"),
                                   preselection = cms.string(''),
                                   checkOverlaps = cms.PSet(
                                       muons = cms.PSet(
                                           src          = cms.InputTag("slimmedMuons"),
                                           algorithm    = cms.string("byDeltaR"),
                                           preselection = cms.string(""),
                                           deltaR       = cms.double(0.4),
                                           checkRecoComponents = cms.bool(False),
                                           pairCut             = cms.string(""),
                                           requireNoOverlaps   = cms.bool(True)
                                       ),
                                       electrons = cms.PSet(
                                           src          = cms.InputTag("slimmedElectrons"),
                                           algorithm    = cms.string("byDeltaR"),
                                           preselection = cms.string(""),
                                           deltaR       = cms.double(0.4),
                                           checkRecoComponents = cms.bool(False),
                                           pairCut             = cms.string(""),
                                           requireNoOverlaps   = cms.bool(True)
                                       )
                                   ),
                                   finalCut = cms.string('')
                                   )



ggNtuplizer = cms.EDAnalyzer("ggNtuplizer",
                             #hggPhotonIDConfiguration = hggPhotonIDCuts,
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

                             year                 = cms.int32(2018), 

                             trgFilterDeltaPtCut  = cms.double(0.5),
                             trgFilterDeltaRCut   = cms.double(0.3),

                             triggerEvent         = cms.InputTag("slimmedPatTrigger", "", ""),
                             triggerResults       = cms.InputTag("TriggerResults", "", "HLT"),
                             #patTriggerResults    = cms.InputTag("TriggerResults", "", "PAT"),
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
                             electronSrc          = cms.InputTag("slimmedElectrons"),
                             #calibelectronSrc     = cms.InputTag("calibratedPatElectrons"),
                             calibelectronSrc     = cms.InputTag("slimmedElectrons"),
                             photonSrc            = cms.InputTag("slimmedPhotons"),
                             #calibphotonSrc       = cms.InputTag("calibratedPatPhotons"),
                             calibphotonSrc       = cms.InputTag("slimmedPhotons"),
                             #muonSrc              = cms.InputTag("slimmedMuons"),
                             muonSrc              = cms.InputTag("cleanedMu"),
                             gsfTrackSrc          = cms.InputTag("reducedEgamma", "reducedGsfTracks"),
                             ebReducedRecHitCollection = cms.InputTag("reducedEgamma", "reducedEBRecHits"),
                             eeReducedRecHitCollection = cms.InputTag("reducedEgamma", "reducedEERecHits"),
                             esReducedRecHitCollection = cms.InputTag("reducedEgamma", "reducedESRecHits"),
                             recoPhotonSrc             = cms.InputTag("reducedEgamma", "reducedGedPhotonCores"),
                             TrackLabel                = cms.InputTag("generalTracks"),
                             gsfElectronLabel          = cms.InputTag("gsfElectrons"),
                             PFAllCandidates           = cms.InputTag("particleFlow"),
                             #ak4JetSrc                 = cms.InputTag("updatedJets"),
                             #ak4JetSrc                 = cms.InputTag("slimmedJets"),
                             #ak4JetSrc                 = cms.InputTag("selectedUpdatedPatJetsUpdatedJEC"),
                             ak8JetSrc                 = cms.InputTag("slimmedJetsAK8"),
                             #ak8JetSrc                 = cms.InputTag("selectedUpdatedPatJetsUpdatedJECAK8"),
                             #boostedDoubleSVLabel      = cms.InputTag("pfBoostedDoubleSecondaryVertexAK8BJetTags"),
                             tauSrc                    = cms.InputTag("slimmedTaus"),
                             #pfLooseId                 = pfJetIDSelector.clone(),

                             packedPFCands             = cms.InputTag("packedPFCandidates"),
                             elePFClusEcalIsoProducer  = cms.InputTag("electronEcalPFClusterIsolationProducer"),
                             elePFClusHcalIsoProducer  = cms.InputTag("electronHcalPFClusterIsolationProducer"),
                             ecalBadCalibReducedMINIAODFilter = cms.InputTag("ecalBadCalibReducedMINIAODFilter"),
                             phoWP80MapToken = cms.InputTag("egmPhotonIDs:mvaPhoID-RunIIIWinter22-v1-wp80"),
                             phoWP90MapToken = cms.InputTag("egmPhotonIDs:mvaPhoID-RunIIIWinter22-v1-wp90"),
                             minJetPt = cms.untracked.double(25.),
                             maxEta = cms.untracked.double(3.),


                             PFJetAK4Collection = cms.InputTag("slimmedJetsPuppi")
                             
)
