#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4StepLimiterPhysics.hh"

// this physicsList is used for radioactive decay source, thus it should not be used during proton beam simulation

myPhysicsList::myPhysicsList()
  : G4VModularPhysicsList()
{
  // Register basic physics
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4EmStandardPhysics());

  // Radioactive decay (for ions)
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // Optional: step limiter
  RegisterPhysics(new G4StepLimiterPhysics());
}
