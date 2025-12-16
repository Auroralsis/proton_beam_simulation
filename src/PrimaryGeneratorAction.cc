#include "PrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // Default: set the gun to emit a single proton of 150 MeV
  auto proton = G4ParticleTable::GetParticleTable()->FindParticle("proton");
  if (proton) {
    fParticleGun->SetParticleDefinition(proton);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    fParticleGun->SetParticleEnergy(150. * MeV);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore
  //
  G4double worldZHalfLength = 0.;
  auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

  // Check that the world volume has box shape
  G4Box* worldBox = nullptr;
  if (worldLV) {
    worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  }

  if (worldBox) {
    worldZHalfLength = worldBox->GetZHalfLength();
  }
  else {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "MyCode0002", JustWarning, msg);
  }

  // point source at world center (can be extended to sample a volume)
  G4ThreeVector srcPos(-40. * mm, 0., 50. * mm);

  // Generate a single proton at 150 MeV from srcPos along +z
  fParticleGun->SetParticlePosition(srcPos);
  // ensure particle definition and energy already set in constructor; override energy just in case
  fParticleGun->SetParticleEnergy(150. * MeV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  fParticleGun->GeneratePrimaryVertex(event);
}
