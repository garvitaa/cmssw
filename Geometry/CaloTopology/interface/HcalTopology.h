#ifndef GEOMETRY_CALOTOPOLOGY_HCALTOPOLOGY_H
#define GEOMETRY_CALOTOPOLOGY_HCALTOPOLOGY_H 1

#include <vector>
#include <map>
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalCalibDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "Geometry/HcalCommonData/interface/HcalTopologyMode.h"
#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"

/** \class HcalTopology

   The HcalTopology class contains a set of constants which represent
   the topology (tower relationship) of the CMS HCAL as built.  These
   constants can be used to determine neighbor relationships and
   existence of cells.

   For use with limited setups (testbeam, cosmic stands, etc), the
   topology can be limited by creating a rejection list -- a list of
   cells which would normally exist in the full CMS HCAL, but are not
   present for the specified topology.
    
*/
class HcalTopology : public CaloSubdetectorTopology {
public:

  HcalTopology(const HcalDDDRecConstants* hcons, const bool mergePosition=false);
  HcalTopology(HcalTopologyMode::Mode mode, int maxDepthHB, int maxDepthHE, HcalTopologyMode::TriggerMode tmode=HcalTopologyMode::TriggerMode_2009);
	
  HcalTopologyMode::Mode mode() const {return mode_;}
  HcalTopologyMode::TriggerMode triggerMode() const { return triggerMode_; }
  /** Add a cell to exclusion list */
  void exclude(const HcalDetId& id);
  /** Exclude an entire subdetector */
  void excludeSubdetector(HcalSubdetector subdet);
  /** Exclude an eta/phi/depth range for a given subdetector */
  int exclude(HcalSubdetector subdet, int ieta1, int ieta2, int iphi1, int iphi2, int depth1=1, int depth2=4);

  static std::string producerTag() { return "HCAL" ; }

  /// return a linear packed id
  unsigned int detId2denseId(const DetId& id) const override;
  /// return a linear packed id
  DetId denseId2detId(unsigned int /*denseid*/) const override;
  /// return a count of valid cells (for dense indexing use)
  unsigned int ncells() const override;
  /// return a version which identifies the given topology
  int topoVersion() const override;

  /** Is this a valid cell id? */
  bool valid(const DetId& id) const override;
  /** Is this a valid cell id? */
  bool validHcal(const HcalDetId& id) const;
  bool validDetId(HcalSubdetector subdet, int ieta, int iphi, int depth) const;
  bool validHT(const HcalTrigTowerDetId& id) const;
  bool validCalib(const HcalCalibDetId& id) const;
  /** Is this a valid cell in context of Plan1 */
  bool validHcal(const HcalDetId& id, const unsigned int flag) const;
  /** Flag=0 for unmerged Id's; =1 for for merged Id's; =2 for either */

  /** Get the neighbors of the given cell in east direction*/
  std::vector<DetId> east(const DetId& id) const override;
  /** Get the neighbors of the given cell in west direction*/
  std::vector<DetId> west(const DetId& id) const override;
  /** Get the neighbors of the given cell in north direction*/
  std::vector<DetId> north(const DetId& id) const override;
  /** Get the neighbors of the given cell in south direction*/
  std::vector<DetId> south(const DetId& id) const override;
  /** Get the neighbors of the given cell in up direction (outward)*/
  std::vector<DetId> up(const DetId& id) const override;
  /** Get the neighbors of the given cell in down direction (inward)*/
  std::vector<DetId> down(const DetId& id) const override;

  /** Get the neighbors of the given cell with higher (signed) ieta */
  int incIEta(const HcalDetId& id, HcalDetId neighbors[2]) const;
  /** Get the neighbors of the given cell with lower (signed) ieta */
  int decIEta(const HcalDetId& id, HcalDetId neighbors[2]) const;
  /** Get the neighbor (if present) of the given cell with higher iphi */
  bool incIPhi(const HcalDetId& id, HcalDetId &neighbor) const;
  /** Get the neighbor (if present) of the given cell with lower iphi */
  bool decIPhi(const HcalDetId& id, HcalDetId &neighbor) const;
  /** Get the detector behind this one */
  bool incrementDepth(HcalDetId& id) const;
  /** Get the detector in front of this one */
  bool decrementDepth(HcalDetId& id) const;

  int firstHBRing() const {return firstHBRing_;}
  int lastHBRing()  const {return lastHBRing_;}
  int firstHERing() const {return firstHERing_;}
  int lastHERing()  const {return lastHERing_;}
  int firstHFRing() const {return firstHFRing_;}
  int lastHFRing()  const {return lastHFRing_;}
  int firstHORing() const {return firstHORing_;}
  int lastHORing()  const {return lastHORing_;}

