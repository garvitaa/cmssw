import FWCore.ParameterSet.Config as cms

process = cms.Process("PWRITE")

#########################
# message logger
######################### 

process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(
        enable = cms.untracked.bool(False)
    ),
    cout = cms.untracked.PSet(
        enable = cms.untracked.bool(True)
    ),
    debugModules = cms.untracked.vstring('*'),
    files = cms.untracked.PSet(
        readFromFile_57620 = cms.untracked.PSet(
            threshold = cms.untracked.string('DEBUG')
        )
    )
)


#########################
# maxEvents ...
#########################

process.maxEvents = cms.untracked.PSet(
     input = cms.untracked.int32(1))

process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(1)
)

#########################
# DQM services
#########################
process.load("DQMServices.Core.DQM_cfg")
#from DQMServices.Core.DQM_cfg import *


########################
# DB parameters
########################

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
#connect = cms.string("sqlite_file:historicDQM.db"),
BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
connect = cms.string("sqlite_file:dbfile.db"),
timetype = cms.untracked.string("runnumber"),
DBParameters = cms.PSet(
   authenticationPath = cms.untracked.string("/afs/cern.ch/cms/DB/conddb"),
   messageLevel = cms.untracked.int32(4)
),
toPut = cms.VPSet(
    cms.PSet(
        record = cms.string("SiStripNoisesRcd"),
        tag = cms.string("SiStripNoises_test1")
    )
),
logconnect = cms.untracked.string("sqlite_file:log.db") 
)


########################
# POPCON Application
########################
process.siStripPopConNoisesDQM = cms.EDAnalyzer("SiStripPopConNoisesDQM",
    record = cms.string("SiStripNoisesRcd"),
    loggingOn = cms.untracked.bool(True),
    SinceAppendMode = cms.bool(True),
    Source = cms.PSet(
        since = cms.untracked.uint32(111753),
        debug = cms.untracked.bool(False),
        ######################
        ## NoisesDQMService ##
        ######################
        RunNb = cms.uint32(111753),
        accessDQMFile = cms.bool(True),
        FILE_NAME = cms.untracked.string("Playback_V0001_SiStrip_R000111753_T00000030.root"),
        ME_DIR = cms.untracked.string("Run 111753"),
        histoList = cms.VPSet()
    )
)

# Schedule

process.p = cms.Path(process.siStripPopConNoisesDQM)
process.asciiPrint = cms.OutputModule("AsciiOutputModule") 
process.ep = cms.EndPath(process.asciiPrint)
    



