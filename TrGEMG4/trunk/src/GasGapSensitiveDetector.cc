#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4HCofThisEvent.hh"
#include "G4HCtable.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "TrGEMAnalysis.hh"
#include "GasGapSensitiveDetector.hh"
#include "GasGapHit.hh"

   GasGapSensitiveDetector::GasGapSensitiveDetector(G4String SDname)
: G4VSensitiveDetector(SDname),
   primaryAcceptance(false),
   elegap(false),
   chargegap(false),
   primaryene(0.), 
   primaryPosX(0.),  primaryPosY(0.),  primaryPosZ(0.),
   primaryPX(0.),    primaryPY(0.),    primaryPZ(0.),
   primaryPdirX(0.), primaryPdirY(0.), primaryPdirZ(0.),
   zAbsorption(-5),
   volAbsorption("none"),
   contaPrimary(0)

{
   G4cout << "*************************************" << G4endl ;
   G4cout << "*** Creating SD with name: "<< SDname << " ***" << G4endl ;  
   G4cout << "*************************************" << G4endl ;

   // Declaration of the hit collection name
   G4String myCollectionName = "GasGapHitCollection" ;
   collectionName.insert( myCollectionName) ;
    
   
   for(G4int t=0;t<3;t++){
     eDep[t]=0.;
  }
}


GasGapSensitiveDetector::~GasGapSensitiveDetector()
{}

G4bool GasGapSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
   //This method is called every time a G4Step is performed in the logical volume
   //to which this SD is attached: the GAS GAP.

   //To identify where the step is we use the touchable navigation,
   //Remember we need to use PreStepPoint!
      
   G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
   G4int copyNo = touchable->GetVolume(0)->GetCopyNo();
   G4int layerIndex = copyNo;
   G4String volName = touchable->GetVolume(0)->GetName();

   //We get now the energy deposited by this step and other interesting info
  
   G4double edep = step->GetTotalEnergyDeposit() ;
   G4Track* track = step->GetTrack();

   G4double energy= step->GetPreStepPoint()->GetKineticEnergy();
   double x= track->GetPosition().getX();
   double y= track->GetPosition().getY();
   double z= track->GetPosition().getZ();
   
   double px= track->GetMomentum().getX();
   double py= track->GetMomentum().getY();
   double pz= track->GetMomentum().getZ();
 
   double pdirx= track->GetMomentumDirection().getX();
   double pdiry= track->GetMomentumDirection().getY();
   double pdirz= track->GetMomentumDirection().getZ();

   G4int parent= track->GetParentID(); 
   G4int isPrimaryDaughter=0;
   if (parent==1) {isPrimaryDaughter=1;} //particles generated directly from primary particle
   else if (parent>1) {isPrimaryDaughter=0;}  
   else {isPrimaryDaughter=-1;}  //primary particle
   
   
   G4int pdg = track->GetParticleDefinition()->GetPDGEncoding();
   G4StepPoint* point = step->GetPostStepPoint();
   const G4VProcess* proc = point->GetProcessDefinedStep();
   const G4String procname = proc->GetProcessName();
   G4int trackIndex=track->GetTrackID();
   double genz= track->GetVertexPosition().getZ(); 
    
   G4String genprocess;
   if(track->GetCreatorProcess()!=0) {
     const G4VProcess * genproc=track->GetCreatorProcess();  
     genprocess = genproc->GetProcessName();
   }
   else  {genprocess="primary";}
   const G4LogicalVolume *  genLogVolume=track->GetLogicalVolumeAtVertex();
   G4String genvolume= genLogVolume->GetName();
   
   
      if( trackIndex==1 && volName == "FakeBottom"  && contaPrimary==0 ) {  
       	 primaryene=energy;
	 primaryAcceptance=true;
	 primaryPosX=x;
	 primaryPosY=y;
	 primaryPosZ=z;
	 primaryPX=px;
	 primaryPY=py;
	 primaryPZ=pz;
	 primaryPdirX=pdirx;
	 primaryPdirY=pdiry;
	 primaryPdirZ=pdirz;
 	 contaPrimary=1;
  }
   
    if( trackIndex==1 && track->GetTrackStatus()!=fAlive){
      zAbsorption=z;
      const G4VPhysicalVolume *absVolume=track->GetVolume();
      std::string absvolume= absVolume->GetName();
      volAbsorption=absvolume;
    }
 
    // energy deposited in FR4_1
    if(volName == "FR4_1") {
	   eDep[0] += edep ;
    }
    if(volName == "Cu") {
	   eDep[1] += edep ;
    }
    if(volName == "FR4_2") {
	   eDep[2] += edep ;
    }
 
   if  (volName == "FakeTop") {   //electrons or charged particles surviving both the FR4 layers
       if(pdg==11){
	   elegap=true;
       }
	if(track->GetParticleDefinition()->GetPDGCharge()!=0){
	   chargegap=true;
       }
    }
   
    
 // SAVE TRACKS IN VOLUMES
 //_________________________________________________________________________
 //identify new particles creatend inside one of the interesting volumes
   G4int newparticle=0;
   if ((volName=="FakeBottom" || volName=="FR4_1" ||volName=="Cu" || volName=="FR4_2" || volName=="FakeTop") && track->GetCurrentStepNumber()==1) newparticle=1;
    
    //register particle entering an interesting volume for the first time and new particles
    if ( step->GetPreStepPoint()->GetStepStatus()==fGeomBoundary || newparticle )     {  
      TrGEMAnalysis::GetInstance()->SaveTrack(trackIndex, pdg, parent, isPrimaryDaughter, genprocess, genvolume,  genz, volName, energy, 
					    x, y, z, px, py, py, pdirx, pdiry, pdirz );
	//G4cout<<trackIndex <<" trackIndex pdg "<<pdg<<" genprocess "<<genprocess<<" parent " <<parent<<" volName "<<volName<<G4endl;
	//G4cout<<" x "<<x<<" y "<<y<<" z "<<z<<" dirx "<<pdirx<<" pdiry "<<pdiry<<" pdirz "<<pdirz<<G4endl;
 
   }  
 
