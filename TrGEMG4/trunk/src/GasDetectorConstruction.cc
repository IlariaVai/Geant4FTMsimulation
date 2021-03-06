#include "GasDetectorConstruction.hh"
#include "GasGapSensitiveDetector.hh"

#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4GeometryManager.hh"
//#include "G4PhysicalVolumeStore.hh"
//#include "G4LogicalVolumeStore.hh"
//#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4UnitsTable.hh"
#include "G4EqMagElectricField.hh"

#include "G4UniformElectricField.hh"
#include "G4FieldManager.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ClassicalRK4.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4EqMagElectricField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"

GasDetectorConstruction::GasDetectorConstruction() :
   fFR4Mat(0), fGasMat(0), fEmptyMat(0), fAirMat(0),
   fCuMat(0),fKAPTONMat(0),fWaterMat(0),fAlMat(0),fMylarMat(0),fHPLMat(0),
   //fGasDetectorCuts(0), 
   tripleGemThinBase(0), tripleGemLargeBase(0), tripleGemHeight(0)
    
{
   tripleGemThinBase  = 271.0*mm ;
   tripleGemLargeBase = 453.0*mm ;
   tripleGemHeight    = 1007.0*mm ;

}

GasDetectorConstruction::~GasDetectorConstruction() {


}

