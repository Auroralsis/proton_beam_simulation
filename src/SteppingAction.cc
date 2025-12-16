#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Detect prompt gammas: first step of a gamma created in the Target with very small local time
  auto track = step->GetTrack();
  if (track->GetDefinition()->GetParticleName() == "gamma") {
    if (track->GetCurrentStepNumber() == 1 && track->GetParentID() != 0) {
      auto creVol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
      if (creVol && std::string(creVol->GetName()) == "Target") {
        // extract kinematics
        G4double energy = track->GetKineticEnergy();
        G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
        // get current event id
        auto evt = G4EventManager::GetEventManager()->GetConstCurrentEvent();
        G4int eventID = evt ? evt->GetEventID() : -1;

        EventAction::PromptGamma pg;
        pg.eventID = eventID;
        pg.energy = energy;
        pg.position = pos;
        if (fEventAction) fEventAction->AddPromptGamma(pg);
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
