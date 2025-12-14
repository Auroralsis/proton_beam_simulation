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
//
/// \file B4/B4c/src/DetectorConstruction.cc
/// \brief Implementation of the B4c::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "TrackerSD.hh"

#include "G4AutoDelete.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4PhysicalConstants.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

namespace B4c
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_AIR");
  G4Element* La = nistManager->FindOrBuildElement("La");
  G4Element* Br = nistManager->FindOrBuildElement("Br");

  G4double density;
  G4int nComponents;
  G4Material* LaBr3 =  new G4Material("LaBr3", density = 5.1 * g / cm3, nComponents = 2);
  LaBr3->AddElement(La, 1);
  LaBr3->AddElement(Br, 3);

  // PMMA (Polymethyl methacrylate) : chemical formula C5H8O2, density ~1.19 g/cm3
  G4Element* C = nistManager->FindOrBuildElement("C");
  G4Element* H = nistManager->FindOrBuildElement("H");
  G4Element* O = nistManager->FindOrBuildElement("O");
  G4Material* PMMA = new G4Material("PMMA", density = 1.19 * g / cm3, nComponents = 3);
  PMMA->AddElement(C, 5);
  PMMA->AddElement(H, 8);
  PMMA->AddElement(O, 2);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Geometry parameters, here is the actual size of them without dividing 2
  G4double scatThickness = 5. * mm;
  G4double absoThickness = 5. * mm;
  G4double scatSizeXY = 50. * mm;
  G4double absoSizeXY = 50. * mm;
  G4double targetSizeX = 40. * mm;
  G4double targetSizeY = 40. * mm;
  G4double targetSizeZ = 40. * mm;

  auto worldSizeXY = 2 * scatSizeXY;
  auto worldSizeZ = 250. * mm;

  G4double absoPosiZ = -40. * mm;
  G4double targetPosiZ = 50. * mm;

  // Get materials
  auto AirMaterial = G4Material::GetMaterial("G4_AIR");
  auto PMMA = G4Material::GetMaterial("PMMA");

  // one of them is composed of GAGG actually,
  // but presume the material is LaBr3 firstly
  auto scatMaterial = G4Material::GetMaterial("LaBr3");
  auto absoMaterial = G4Material::GetMaterial("LaBr3");

  if (!AirMaterial || !scatMaterial || !absoMaterial) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorConstruction::DefineVolumes()", "MyCode0001", FatalException, msg);
  }

  //
  // World
  //
  auto worldS = new G4Box("World",  // its name
                          worldSizeXY / 2, worldSizeXY / 2, worldSizeZ / 2);  // its size

  auto worldLV = new G4LogicalVolume(worldS,  // its solid
                                     AirMaterial,  // its material
                                     "World");  // its name

  auto worldPV = new G4PVPlacement(nullptr,  // no rotation
                                   G4ThreeVector(),  // at (0,0,0)
                                   worldLV,  // its logical volume
                                   "World",  // its name
                                   nullptr,  // its mother  volume
                                   false,  // no boolean operation
                                   0,  // copy number
                                   fCheckOverlaps);  // checking overlaps
                              
  auto targetS = new G4Box("Target", 
                           targetSizeX / 2, targetSizeY / 2, targetSizeZ / 2);

  auto targetLV = new G4LogicalVolume(targetS, 
                                      PMMA, 
                                      "Target");

  auto targetPV = new G4PVPlacement(nullptr, 
                                    G4ThreeVector(0., 0., targetPosiZ), 
                                    targetLV, 
                                    "Target", 
                                    worldLV, 
                                    false, 
                                    0, 
                                    fCheckOverlaps);

  //
  // Absorber
  //
  auto absorberS = new G4Box("Abso",  // its name
                             absoSizeXY / 2, absoSizeXY / 2, absoThickness / 2);  // its size

  auto absorberLV = new G4LogicalVolume(absorberS,  // its solid
                                        absoMaterial,  // its material
                                        "AbsoLV");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(0., 0., absoPosiZ),  // its position
                    absorberLV,  // its logical volume
                    "Abso",  // its name
                    worldLV,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    fCheckOverlaps);  // checking overlaps

  //
  // Scatter
  //
  auto scatterS = new G4Box("Scat",  // its name
                        scatSizeXY / 2, scatSizeXY / 2, scatThickness / 2);  // its size

  auto scatterLV = new G4LogicalVolume(scatterS,  // its solid
                                   scatMaterial,  // its material
                                   "ScatLV");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(0., 0., 0.),  // its position
                    scatterLV,  // its logical volume
                    "Scat",  // its name
                    worldLV,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    fCheckOverlaps);  // checking overlaps

  //
  // print parameters
  //
  // G4cout << G4endl << "------------------------------------------------------------" << G4endl
  //        << "---> The calorimeter is " << fNofLayers << " layers of: [ " << absoThickness / mm
  //        << "mm of " << absorberMaterial->GetName() << " + " << gapThickness / mm << "mm of "
  //        << gapMaterial->GetName() << " ] " << G4endl
  //        << "------------------------------------------------------------" << G4endl;

  //
  // Visualization attributes
  //
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  //
  // Always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  //
  // Sensitive detectors
  //
  auto absoSD = new TrackerSD("AbsorberSD", "AbsorberHitsCollection");
  auto scatSD = new TrackerSD("ScatterSD", "ScatterHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(absoSD);
  G4SDManager::GetSDMpointer()->AddNewDetector(scatSD);
  SetSensitiveDetector("AbsoLV", absoSD);
  SetSensitiveDetector("ScatLV", scatSD);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B4c