void GasDetectorConstruction::DefineMaterials() {

   G4NistManager* manager = G4NistManager::Instance() ;
   // define Elements
   G4Element* elC  = manager->FindOrBuildElement(6);
   G4Element* elF  = manager->FindOrBuildElement(9);
   G4Element* elSi = manager->FindOrBuildElement(14);
   G4Element* elO  = manager->FindOrBuildElement(8);
   G4Element* elH  = manager->FindOrBuildElement(1);
   G4Element* elS  = manager->FindOrBuildElement(16);

   // define Materials
   G4Material *Cu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu") ;
   G4Material *Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al") ;
   G4Material *KAPTON = G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
   G4Material* Mylar = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");
   fCuMat = Cu;
   fKAPTONMat = KAPTON;
   fAlMat = Al;
   fMylarMat= Mylar;
   
   G4int numel(0), natoms(0) ;
   G4double density(0.), temperature(0.), pressure(0.), fractionMass(0.)  ;
   G4String name, symbol ;
   
   G4Material* SiO2 =  new G4Material("quartz",density= 2.200*g/cm3, numel=2);
   SiO2->AddElement(elSi, natoms=1);
   SiO2->AddElement(elO , natoms=2);

   //from http://www.physi.uni-heidelberg.de/~adler/TRD/TRDunterlagen/RadiatonLength/tgc2.htm
  //Epoxy (for FR4 )
   density = 1.2*g/cm3;
   G4Material* Epoxy = new G4Material("Epoxy" , density, numel=2);
   Epoxy->AddElement(elH, natoms=2);
   Epoxy->AddElement(elC, natoms=2);
  
   //FR4 (Glass + Epoxy)
   density = 1.86*g/cm3; 
   G4Material* FR4 = new G4Material("FR4"  , density, numel=2);
   FR4->AddMaterial(Epoxy, fractionMass=0.472);
   FR4->AddMaterial(SiO2, fractionMass=0.528);
   fFR4Mat = FR4;

   //High Pressure Laminate HPL   
   G4Material* HPL = new G4Material(name = "HPL", density= 1.4*g/cm3, numel=3);
   HPL->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_H"), fractionMass = 5.74*perCent);
   HPL->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_C"), fractionMass = 77.46*perCent);
   HPL->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_O"), fractionMass = 16.8*perCent);
   fHPLMat = HPL ;

   //Air   
   G4Material* Air = new G4Material(name = "Air", density= 1.29*mg/cm3, numel=2);
    Air->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_N"), fractionMass = 70.*perCent);
    Air->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_O"), fractionMass = 30.*perCent);
   fAirMat = Air ;

   // gases at STP conditions 
   G4Material* Argon = manager->FindOrBuildMaterial("G4_Ar");
   G4Material* CarbonDioxide = manager->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
   G4Material* empty = manager->FindOrBuildMaterial("G4_Galactic");
   fEmptyMat = empty ;

   // CF4 must be defined by hand
   //G4int numel(0) ;
   //G4double density(0.), temperature(0.), pressure(0.) ;
   //G4String name, symbol ;
   G4Material* CF4 = new G4Material(name="CF4", density=0.003884*g/cm3, numel=2, kStateGas, temperature = 273.15*kelvin, pressure=1.0*atmosphere);
   CF4->AddElement(elC, 1) ;
   CF4->AddElement(elF, 4) ; 

   // Ar:CO2 (70:30) @ STP conditions
   G4double mixtureDensity = (Argon->GetDensity() * 70/100.0 + CarbonDioxide->GetDensity() * 30/100.0) ;
   G4Material *ArCO2 = new G4Material("Ar/CO2",mixtureDensity,2) ;
   ArCO2->AddMaterial(Argon, 0.7) ;
   ArCO2->AddMaterial(CarbonDioxide, 0.3) ;

   // Ar:CO2:CF4 (45:15:40) @ STP conditions
   mixtureDensity = (Argon->GetDensity() * 45/100.0 + CarbonDioxide->GetDensity() * 15/100.0 + CF4->GetDensity() * 40/100.0) ;
   G4Material *ArCO2CF4 = new G4Material("Ar/CO2/CF4",mixtureDensity,3) ;
   ArCO2CF4->AddMaterial(Argon, 0.45) ;
   ArCO2CF4->AddMaterial(CarbonDioxide,0.15) ;
   ArCO2CF4->AddMaterial(CF4,0.40) ;



  // RPC mixture gas components
   // iso-Butane (methylpropane), STP
   density = 2.67*mg/cm3;
   G4Material* isobutane = new G4Material(name = "isoC4H10", density, numel=2) ;
   isobutane->AddElement(elC,4);
   isobutane->AddElement(elH,10);

   // Freon
   density = 4.55*mg/cm3;
   G4Material* C2H2F4 = new G4Material(name = "Freon", density, numel=3) ;
   C2H2F4->AddElement(elC, natoms=2);
   C2H2F4->AddElement(elH, natoms=2);
   C2H2F4->AddElement(elF, natoms=4);
 
   // esafluoruro di zolfo SF6
   density = 6.14*mg/cm3;
   G4Material* SF6 = new G4Material(name = "SF6", density, numel=2) ;
   SF6->AddElement(elS, natoms=1);
   SF6->AddElement(elF, natoms=6);


    density = 3.569*mg/cm3;
   G4Material* RPCgas = new G4Material(name = "RPCGas", density, numel=3) ;
   RPCgas->AddMaterial(isobutane, fractionMass = 4.5* perCent) ;
   RPCgas->AddMaterial(C2H2F4, fractionMass = 95.2* perCent) ;
   RPCgas->AddMaterial(SF6, fractionMass = 0.3* perCent) ;

   // Choice of the gas
   //  fGasMat = ArCO2 ;
  //   fGasMat = fAirMat ;
   fGasMat = ArCO2CF4 ;
  
  density = 1.000*g/cm3;
  G4Material* H2O = new G4Material(name="Water",density,2);
  H2O->AddElement(elH, natoms=2);
  H2O->AddElement(elO, natoms=1);
  fWaterMat=H2O;
}

G4VPhysicalVolume* GasDetectorConstruction::Construct() {

   // Cleanup old geometry
   G4GeometryManager::GetInstance()->OpenGeometry();


   // Define all materials and set global variables
   DefineMaterials() ;

   // SD Manager 
   G4SDManager* sdman = G4SDManager::GetSDMpointer() ;

   G4double worldSizeX = 1.*m;
   G4double worldSizeY = 1.*m;
   G4double worldSizeZ = 1.*m;

   // Rotation Matrix for layers
   G4RotationMatrix* rotationPlacement = new G4RotationMatrix() ;
   rotationPlacement->rotateY(M_PI / 2.0) ;
   rotationPlacement->rotateX(M_PI / 2.0) ;

   // Visual attributes
   G4VisAttributes *cathodeAttributes = new G4VisAttributes(G4Color::Grey()) ;
   cathodeAttributes->SetForceWireframe(true) ;
   G4VisAttributes *g10Attributes = new G4VisAttributes(G4Color::White()) ;
   g10Attributes->SetForceWireframe(true) ;
   G4VisAttributes *gasAttributes = new G4VisAttributes(G4Color::Red()) ;
   gasAttributes->SetForceWireframe(true) ;
   G4VisAttributes *gemAttributes = new G4VisAttributes(G4Color::Green()) ;
   gemAttributes->SetForceWireframe(true) ;
   G4VisAttributes *insAttributes = new G4VisAttributes(G4Color::Blue()) ;
  insAttributes->SetForceWireframe(true) ;


   // World definition and placement
   G4Box* worldBox = new G4Box("WorldBox", worldSizeX, worldSizeY, worldSizeZ) ;
   G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, fEmptyMat, "WorldLog") ;
   // Set visual attributes
   G4VisAttributes *worldAttributes = new G4VisAttributes ;
   worldAttributes->SetVisibility(false) ;
   worldLog->SetVisAttributes(worldAttributes) ;

   G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldSpace", 0, false, 0) ;

