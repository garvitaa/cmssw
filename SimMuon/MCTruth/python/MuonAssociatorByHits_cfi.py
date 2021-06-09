import FWCore.ParameterSet.Config as cms


muonAssociatorByHitsCommonParameters = cms.PSet(
    dumpInputCollections = cms.untracked.bool(False),
    #
    #....... general input parameters
    #
    # include invalid muon hits
    includeZeroHitMuons = cms.bool(True),
    #
    # accept mismatched association of tracker/muon stub of global hit patterns
    acceptOneStubMatchings = cms.bool(False),
    # reject bad global muons made of only tracker hits
    rejectBadGlobal = cms.bool(True),
    #
    # switches to be set according to the input Track collection
    UseTracker = cms.bool(True),
    UseMuon = cms.bool(True),
    #
    # cuts for the muon stub
    AbsoluteNumberOfHits_muon = cms.bool(False),
    NHitCut_muon = cms.uint32(0),
    EfficiencyCut_muon = cms.double(0.),
    PurityCut_muon = cms.double(0.),
    #
    # cuts for the tracker stub
    AbsoluteNumberOfHits_track = cms.bool(False),
    NHitCut_track = cms.uint32(0),
    EfficiencyCut_track = cms.double(0.),
    PurityCut_track = cms.double(0.),
    #
    # switches for the tracker stub
    UsePixels = cms.bool(True),
    UseGrouped = cms.bool(True),
    UseSplitting = cms.bool(True),
    ThreeHitTracksAreSpecial = cms.bool(False),
    #
    # for DT Hit associator
    crossingframe = cms.bool(False),
    simtracksTag = cms.InputTag("g4SimHits"),
    simtracksXFTag = cms.InputTag("mix","g4SimHits"),
    #
    DTsimhitsTag = cms.InputTag("g4SimHits","MuonDTHits"),
    DTsimhitsXFTag = cms.InputTag("mix","g4SimHitsMuonDTHits"),
    DTdigiTag = cms.InputTag("simMuonDTDigis"),
    DTdigisimlinkTag = cms.InputTag("simMuonDTDigis"),
    DTrechitTag = cms.InputTag("dt1DRecHits"),
    #
    dumpDT = cms.bool(False),
    links_exist = cms.bool(True),
    associatorByWire = cms.bool(False),
    #
    # for CSC Hit associator
    CSCsimHitsTag = cms.InputTag("g4SimHits","MuonCSCHits"),
    CSCsimHitsXFTag = cms.InputTag("mix","g4SimHitsMuonCSCHits"),
    CSClinksTag = cms.InputTag("simMuonCSCDigis","MuonCSCStripDigiSimLinks"),
    CSCwireLinksTag = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigiSimLinks"),
    #
    # for RPC Hit associator
    RPCsimhitsTag = cms.InputTag("g4SimHits","MuonRPCHits"),
    RPCsimhitsXFTag = cms.InputTag("mix","g4SimHitsMuonRPCHits"),
    RPCdigisimlinkTag = cms.InputTag("simMuonRPCDigis","RPCDigiSimLink"),
    #
    # for GEM Hit associator
    useGEMs = cms.bool(False),
    GEMsimhitsTag = cms.InputTag("g4SimHits","MuonGEMHits"),
    GEMsimhitsXFTag = cms.InputTag("mix","g4SimHitsMuonGEMHits"),
    GEMdigisimlinkTag = cms.InputTag("simMuonGEMDigis","GEM"),
    #
    # for Tracker Hit associator
    #
    associatePixel = cms.bool(True),
    associateStrip = cms.bool(True),
    usePhase2Tracker = cms.bool(False),
    pixelSimLinkSrc = cms.InputTag("simSiPixelDigis"),
    stripSimLinkSrc = cms.InputTag("simSiStripDigis"),
    phase2TrackerSimLinkSrc  = cms.InputTag("simSiPixelDigis","Tracker"),
    associateRecoTracks = cms.bool(True),
    #                                
    ROUList = cms.vstring('TrackerHitsTIBLowTof', 
        'TrackerHitsTIBHighTof', 
        'TrackerHitsTIDLowTof', 
        'TrackerHitsTIDHighTof', 
        'TrackerHitsTOBLowTof', 
        'TrackerHitsTOBHighTof', 
        'TrackerHitsTECLowTof', 
        'TrackerHitsTECHighTof', 
        'TrackerHitsPixelBarrelLowTof', 
        'TrackerHitsPixelBarrelHighTof', 
        'TrackerHitsPixelEndcapLowTof', 
        'TrackerHitsPixelEndcapHighTof'),
    #
    # to associate to reco::Muon segments 
    inputDTRecSegment4DCollection = cms.InputTag("dt4DSegments"),
    inputCSCSegmentCollection = cms.InputTag("cscSegments"),
)

from Configuration.Eras.Modifier_run3_GEM_cff import run3_GEM
from Configuration.Eras.Modifier_phase2_tracker_cff import phase2_tracker
run3_GEM.toModify(muonAssociatorByHitsCommonParameters, useGEMs = True)
phase2_tracker.toModify(muonAssociatorByHitsCommonParameters,
    usePhase2Tracker = True,
    pixelSimLinkSrc = "simSiPixelDigis:Pixel",
)

from Configuration.Eras.Modifier_fastSim_cff import fastSim
fastSim.toModify(muonAssociatorByHitsCommonParameters,
    simtracksTag = "fastSimProducer",
    DTsimhitsTag  = "MuonSimHits:MuonDTHits",
    CSCsimHitsTag = "MuonSimHits:MuonCSCHits",
    RPCsimhitsTag = "MuonSimHits:MuonRPCHits",
    simtracksXFTag = "mix:fastSimProducer",
    DTsimhitsXFTag  = "mix:MuonSimHitsMuonDTHits",
    CSCsimHitsXFTag = "mix:MuonSimHitsMuonCSCHits",
    RPCsimhitsXFTag = "mix:MuonSimHitsMuonRPCHits",
    ROUList = ['fastSimProducerTrackerHits']
)

from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2
premix_stage2.toModify(muonAssociatorByHitsCommonParameters,
    pixelSimLinkSrc = "mixData:PixelDigiSimLink",
    stripSimLinkSrc = "mixData:StripDigiSimLink",
    phase2TrackerSimLinkSrc = "mixData:Phase2OTDigiSimLink",
)
  
muonAssociatorByHits = cms.EDProducer("MuonAssociatorEDProducer",
    # COMMON CONFIGURATION
    muonAssociatorByHitsCommonParameters,
    # for Muon Track association
    #
    #     input collections
    #
    # ... reco::Track collection
    tracksTag = cms.InputTag("globalMuons"),
    ignoreMissingTrackCollection = cms.untracked.bool(False),
    # ... TrackingParticle collection
    tpTag = cms.InputTag("mix","MergedTrackTruth"),
    tpRefVector = cms.bool(False)
)

premix_stage2.toModify(muonAssociatorByHits, tpTag = "mixData:MergedTrackTruth")
