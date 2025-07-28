#include "ggAnalysis/ggNtuplizer/interface/ggNtuplizer.h"

using namespace std;

void setbit(UShort_t& x, UShort_t bit) {
  UShort_t a = 1;
  x |= (a << bit);
}

ggNtuplizer::ggNtuplizer(const edm::ParameterSet& ps) :
  //hltPrescaleProvider_(ps, consumesCollector(), *this)
  ecalClusterToolsESGetTokens_{consumesCollector()} ,
  caloTop(esConsumes()),
  randomGenerator_(12345),normalDistribution_(0.0, 1.0)
{

  development_               = ps.getParameter<bool>("development");
  addFilterInfoMINIAOD_      = ps.getParameter<bool>("addFilterInfoMINIAOD");
  doNoHFMET_                 = ps.getParameter<bool>("doNoHFMET");

  doGenParticles_            = ps.getParameter<bool>("doGenParticles");
  runOnParticleGun_          = ps.getParameter<bool>("runOnParticleGun");
  runOnSherpa_               = ps.getParameter<bool>("runOnSherpa");
  runL1ECALPrefire_          = ps.getParameter<bool>("runL1ECALPrefire");
  dumpPFPhotons_             = ps.getParameter<bool>("dumpPFPhotons");
  dumpJets_                  = ps.getParameter<bool>("dumpJets");
  dumpAK8Jets_               = ps.getParameter<bool>("dumpAK8Jets");
  dumpSoftDrop_              = ps.getParameter<bool>("dumpSoftDrop");
  dumpTaus_                  = ps.getParameter<bool>("dumpTaus");
  dumpPDFSystWeight_         = ps.getParameter<bool>("dumpPDFSystWeight");
  dumpHFElectrons_           = ps.getParameter<bool>("dumpHFElectrons");
  year_                      = ps.getParameter<int>("year");

  trgFilterDeltaPtCut_       = ps.getParameter<double>("trgFilterDeltaPtCut");
  trgFilterDeltaRCut_        = ps.getParameter<double>("trgFilterDeltaRCut");

  vtxLabel_                  = consumes<reco::VertexCollection>        (ps.getParameter<InputTag>("VtxLabel"));
  vtxBSLabel_                = consumes<reco::VertexCollection>        (ps.getParameter<InputTag>("VtxBSLabel"));
  rhoLabel_                  = consumes<double>                        (ps.getParameter<InputTag>("rhoLabel"));
  rhoCentralLabel_           = consumes<double>                        (ps.getParameter<InputTag>("rhoCentralLabel"));
  trgEventLabel_             = consumes<trigger::TriggerEvent>         (ps.getParameter<InputTag>("triggerEvent"));
  triggerObjectsLabel_       = consumes<pat::TriggerObjectStandAloneCollection>(ps.getParameter<edm::InputTag>("triggerEvent"));
  trgResultsLabel_           = consumes<edm::TriggerResults>           (ps.getParameter<InputTag>("triggerResults"));
  patTrgResultsLabel_        = consumes<edm::TriggerResults>           (ps.getParameter<InputTag>("patTriggerResults"));
  trgResultsProcess_         =                                          ps.getParameter<InputTag>("triggerResults").process();
  generatorLabel_            = consumes<GenEventInfoProduct>           (ps.getParameter<InputTag>("generatorLabel"));
  lheEventLabel_             = consumes<LHEEventProduct>               (ps.getParameter<InputTag>("LHEEventLabel"));
  puCollection_              = consumes<vector<PileupSummaryInfo> >    (ps.getParameter<InputTag>("pileupCollection"));
  genParticlesCollection_    = consumes<vector<reco::GenParticle> >    (ps.getParameter<InputTag>("genParticleSrc"));
  pfMETlabel_                = consumes<View<pat::MET> >               (ps.getParameter<InputTag>("pfMETLabel"));
  puppiMETlabel_             = consumes<View<pat::MET> >               (ps.getParameter<InputTag>("puppiMETLabel"));
  electronCollection_        = consumes<View<pat::Electron> >          (ps.getParameter<InputTag>("electronSrc"));
  gsfTracks_                 = consumes<View<reco::GsfTrack>>          (ps.getParameter<InputTag>("gsfTrackSrc"));

  ecalBadCalibFilterUpdate_  = consumes<bool>                          (ps.getParameter<InputTag>("ecalBadCalibReducedMINIAODFilter"));

  photonCollection_          = consumes<View<pat::Photon> >            (ps.getParameter<InputTag>("photonSrc"));
  muonCollection_            = consumes<View<pat::Muon> >              (ps.getParameter<InputTag>("muonSrc"));
  ebReducedRecHitCollection_ = consumes<EcalRecHitCollection>          (ps.getParameter<InputTag>("reducedEcalRecHitsEB"));
  eeReducedRecHitCollection_ = consumes<EcalRecHitCollection>          (ps.getParameter<InputTag>("reducedEcalRecHitsEE"));
  esReducedRecHitCollection_ = consumes<EcalRecHitCollection>          (ps.getParameter<InputTag>("reducedEcalRecHitsES")); 
  recophotonCollection_      = consumes<reco::PhotonCollection>        (ps.getParameter<InputTag>("recoPhotonSrc"));
  tracklabel_                = consumes<reco::TrackCollection>         (ps.getParameter<InputTag>("TrackLabel"));
  gsfElectronlabel_          = consumes<reco::GsfElectronCollection>   (ps.getParameter<InputTag>("gsfElectronLabel"));
  tauCollection_             = consumes<vector<pat::Tau> >             (ps.getParameter<InputTag>("tauSrc"));
  pfAllParticles_            = consumes<reco::PFCandidateCollection>   (ps.getParameter<InputTag>("PFAllCandidates"));
  pckPFCandidateCollection_  = consumes<pat::PackedCandidateCollection>(ps.getParameter<InputTag>("packedPFCands"));
  pckPFCdsLabel_             = consumes<vector<pat::PackedCandidate>>  (ps.getParameter<InputTag>("packedPFCands"));
  recoCdsLabel_              = consumes<View<reco::Candidate>>         (ps.getParameter<InputTag>("packedPFCands"));
  
  newparticles_              =                                          ps.getParameter< vector<int > >("newParticles");

  prefweight_token_          = consumes<double>(edm::InputTag("prefiringweight:nonPrefiringProb"));
  prefweightup_token_        = consumes<double>(edm::InputTag("prefiringweight:nonPrefiringProbUp"));
  prefweightdown_token_      = consumes<double>(edm::InputTag("prefiringweight:nonPrefiringProbDown"));

  // Added by me
  tok_pfjetAK4s_ = consumes<edm::View<pat::Jet>>(ps.getParameter<edm::InputTag>("PFJetsAK4"));
  tok_genjetAK4s_= consumes<reco::GenJetCollection>( ps.getParameter<edm::InputTag>("GENJetAK4"));
  tok_Rho_ = consumes<double>(ps.getParameter<InputTag>("PFRho"));
  min_pt_AK4jet = ps.getUntrackedParameter<double>("minJetPt", 25.);
  max_eta = ps.getUntrackedParameter<double>("maxEta",3.);
  year		=  ps.getUntrackedParameter<string>("YEAR","2018");
  isData    = ps.getUntrackedParameter<bool>("Data",false);
  isMC      = ps.getUntrackedParameter<bool>("MonteCarlo", false);
  isRun3 	= ps.getUntrackedParameter<bool>("isRun3", false);
  isUltraLegacy = ps.getUntrackedParameter<bool>("UltraLegacy", false);
  tok_muons_ = consumes<edm::View<pat::Muon>> ( ps.getParameter<edm::InputTag>("Muons"));
  mJetVetoMap = ps.getParameter<std::string>("JetVetoMap");
  
  store_electron_scalnsmear = ps.getUntrackedParameter<bool>("store_electron_scalnsmear", false);
  store_photon_scalnsmear = ps.getUntrackedParameter<bool>("store_photon_scalnsmear", false);
  store_electrons = ps.getUntrackedParameter<bool>("store_electrons", false);
  store_muons = ps.getUntrackedParameter<bool>("store_muons", false);
  store_photons = ps.getUntrackedParameter<bool>("store_photons", false);
  store_ak4jets = ps.getUntrackedParameter<bool>("store_ak4jets", false);
  store_CHS_met   = ps.getUntrackedParameter<bool>("store_CHS_met", false);
  store_PUPPI_met = ps.getUntrackedParameter<bool>("store_PUPPI_met", false);
  store_electron_idSF      = ps.getUntrackedParameter<bool>("store_electron_idSF", false);
  store_photon_idSF        = ps.getUntrackedParameter<bool>("store_photon_idSF", false);
    
  //Scale and Smearing for electron and photon
  dataYear_ = ps.getParameter<int>("dataYear");
  dataPeriod_ = ps.getParameter<std::string>("dataPeriod");
  useETDependentCorrections_ = ps.getParameter<bool>("useETDependentCorrections");
  applyEGMCorrections_ = ps.getParameter<bool>("applyEGMCorrections");
  
  isData_ = ps.getParameter<bool>("isData");
  
  Service<TFileService> fs;
  tree_    = fs->make<TTree>("EventTree", "Event data (tag V10_02_10_04)");
  hEvents_ = fs->make<TH1F>("hEvents",    "total processed and skimmed events",   2,  0,   2);

  branchesGlobalEvent(tree_);

  if (doGenParticles_) {
    branchesGenInfo(tree_, fs);
    branchesGenPart(tree_);
  }
  
  branchesMET(tree_);
  branchesPhotons(tree_);
  branchesElectrons(tree_);
  branchesMuons(tree_);
}