// 
// //____________________________________________________________________________________________________________
 	        
		
	//CONTINOUS MATERIALS
		std::string NomeStrati[5]= 
		{
		"FakeBottom", "FR4_1", "Cu", "FR4_2", "FakeTop"					       	
		};
             
	        std::string NomeStratiLog[5];
		
		for(size_t A=0; A<5; A++) { 
		    NomeStratiLog[A]=NomeStrati[A]+"Log";
		}
		

	        G4Material* MatStrati[5] = 
	        {
	        fAirMat, fFR4Mat, fCuMat, fFR4Mat,fAirMat
	        };
		
		G4double mythickness=500 * um ;
		
		G4double spessoreStrati[5]=
		{
		0.01*mm,mythickness,0.035*mm,0.2*mm,0.01*mm	
		};
		
		GasGapSensitiveDetector* sensitive = new GasGapSensitiveDetector("/GasGap") ;
		sdman->AddNewDetector(sensitive) ;
		
		G4Box* strato; 
		G4LogicalVolume* logicStrato;
		double boxWidth=10*cm;
		for(G4int lyr=0;lyr<5;lyr++){
			
        strato= new G4Box(NomeStrati[lyr] , spessoreStrati[lyr]/2, boxWidth/2 , boxWidth/2) ;
 	logicStrato = new G4LogicalVolume(strato,MatStrati[lyr] ,NomeStratiLog[lyr] ) ;
  	logicStrato->SetVisAttributes(new G4VisAttributes(*insAttributes)) ;
  	trdCollection.push_back(strato) ;
  	trdLogCollection.push_back(logicStrato) ;
  	logicStrato->SetSensitiveDetector(sensitive) ;
}
     G4bool allLocal = true;	
     trdLogCollection[1]->SetFieldManager(fFieldMgr1,allLocal);

 PlaceGeometry(rotationPlacement,G4ThreeVector(0.,0.,0.),worldLog) ;

   return worldPhys ;


}

G4Trd* GasDetectorConstruction::Trapezoid(G4String name, G4double width) {
   G4Trd* shape = new G4Trd(name,
	 width/2, width/2,
	 tripleGemThinBase/2,
	 tripleGemLargeBase/2,
	 tripleGemHeight/2) ;
   return shape ;
}

void GasDetectorConstruction::PlaceGeometry(G4RotationMatrix *pRot, G4ThreeVector tlate, G4LogicalVolume* pMotherLogical) {
   
   G4double XTranslation = 0 ;
   G4int cpN=0;
   G4ThreeVector position;   
  

    for(size_t i=0 ; i<trdCollection.size() ; i++) {
      // i counts as the copyNo
      XTranslation += trdCollection.at(i)->GetXHalfLength() ;
      position = tlate + G4ThreeVector(XTranslation,0,0).transform(G4RotationMatrix(*pRot).inverse()) ;
      G4cout << "Volume (" << cpN << ") " << trdCollection.at(i)->GetName() << " the position is " << G4BestUnit(XTranslation,"Length") << G4endl ;
      new G4PVPlacement(pRot,
	    position,
	    trdLogCollection.at(i),
	    trdCollection.at(i)->GetName(),
	    pMotherLogical,
	    false,
	    cpN) ;
      XTranslation += trdCollection.at(i)->GetXHalfLength() ;
      cpN++;
   }
   
 
}

