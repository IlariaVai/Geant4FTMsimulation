#ifndef GasDetectorConstruction_hh
#define GasDetectorConstruction_hh

class G4LogicalVolume ;
class G4PhysicalVolume ;


// Standard includes
#include <list>
#include <string>
#include <cmath>

// Geant4 includes
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4ProductionCuts.hh"
#include "G4Region.hh"
#include "G4Trd.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"

#include "G4UniformElectricField.hh"
#include "G4EqMagElectricField.hh"
#include "G4FieldManager.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"

class GasDetectorConstruction : public G4VUserDetectorConstruction
{
   public :
      // Constructor
      GasDetectorConstruction() ;
      // Destructor
      ~GasDetectorConstruction() ;

      void DefineMaterials() ;
      G4VPhysicalVolume* Construct() ;
      G4Trd* Trapezoid(G4String name, G4double width) ;
      void PlaceGeometry(G4RotationMatrix *pRot, G4ThreeVector tlate, G4LogicalVolume *pMotherLogical) ; 

   private :
      G4Material*        fFR4Mat;
      G4Material*        fGasMat;
      G4Material*        fEmptyMat;
      G4Material*        fAirMat;
      G4Material*        fCuMat;
      G4Material*        fKAPTONMat;
      G4Material*        fWaterMat;
      G4Material*	 fAlMat;
      G4Material*	 fMylarMat;
      G4Material*	 fHPLMat;
      //G4ProductionCuts*  fGasDetectorCuts;
      G4double           tripleGemThinBase; 
      G4double           tripleGemLargeBase;
      G4double           tripleGemHeight;
     
      std::vector<G4Box*>           trdCollection ;
      std::vector<G4LogicalVolume*> trdLogCollection ;
   
     G4ElectricField*        fEMfield1;
     G4EqMagElectricField*   fEquation1;
     G4MagIntegratorStepper* fStepper1;
     G4FieldManager*         fFieldMgr1;
     G4double                fMinStep ;
     G4ChordFinder*          fChordFinder1 ;
     G4MagInt_Driver*        fIntgrDriver1;
     
      

};

#endif

