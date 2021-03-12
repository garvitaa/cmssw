import FWCore.ParameterSet.Config as cms

from RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizerPreSplitting_cfi import siPixelClustersPreSplitting
from RecoLocalTracker.SiPixelClusterizer.siPixelRawToClusterCUDA_cfi import siPixelRawToClusterCUDA as _siPixelRawToClusterCUDA
from RecoLocalTracker.SiPixelClusterizer.siPixelDigisClustersFromSoA_cfi import siPixelDigisClustersFromSoA as _siPixelDigisClustersFromSoA
from CalibTracker.SiPixelESProducers.siPixelROCsStatusAndMappingWrapperESProducer_cfi import *
from CalibTracker.SiPixelESProducers.siPixelGainCalibrationForHLTGPU_cfi import *

siPixelClustersPreSplittingTask = cms.Task(siPixelClustersPreSplitting)

siPixelClustersPreSplittingCUDA = _siPixelRawToClusterCUDA.clone()
from Configuration.Eras.Modifier_run3_common_cff import run3_common
run3_common.toModify(siPixelClustersPreSplittingCUDA,
  isRun2=False
)

siPixelDigisClustersPreSplitting = _siPixelDigisClustersFromSoA.clone()
siPixelClustersPreSplittingTaskCUDA = cms.Task(
    siPixelClustersPreSplittingCUDA,
    siPixelDigisClustersPreSplitting,
)

from Configuration.ProcessModifiers.gpu_cff import gpu
_siPixelClustersPreSplittingTask_gpu = siPixelClustersPreSplittingTask.copy()
_siPixelClustersPreSplittingTask_gpu.add(siPixelClustersPreSplittingTaskCUDA)
gpu.toReplaceWith(siPixelClustersPreSplittingTask, _siPixelClustersPreSplittingTask_gpu)
