#include "TrGEMAnalysis.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4String.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>

TrGEMAnalysis* TrGEMAnalysis::singleton = 0 ;

TrGEMAnalysis* TrGEMAnalysis::GetInstance() {
   if ( singleton == 0 ) {
      static TrGEMAnalysis analysis;
      singleton = &analysis;
   }
   return singleton;
}

TrGEMAnalysis::~TrGEMAnalysis() 
{}

TrGEMAnalysis::TrGEMAnalysis() 
{
   m_ROOT_file = 0;
}

void TrGEMAnalysis::PrepareNewEvent(const G4Event* /*anEvent*/)
{
   //Reset variables relative to this event
   isNewEvent = true ;
   thisEventTotEM = 0;
   thisEventCentralEM = 0;
   thisEventSecondaries = 0;

  for(G4int t=0;t<3;t++){
  
      Edep[t] = 0. ;
   }
   primaryAcceptance=0;
   eleGap=0;
   chargeGap=0;
   primaryEne=0.;
   zAbsorption=-5;
   primaryPosX=0, primaryPosY=0, primaryPosZ=0;  
   primaryPX=0, primaryPY=0, primaryPZ=0; 
   primaryPdirX=0,  primaryPdirY=0,  primaryPdirZ=0;  
 
   TrackIndex.clear();
   TrackPart.clear();
   TrackParent.clear();
   TrackIsPrimary.clear();
   TrackGenProcess.clear();
   TrackGenVolume.clear();
   TrackGenZ.clear();
   TrackVolume.clear();
   TrackEne.clear();
   TrackPosX.clear() ;
   TrackPosY.clear() ;
   TrackPosZ.clear() ;
   TrackPX.clear() ;
   TrackPY.clear() ;
   TrackPZ.clear() ;
   TrackPdirX.clear() ;
   TrackPdirY.clear() ;
   TrackPdirZ.clear() ;
    
}

void TrGEMAnalysis::PrepareNewRun(const G4Run* /*aRun*/)
{

  eventCounter = 0 ;
  //Reset variables relative to the run
  thisRunTotEM  = 0;
  thisRunTotEM2 = 0;
  thisRunCentralEM  = 0;
  thisRunCentralEM2 = 0;
    
   pTrackIndex	=&TrackIndex;
   pTrackPart	=&TrackPart;
   pTrackParent	 =&TrackParent;
   pTrackIsPrimary=&TrackIsPrimary;
   pTrackGenProcess=&TrackGenProcess;
   pTrackGenVolume	 =&TrackGenVolume;
   pTrackGenZ	=&TrackGenZ;
   pTrackVolume	 =&TrackVolume;
   pTrackEne	=&TrackEne;
   pTrackPosX	=&TrackPosX;
   pTrackPosY	=&TrackPosY;
   pTrackPosZ	=&TrackPosZ;
   pTrackPX	=&TrackPX;
   pTrackPY	=&TrackPY;
   pTrackPZ	=&TrackPZ;
   pTrackPdirX	=&TrackPdirX;
   pTrackPdirY	=&TrackPdirY;
   pTrackPdirZ	=&TrackPdirZ;
 
   // create ROOT file
   m_ROOT_file = new TFile("task.root","RECREATE");
   if(m_ROOT_file) 
      G4cout << "ROOT file task.root is created " << G4endl;
   else {
      //G4Exception("ROOT file task.root has not been created!");
      G4cerr << "ROOT file task.root has not been created!" << G4endl;
   }


   // create tree and branch(es)
   t = new TTree("MyRun","Run with awesome data") ;
 
   t->Branch("Edep",Edep,"Edep[3]/D") ;
   t->Branch("primaryAcceptance",&primaryAcceptance, "primaryAcceptance/O") ;
   t->Branch("eleGap",&eleGap, "eleGap/O") ;
   t->Branch("chargeGap",&chargeGap, "chargeGap/O") ;
   t->Branch("primaryEne",&primaryEne, "primaryEne/D") ;

   t->Branch("primaryPosX",&primaryPosX, "primaryPosX/D") ;
   t->Branch("primaryPosY",&primaryPosY, "primaryPosY/D") ;
   t->Branch("primaryPosZ",&primaryPosZ, "primaryPosZ/D") ;
   t->Branch("primaryPX", &primaryPX, "primaryPX/D") ;
   t->Branch("primaryPY", &primaryPY, "primaryPY/D") ;
   t->Branch("primaryPZ", &primaryPZ, "primaryPZ/D") ;
   t->Branch("primaryPdirX",&primaryPdirX, "primaryPdirX/D") ;
   t->Branch("primaryPdirY",&primaryPdirY, "primaryPdirY/D") ;
   t->Branch("primaryPdirZ",&primaryPdirZ, "primaryPdirZ/D") ;
   t->Branch("zAbsorption", &zAbsorption,   "zAbsorption/D") ;
   t->Branch("volAbsorption", &volAbsorption) ;
 
   t->Branch("TrackIndex",&pTrackIndex);
   t->Branch("TrackPart",&pTrackPart);
   t->Branch("TrackParent",&pTrackParent);
   t->Branch("TrackIsPrimary",&pTrackIsPrimary);
   t->Branch("TrackGenProcess",&pTrackGenProcess);
   t->Branch("TrackGenVolume",&pTrackGenVolume);
   t->Branch("TrackGenZ",&pTrackGenZ);
   t->Branch("TrackVolume",&pTrackVolume);
   t->Branch("TrackEne",&pTrackEne);
   t->Branch("TrackPosX",&pTrackPosX);
   t->Branch("TrackPosY",&pTrackPosY);
   t->Branch("TrackPosZ",&pTrackPosZ);
   t->Branch("TrackPX",&pTrackPX);
   t->Branch("TrackPY",&pTrackPY);
   t->Branch("TrackPZ",&pTrackPZ);
   t->Branch("TrackPdirX",&pTrackPdirX);
   t->Branch("TrackPdirY",&pTrackPdirY);
   t->Branch("TrackPdirZ",&pTrackPdirZ);

}