  int firstHEDoublePhiRing()   const {return firstHEDoublePhiRing_;} 
  int firstHEQuadPhiRing()     const {return firstHEQuadPhiRing_;} 
  int firstHFQuadPhiRing()     const {return firstHFQuadPhiRing_;}
  int firstHETripleDepthRing() const {return firstHETripleDepthRing_;}
  int singlePhiBins()          const {return singlePhiBins_;}
  int doublePhiBins()          const {return doublePhiBins_;}

  /// finds the number of depth bins and which is the number to start with
  void depthBinInformation(HcalSubdetector subdet, int etaRing,
			   int iphi, int zside, int & nDepthBins,
			   int & startingBin) const;
  bool mergedDepth29(HcalDetId id) const {
    return hcons_->mergedDepthList29(id.ieta(), id.iphi(), id.depth());
  }
  std::vector<int> mergedDepthList29(HcalDetId id) const {
    return hcons_->mergedDepthList29(id.ieta(), id.iphi());
  }

  /// how many phi segments in this ring
  int nPhiBins(int etaRing) const;
  int nPhiBins(HcalSubdetector subdet, int etaRing) const;

  /// eta and phi index from eta, phi values
  int etaRing(HcalSubdetector subdet, double eta) const;
  int phiBin(HcalSubdetector subdet, int etaRing, double phi) const;

  /// for each of the ~17 depth segments, specify which readout bin they belong to
  /// if the ring is not found, the first one with a lower ring will be returned.
  void getDepthSegmentation(const unsigned ring, 
			    std::vector<int> & readoutDepths,
			    const bool flag=false) const;
  void setDepthSegmentation(const unsigned ring, 
			    const std::vector<int> & readoutDepths,
			    const bool flag);
  /// returns the boundaries of the depth segmentation, so that the first
  /// result is the first segment, and the second result is the first one
  /// of the next segment.  Used for calculating physical bounds.
  std::pair<int, int> segmentBoundaries(const unsigned ring,
					const unsigned depth,
					const bool flag=false) const;
  int getPhiZOne(std::vector<std::pair<int,int> >& phiz) const {return hcons_->getPhiZOne(phiz);}

  unsigned int getHBSize() const {return HBSize_;}
  unsigned int getHESize() const {return HESize_;}
  unsigned int getHOSize() const {return HOSize_;}
  unsigned int getHFSize() const {return HFSize_;}
  unsigned int getHTSize() const {return HTSize_;}
  unsigned int getCALIBSize() const {return CALIBSize_;}

  int maxDepthHB() const { return maxDepthHB_;}
  int maxDepthHE() const { return maxDepthHE_;}
  int maxDepth( void ) const;
  int maxDepth(HcalSubdetector subdet) const;
  double etaMax(HcalSubdetector subdet) const;
  std::pair<double,double> etaRange(HcalSubdetector subdet, int ieta) const;

  /// return a linear packed id from HB
  unsigned int detId2denseIdHB(const DetId& id) const;
  /// return a linear packed id from HE
  unsigned int detId2denseIdHE(const DetId& id) const;
  /// return a linear packed id from HO
  unsigned int detId2denseIdHO(const DetId& id) const;
  /// return a linear packed id from HF
  unsigned int detId2denseIdHF(const DetId& id) const;
  /// return a linear packed id from HT
  unsigned int detId2denseIdHT(const DetId& id) const;
  /// return a linear packed id from CALIB
  unsigned int detId2denseIdCALIB(const DetId& id) const;
  /// return a Calib DetId from linear packed id
  HcalCalibDetId denseId2detIdCALIB(const unsigned int& id) const;

  unsigned int getNumberOfShapes() const { return numberOfShapes_; }
  bool isBH() const { return ((hcons_ == nullptr) ? false : hcons_->isBH()); }

  const HcalDDDRecConstants* dddConstants () const {return hcons_;}
  bool  withSpecialRBXHBHE() const {return hcons_->withSpecialRBXHBHE();}
  HcalDetId mergedDepthDetId(const HcalDetId& id) const {return hcons_->mergedDepthDetId(id); }
  bool  getMergePositionFlag() const {return mergePosition_;}
  void  unmergeDepthDetId(const HcalDetId& id,
			  std::vector<HcalDetId>& ids) const {
    hcons_->unmergeDepthDetId(id, ids);
  }
  // Returns the DetId of the front Id if it is a merged RecHit in "Plan 1"
  HcalDetId idFront(const HcalDetId& id) const {return hcons_->idFront(id);}
  HcalDetId idBack (const HcalDetId& id) const {return hcons_->idBack(id);}

private:
  /** Get the neighbors of the given cell with higher absolute ieta */
  int incAIEta(const HcalDetId& id, HcalDetId neighbors[2]) const;
  /** Get the neighbors of the given cell with lower absolute ieta */
  int decAIEta(const HcalDetId& id, HcalDetId neighbors[2]) const;

