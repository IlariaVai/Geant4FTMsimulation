//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file radioactivedecay/rdecay01/src/ParallelPrimaryGeneratorActionScan.cc
/// \brief Implementation of the ParallelPrimaryGeneratorActionScan class
//
//
// $Id$
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "ParallelPrimaryGeneratorActionScan.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include <fstream>

 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ParallelPrimaryGeneratorActionScan::ParallelPrimaryGeneratorActionScan(
      const G4String& particleName,
      G4double energy,
      G4ThreeVector position,
      G4ThreeVector momentumDirection) :
   G4VUserPrimaryGeneratorAction(),
   fParticleGun(0)
{
  G4int n_particle = 1;
//  fParticleGun  = new G4ParticleGun(n_particle);
 G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
G4ParticleDefinition* particlen = particleTable -> FindParticle("gamma");
//G4ParticleDefinition* particlen = particleTable -> FindParticle("e-");
// G4ParticleDefinition* particlen = particleTable -> FindParticle("mu-");

 fParticleGun  = new G4ParticleGun(particlen,n_particle);
 //fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-300.));
  
// set energy
 
  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticleMomentumDirection((G4ThreeVector(0.,0.,1.))); 
 // fParticleGun->SetParticleMomentumDirection((G4ThreeVector(0.,0.,-1.))); 
  
           
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ParallelPrimaryGeneratorActionScan::~ParallelPrimaryGeneratorActionScan()
{
  delete fParticleGun;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ParallelPrimaryGeneratorActionScan::GeneratePrimaries(G4Event* anEvent)
{

  // set direction
  // neutron
  // these are the cosines for an isotropic direction
  //  fParticleGun->SetParticleMomentumDirection((G4ThreeVector(0.,0.,1.))); 
 
 //G4double gX = G4UniformRand()*200.-100.;
 //  G4double gY = G4UniformRand()*200.-100.;
 //fParticleGun->SetParticlePosition(G4ThreeVector(gX,gY,-1.));
 //fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-1.));
 fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-1.));
 //fParticleGun->SetParticlePosition(G4ThreeVector(0,0,19.));



  //create vertex
  //   

  fParticleGun->GeneratePrimaryVertex(anEvent);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
