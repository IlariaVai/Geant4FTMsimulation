#ifndef TRGEMANALYSIS_HH
#define TRGEMANALYSIS_HH 1

#include "globals.hh"
#include <vector>

class G4Run;
class G4Event;
class G4ParticleDefinition;
class TFile;
class TH1D;
class TNtuple;
class TTree;

class TrGEMAnalysis {

   public:

      static TrGEMAnalysis* GetInstance();
      ~TrGEMAnalysis();

      void PrepareNewEvent(const G4Event* anEvent);
      void PDGHisto(std::vector<G4double> PDGvector) ;
      void EndOfEvent(const G4Event* anEvent);
      void PrepareNewRun(const G4Run* aRun);
      void EndOfRun(const G4Run* aRun);
      void AddSecondary(const G4ParticleDefinition* part);
      void AddGapSecondary(const G4ParticleDefinition* part, G4int gapNum);
      void AddEDep(G4double edep, G4double z);
      void AddEDepSD(G4double z, G4int copyno);
      //void SetEcalZposition(G4double val) { eCalZposition = val; };
      void SetBeam(const G4ParticleDefinition* part, G4double energy);
      //void CreateBranch(G4String name, G4int evtNo, G4String type) ;
      void AddParticlesPerEvent(G4int PDGCode) ;
      
      void SetEdep(G4double *someEdep   ) ;
      void SetPrimaryAcceptance(G4bool someBool) ;
      void SetEleGap(G4bool elegap ) ;
      void SetChargeGap(G4bool chargegap) ;
      void SavePrimary( G4double primaryene,
      			G4double primaryposX, G4double primaryposY, G4double primaryposZ,  
      			G4double primarypX, G4double primarypY, G4double primarypZ,  
      			G4double primarypdirX, G4double primarypdirY, G4double primarypdirZ,  
			G4double zabsorption, std::string volabsorption);
      void SaveTrack(G4int trackindex, G4int pdg, G4int parent, G4int isprimary, 
      		     std::string genprocess, std::string genvolume, G4double genz, std::string volname, G4double kinene , 
		     G4double x, G4double y, G4double z, G4double px, G4double py, G4double pz, G4double pdirx,  G4double pdiry, G4double pdirz
      		     );
   private:

	 TrGEMAnalysis();
	 static TrGEMAnalysis* singleton;

	 bool isNewEvent ;
	 G4int eventCounter ;

	 // beam and calorimeter geometry
	 const G4ParticleDefinition* beamParticle;
	 G4double beamEnergy;
	 //G4double eCalZposition;

	 // simple analysis parameters
	 G4double thisEventTotEM;
	 G4double thisEventCentralEM;
	 G4double thisRunTotEM;
	 G4double thisRunTotEM2;
	 G4double thisRunCentralEM;
	 G4double thisRunCentralEM2;

	 // counters
	 G4int thisEventSecondaries;

	 G4bool eleGap ;
	 G4bool chargeGap ;

	 G4double Edep[3] ;

	 G4double primaryEne;
	 G4double primaryPosX;
	 G4double primaryPosY;
	 G4double primaryPosZ;
	 G4double primaryPX;
	 G4double primaryPY;
	 G4double primaryPZ;
	 G4double primaryPdirX;
	 G4double primaryPdirY;
	 G4double primaryPdirZ;
	 G4double zAbsorption;
	 std::string volAbsorption;
	 
	 std::vector<G4int> TrackIndex ;
         std::vector<G4int>* pTrackIndex;
	 std::vector<G4int> TrackPart ;
         std::vector<G4int>* pTrackPart;
         std::vector<G4int> TrackParent ;
         std::vector<G4int>* pTrackParent;
         std::vector<G4int> TrackIsPrimary ;
         std::vector<G4int>* pTrackIsPrimary;
  	 std::vector<std::string> TrackGenProcess ;
         std::vector<std::string>* pTrackGenProcess ;
         std::vector<std::string> TrackGenVolume ;
         std::vector<std::string>* pTrackGenVolume;
         std::vector<G4double> TrackGenZ ;
         std::vector<G4double>* pTrackGenZ;
         std::vector<std::string> TrackVolume ;
         std::vector<std::string>* pTrackVolume;
         std::vector<G4double> TrackEne ;
         std::vector<G4double>* pTrackEne;
         std::vector<G4double>  TrackPosX ;
         std::vector<G4double>* pTrackPosX;
         std::vector<G4double>  TrackPosY ;
         std::vector<G4double>* pTrackPosY;
         std::vector<G4double>  TrackPosZ ;
         std::vector<G4double>* pTrackPosZ;
         std::vector<G4double>  TrackPX ;
         std::vector<G4double>* pTrackPX;
         std::vector<G4double>  TrackPY ;
         std::vector<G4double>* pTrackPY;
         std::vector<G4double>  TrackPZ ;
         std::vector<G4double>* pTrackPZ;
         std::vector<G4double>  TrackPdirX ;
         std::vector<G4double>* pTrackPdirX;
         std::vector<G4double>  TrackPdirY ;
         std::vector<G4double>* pTrackPdirY;
         std::vector<G4double>  TrackPdirZ ;
         std::vector<G4double>* pTrackPdirZ;

	
	
	 G4int vecProcNo ;
	 G4bool primaryAcceptance ;

	

	 // ROOT objects
	 TFile*    m_ROOT_file;
	 TTree     *t ;

};

#endif /* TRGEMANALYSIS_HH */
