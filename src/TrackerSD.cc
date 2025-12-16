#include "TrackerSD.hh"

#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::TrackerSD(const G4String& name, const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add these collections in hce
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TrackerSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();

  if (edep == 0.) return false;

  auto newHit = new TrackerHit();

  newHit->SetTrackID(step->GetTrack()->GetTrackID());
  newHit->SetEdep(edep);
  newHit->SetPos(step->GetPostStepPoint()->GetPosition());

  fHitsCollection->insert(newHit);

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel > 1) {
    auto nofHits = fHitsCollection->entries();
    G4cout << G4endl << "-------->Hits Collection: in this event they are " << nofHits
           << " hits in the tracker chambers: " << G4endl;
    for (std::size_t i = 0; i < nofHits; ++i)
      (*fHitsCollection)[i]->Print();
  }
}