  /** Is this a valid cell id, ignoring the exclusion list */
  bool validDetIdPreLS1(const HcalDetId& id) const;
  bool validRaw(const HcalDetId& id) const;
  unsigned int detId2denseIdPreLS1 (const DetId& id) const;
  bool isExcluded(const HcalDetId& id) const;

  const HcalDDDRecConstants *hcons_;
  bool                       mergePosition_;
  std::vector<HcalDetId> exclusionList_;
  bool excludeHB_, excludeHE_, excludeHO_, excludeHF_;

  HcalTopologyMode::Mode mode_;
  HcalTopologyMode::TriggerMode triggerMode_;

  int firstHBRing_, lastHBRing_;
  int firstHERing_, lastHERing_;
  int firstHFRing_, lastHFRing_;
  int firstHORing_, lastHORing_;

  std::vector<HcalDDDRecConstants::HcalEtaBin> etaBinsHB_, etaBinsHE_;
  int nEtaHB_, nEtaHE_;

  int firstHEDoublePhiRing_, firstHEQuadPhiRing_, firstHFQuadPhiRing_;
  int firstHETripleDepthRing_;
  int singlePhiBins_, doublePhiBins_;
  int maxDepthHB_, maxDepthHE_, maxDepthHF_;
  int etaHE2HF_, etaHF2HE_;
  int maxEta_, maxPhiHE_;

  unsigned int HBSize_;
  unsigned int HESize_;
  unsigned int HOSize_;
  unsigned int HFSize_;
  unsigned int HTSize_;
  unsigned int CALIBSize_;
  unsigned int numberOfShapes_;

  std::vector<double> etaTable, etaTableHF, dPhiTable, dPhiTableHF;
  std::vector<double> phioff;
  std::vector<int>    unitPhi, unitPhiHF;

  int topoVersion_;
    
  // index is ring;
  typedef std::map<unsigned, std::vector<int> > SegmentationMap;
  SegmentationMap depthSegmentation_;
  SegmentationMap depthSegmentationOne_;