//----------------------------------------------------------------------------------------------------------------------
      
 //This line is used to store in Analysis class the energy deposited in this layer
   //The Analysis class will sum up this edep to the current event total energy in this layer
   //Pass the number directly to the Analysis manager. No Hits objects are created in 
   //this case
   //TrGEMAnalysis::GetInstance()->AddEDepHad(layerIndex,edep);

   // Tricks to implement hits 
   hitMap_t::iterator it = hitMap.find(layerIndex) ;
   GasGapHit* aHit = 0 ;
   if( it != hitMap.end() ) {
      aHit = it->second ;
   }
   else {
      aHit = new GasGapHit(layerIndex) ;
      hitMap.insert( std::make_pair(layerIndex, aHit) ) ;
      hitCollection->insert(aHit) ;
   }
   aHit->AddEdep(edep) ;

   return true;
   }

   void GasGapSensitiveDetector::Initialize(G4HCofThisEvent* HCE)
   {
      hitCollection = new GasGapHitCollection(GetName(), collectionName[0]) ;
      static G4int HCID = -1 ;
      if(HCID<0) HCID = GetCollectionID(0) ; // <<-- this is to get an ID for collectionName[0]
      HCE->AddHitsCollection(HCID, hitCollection) ;

      // Reset map of hits
      hitMap.clear() ;
   }

   void GasGapSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
   {
      TrGEMAnalysis::GetInstance()->SetEdep( eDep ) ;
	
      TrGEMAnalysis::GetInstance()->SavePrimary(primaryene,
	 					primaryPosX,primaryPosY, primaryPosZ, 
	 					primaryPX,primaryPY, primaryPZ, 
	 					primaryPdirX,primaryPdirY, primaryPdirZ, 
						zAbsorption, volAbsorption) ;
 
      TrGEMAnalysis::GetInstance()->SetPrimaryAcceptance(primaryAcceptance) ;
      TrGEMAnalysis::GetInstance()->SetEleGap(elegap) ;
      TrGEMAnalysis::GetInstance()->SetChargeGap(chargegap) ;
      
      for(G4int t=0;t<3;t++){
 	eDep[t] = 0. ;
       }
      primaryAcceptance = false ;
      contaPrimary=0;
      elegap=false;
      chargegap=false;
      primaryene=0.;
      primaryPosX=0.,	primaryPosY=0.,	 primaryPosZ=0.; 
      primaryPX=0.,	primaryPY=0., 	 primaryPZ=0.; 
      primaryPdirX=0.,	primaryPdirY=0., primaryPdirZ=0.; 
      zAbsorption=-5;
      volAbsorption="none";

 
   }