void TrGEMAnalysis::EndOfEvent(const G4Event* /*anEvent*/)
{
 
   t->Fill() ; 
 
}

void TrGEMAnalysis::EndOfRun(const G4Run* aRun)
{

   //Some print outs
   G4int numEvents = aRun->GetNumberOfEvent();
   if(numEvents == 0) { return; }

   //G4double norm = numEvents*beamEnergy;
   G4cout << "================="<<G4endl;
   G4cout << "Summary for run: " << aRun->GetRunID() << G4endl ;
   G4cout << "  Beam of " << beamParticle->GetParticleName() 
      << " kinetic energy: "<< G4BestUnit(beamEnergy,"Energy") << G4endl ;
   G4cout << "  Event processed:         " << numEvents << G4endl ;
 
    // Writing and closing the ROOT file
   m_ROOT_file->cd() ;
   t->Write() ;
   G4cout << "ROOT: files writing..." << G4endl;
   m_ROOT_file->Write();
   G4cout << "ROOT: files closing..." << G4endl;
   m_ROOT_file->Close();
   delete m_ROOT_file;

}

void TrGEMAnalysis::AddSecondary(const G4ParticleDefinition* part)
{
}


void TrGEMAnalysis::AddGapSecondary(const G4ParticleDefinition* part, G4int gapNum)
{
}

void TrGEMAnalysis::AddEDep(G4double edep, G4double z)
{
}

void TrGEMAnalysis::AddEDepSD(G4double edep, G4int layerIndex)
{
}

void TrGEMAnalysis::SetBeam(const G4ParticleDefinition* part, G4double energy)
{
   beamParticle = part;
   beamEnergy = energy;
}


void TrGEMAnalysis::AddParticlesPerEvent(G4int PDGCode) {
}

void TrGEMAnalysis::SetEdep(G4double *someEdep ) {

   for(G4int t=0;t<3;t++){
      Edep[t] = someEdep[t] ;
   }

}


void TrGEMAnalysis::SetPrimaryAcceptance(G4bool someBool) {

   primaryAcceptance = someBool ;

}

void TrGEMAnalysis::SetEleGap(G4bool elegap) {

   eleGap = elegap ;

}
void TrGEMAnalysis::SetChargeGap(G4bool chargegap) {

   chargeGap = chargegap ;

}

void TrGEMAnalysis::SavePrimary(G4double primaryene,
				G4double primaryposx, G4double primaryposy, G4double primaryposz,
				G4double primarypx, G4double primarypy, G4double primarypz,
				G4double primarypdirx, G4double primarypdiry, G4double primarypdirz,
				G4double zabsorption, std::string volabsorption){
  
   primaryEne = primaryene;
   zAbsorption = zabsorption;
   volAbsorption = volabsorption;
   primaryPosX = primaryposx;
   primaryPosY = primaryposy;
   primaryPosZ = primaryposz;
   primaryPX = primarypx;
   primaryPY = primarypy;
   primaryPZ = primarypz;
   primaryPdirX = primarypdirx;
   primaryPdirY = primarypdiry;
   primaryPdirZ = primarypdirz;

}
 

void TrGEMAnalysis::SaveTrack(G4int gapIndex, G4int gapPart, G4int gapParent,  G4int gapIsPrimary, std::string genprocess, std::string genvolume, G4double genz, std::string volname,  G4double kinene,
  G4double gapPosX, G4double gapPosY, G4double gapPosZ, G4double gapPX, G4double gapPY, G4double gapPZ, G4double gapPdirX, G4double gapPdirY, G4double
  gapPdirZ ){
 
      TrackIndex.push_back(gapIndex) ;
      TrackPart.push_back(gapPart) ;
      TrackParent.push_back(gapParent) ;
      TrackIsPrimary.push_back(gapIsPrimary) ;
      TrackGenProcess.push_back(genprocess) ;
      TrackGenVolume.push_back(genvolume) ;
      TrackGenZ.push_back(genz) ;
      TrackVolume.push_back(volname) ;
      TrackEne.push_back(kinene) ;
      TrackPosX.push_back(gapPosX) ;
      TrackPosY.push_back(gapPosY) ;
      TrackPosZ.push_back(gapPosZ) ;
      TrackPX.push_back(gapPX) ;
      TrackPY.push_back(gapPY) ;
      TrackPZ.push_back(gapPZ) ;
      TrackPdirX.push_back(gapPdirX) ;
      TrackPdirY.push_back(gapPdirY) ;
      TrackPdirZ.push_back(gapPdirZ) ;


}

