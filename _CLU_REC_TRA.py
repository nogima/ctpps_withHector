import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('CTPPS2',eras.Run2_25ns)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cclu_info'),
    cclu_info = cms.untracked.PSet( threshold = cms.untracked.string('INFO'))
)
process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)

# Track memory leaks
process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:step25_RAW2DIGI.root'
),
duplicateCheckMode = cms.untracked.string("checkEachFile")
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_hlt_relval', '')


# raw-to-digi conversion
#process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

############
process.o1 = cms.OutputModule("PoolOutputModule",
        outputCommands = cms.untracked.vstring('drop *',
                                               'keep CTPPS*_*_*_*'
    
),
        fileName = cms.untracked.string('simevent_CTPPS_CLU__REC_TRA_DB_mem.root')
        )

#process.load("RecoCTPPS.Configuration.recoCTPPS_cff")


#process.ALL = cms.Path(process.recoCTPPS)

from Geometry.VeryForwardGeometry.geometryRP_cfi import *

# local clusterizer
from RecoCTPPS.PixelLocal.ctppsPixelLocalReconstruction_cff import *
##from Geometry.VeryForwardGeometryBuilder.ctppsIncludeAlignments_cfi import *
##process.load("RecoCTPPS.PixelLocal.ctppsPixelClusters_cfi")
##process.load("RecoCTPPS.PixelLocal.ctppsPixelRecHits_cfi")
##process.load("RecoCTPPS.PixelLocal.ctppsPixelLocalTracks_cfi")
process.load("RecoCTPPS.PixelLocal.ctppsPixelLocalReconstruction_cff")
#process.ALL = cms.Path(process.ctppsPixelClusters*process.ctppsPixelRecHits*process.ctppsPixelLocalTracks)
process.ALL = cms.Path(process.ctppsPixelLocalReconstruction)
process.outpath = cms.EndPath(process.o1)

process.schedule = cms.Schedule(process.ALL,process.outpath)

# filter all path with the production filter sequence
for path in process.paths:
    #  getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq
    getattr(process,path)._seq = getattr(process,path)._seq