ggNtuplizer::~ggNtuplizer() {
  cleanupPhotons();
 
}

void ggNtuplizer::analyze(const edm::Event& e, const edm::EventSetup& es) {

  hEvents_->Fill(0.5);
  
  edm::Handle<reco::VertexCollection> vtxHandle;
  e.getByToken(vtxLabel_, vtxHandle);
  edm::Handle<edm::View<pat::Muon>> muons;
  e.getByToken(tok_muons_, muons);

  reco::Vertex vtx;

  // best-known primary vertex coordinates
  math::XYZPoint pv(0, 0, 0);
  for (vector<reco::Vertex>::const_iterator v = vtxHandle->begin(); v != vtxHandle->end(); ++v) {
    bool isFake = (v->chi2() == 0 && v->ndof() == 0);
    if (!isFake) {
      pv.SetXYZ(v->x(), v->y(), v->z());
      vtx = *v;
      break;
    }
  }

  //  initTriggerFilters(e);
  fillGlobalEvent(e, es);

  if (!e.isRealData()) {
    fillGenInfo(e);
    if (doGenParticles_)
      fillGenPart(e);
  }

  fillMET(e, es);
  fillElectrons(e, es, pv);
  fillMuons(e, pv, vtx);
  fillPhotons(e, es); 

  hEvents_->Fill(1.5);
  tree_->Fill();


  //scale and smearing for electron and photon
   if (applyEGMCorrections_) {
        try {
            egmCorrectionManager_ = std::make_unique<EGMCorrectionManager>(
                dataYear_, dataPeriod_, useETDependentCorrections_
									   );
	}catch (const std::exception& e) {
	  applyEGMCorrections_ = false;
        }
   }

   if (store_electron_idSF || store_photon_idSF) {
        egmIDSFManager_ = std::make_unique<EGMIDSFManager>(dataYear_, dataPeriod_);
    }
	

  
}

DEFINE_FWK_MODULE(ggNtuplizer);