  enum { kHBhalf = 1296 ,
	 kHEhalf = 1296 ,
	 kHOhalf = 1080 ,
	 kHFhalf = 864  ,
	 kHThalf = 2088,
	 kZDChalf = 11,
	 kCASTORhalf = 224,
	 kCALIBhalf = 693,
         kHThalfPhase1 = 2520 ,
	 kHcalhalf = kHBhalf + kHEhalf + kHOhalf + kHFhalf } ;
  enum { kSizeForDenseIndexingPreLS1 = 2*kHcalhalf } ;
  enum { kHBSizePreLS1 = 2*kHBhalf } ;
  enum { kHESizePreLS1 = 2*kHEhalf } ;
  enum { kHOSizePreLS1 = 2*kHOhalf } ;
  enum { kHFSizePreLS1 = 2*kHFhalf } ;
  enum { kHTSizePreLS1 = 2*kHThalf } ;
  enum { kHTSizePhase1 = 2*kHThalfPhase1 } ;
  enum { kCALIBSizePreLS1 = 2*kCALIBhalf };
  static const int minMaxDepth_ = 4;
  static const unsigned int minPhi_=1, maxPhi_=72;
  static const unsigned int nchanCalibHB_=3, nchanCalibHE_=6;
  static const unsigned int nchanCalibHF_=3, nchanCalibHO_=2, chanCalibHOs_=7;
  static constexpr int chanCalibHB_[nchanCalibHB_] = {0,1,2};
  static constexpr int chanCalibHE_[nchanCalibHE_] = {0,1,3,4,5,6};
  static constexpr int chanCalibHF_[nchanCalibHF_] = {0,1,8};
  static constexpr int chanCalibHO_[nchanCalibHO_] = {0,1};
  static const unsigned int nEtaCalibHBHEHF_=2, nEtaCalibHO_=5;
  static constexpr int etaCalibHBHEHF_[nEtaCalibHBHEHF_] = {-1,1};
  static constexpr int etaCalibHO_[nEtaCalibHO_] = {-2,-1,0,1,2};
  static constexpr int phiCalibHO_[nEtaCalibHO_] = {59,47,53,47,47};
  static const unsigned int mPhiCalibHBHE_=4, mPhiCalibHF_=18;
  static const unsigned int mPhiCalibHO1_=12, mPhiCalibHO0_=mPhiCalibHO1_/2;
  static const unsigned int kPhiCalibHBHE_=maxPhi_/mPhiCalibHBHE_;
  static const unsigned int kchanCalibHB_=nchanCalibHB_*kPhiCalibHBHE_/2;
  static const unsigned int kchanCalibHE_=nchanCalibHE_*kPhiCalibHBHE_/2;
  static const unsigned int nCalibHB_=kPhiCalibHBHE_*nchanCalibHB_*nEtaCalibHBHEHF_;
  static const unsigned int nCalibHE_=kPhiCalibHBHE_*nchanCalibHE_*nEtaCalibHBHEHF_;
  static const unsigned int kPhiCalibHF_=maxPhi_/mPhiCalibHF_;
  static const unsigned int kchanCalibHF_=nchanCalibHF_*kPhiCalibHF_/2;
  static const unsigned int nCalibHF_=kPhiCalibHF_*nchanCalibHF_*nEtaCalibHBHEHF_;
  static const unsigned int kPhiCalibHO0_=maxPhi_/mPhiCalibHO0_;
  static const unsigned int kPhiCalibHO1_=maxPhi_/mPhiCalibHO1_;
  static const unsigned int kPhiCalibHO2_=(3*kPhiCalibHO1_+kPhiCalibHO0_);
  static const unsigned int kchanCalibHO_=nchanCalibHO_*kPhiCalibHO1_/2;
  static const unsigned int kchanCalibHO1_=maxPhi_/2;
  static const unsigned int kEtaPhiCalibHO_=kPhiCalibHO1_*(1+nEtaCalibHO_);
  static const unsigned int nCalibHO0_=kEtaPhiCalibHO_*nchanCalibHO_;
  static const unsigned int nCalibHO_=(nEtaCalibHO_+nCalibHO0_);
  static const unsigned int kOffCalibHB_=0, kOffCalibHE_=(kOffCalibHB_+nCalibHB_);
  static const unsigned int kOffCalibHF_=(kOffCalibHE_+nCalibHE_);
  static const unsigned int kOffCalibHO0_=(kOffCalibHF_+nCalibHF_);
  static const unsigned int kOffCalibHO_=(kOffCalibHO0_+nCalibHO0_);
  static const unsigned int kOffCalibHOX_=(kOffCalibHO0_+nCalibHO_);
  static const unsigned int nEtaCalibHOX_=2, ctypeHX_=-999;
  static constexpr unsigned int etaCalibHOX_[nEtaCalibHOX_]={4,15};
  static constexpr unsigned int mPhiCalibHOX_[nEtaCalibHOX_]={2,1};
  static constexpr unsigned int nPhiCalibHOX_[nEtaCalibHOX_]={36,72};
  static const int nCalibHOX_=(2*(nPhiCalibHOX_[0]+nPhiCalibHOX_[1]));
  static constexpr int phiCalibHOX_[37]= {36, 1,38, 3,40,41, 6,43, 8,45,10,47,
					  12,49,14,51,16,17,54,19,56,21,58,23,
					  60,25,62,27,64,65,30,67,32,69,34,71,
					  36};
  static const unsigned int kOffCalibHBX_=(kOffCalibHOX_+nCalibHOX_);
  static const unsigned int nEtaCalibHBX_=1,  etaCalibHBX_=16;
  static const unsigned int mPhiCalibHBX_=1,  nPhiCalibHBX_=maxPhi_/mPhiCalibHBX_;
  static const unsigned int nCalibHBX_=(2*nPhiCalibHBX_*nEtaCalibHBX_);
  static const unsigned int kOffCalibHEX_=(kOffCalibHBX_+nCalibHBX_);
  static const unsigned int nEtaCalibHEX_=2;
  static constexpr int etaCalibHEX_[nEtaCalibHEX_]={25,27};
  static const unsigned int mPhiCalibHEX_=2,  nPhiCalibHEX_=maxPhi_/mPhiCalibHEX_;
  static const unsigned int nCalibHEX_=(2*nPhiCalibHEX_*nEtaCalibHEX_);
  static const unsigned int kOffCalibHFX_=(kOffCalibHEX_+nCalibHEX_);
};

#endif
