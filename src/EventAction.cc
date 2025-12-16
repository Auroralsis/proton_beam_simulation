#include "EventAction.hh"

#include "TrackerHit.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"

#include <iomanip>
#include <algorithm>

EventAction::EventAction(RunAction* runAction) : fRunAction(runAction) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerHitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
  auto hitsCollection = static_cast<TrackerHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));

  if (!hitsCollection) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("EventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::PrintEventStatistics(G4double absoEdep, G4double absoTrackLength,
                                       G4double gapEdep, G4double gapTrackLength) const
{
  // print event statistics
  G4cout << "   Absorber: total energy: " << std::setw(7) << G4BestUnit(absoEdep, "Energy")
         << "       total track length: " << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
         << G4endl << "        Gap: total energy: " << std::setw(7) << G4BestUnit(gapEdep, "Energy")
         << "       total track length: " << std::setw(7) << G4BestUnit(gapTrackLength, "Length")
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* /*event*/) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Get hits collections IDs (only once)
  if (fScatHCID == -1) {
    fScatHCID = G4SDManager::GetSDMpointer()->GetCollectionID("ScatterHitsCollection");
    fAbsoHCID = G4SDManager::GetSDMpointer()->GetCollectionID("AbsorberHitsCollection");
  }

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  auto NtupleID = fRunAction->GetPromptNtupleID();

	// Write prompt gammas recorded in this event into PromptGamma ntuple
	if (!fPromptGammas.empty()) {
			for (const auto &g : fPromptGammas) {
        analysisManager->FillH1(2, g.energy);
				analysisManager->FillNtupleIColumn(NtupleID, 0, g.eventID);
				analysisManager->FillNtupleDColumn(NtupleID, 1, g.energy);
				analysisManager->FillNtupleDColumn(NtupleID, 2, g.position.x());
				analysisManager->FillNtupleDColumn(NtupleID, 3, g.position.y());
				analysisManager->FillNtupleDColumn(NtupleID, 4, g.position.z());
        analysisManager->AddNtupleRow(NtupleID);
			}
	}
	fPromptGammas.clear();

  // Get hits collections
  auto scatHC = GetHitsCollection(fScatHCID, event);
  auto absoHC = GetHitsCollection(fAbsoHCID, event);

  auto eventID = event->GetEventID();

  G4int nScat = static_cast<G4int>(scatHC->entries());
  G4int nAbso = static_cast<G4int>(absoHC->entries());

  if (nScat == 0 && nAbso == 0) return;

  G4double scatEdep = 0.;
  G4ThreeVector scatPosi(0., 0., 0.);
  G4double absoEdep = 0.;
  G4ThreeVector absoPosi(0., 0., 0.);

  for (int i = 0; i < nScat; i++) {
    scatEdep += (*scatHC)[i]->GetEdep();
    scatPosi += (*scatHC)[i]->GetPos();
  }

  for (int i = 0; i < nAbso; i++) {
    absoEdep += (*absoHC)[i]->GetEdep();
    absoPosi += (*absoHC)[i]->GetPos();
  }

  if (nScat != 0) {
    analysisManager->FillH1(0, scatEdep);
  }
  if (nAbso != 0) {
    analysisManager->FillH1(1, absoEdep);
  }

  // record data only when both scatter and absorber detect event simultaneously
  if (nScat == 0 || nAbso == 0) return;
  
  absoPosi /= nAbso;
  scatPosi /= nScat;

  NtupleID = fRunAction->GetDetectionNtupleID();

  analysisManager->FillNtupleIColumn(NtupleID, 0, eventID);
  analysisManager->FillNtupleDColumn(NtupleID, 1, scatPosi[0]);
  analysisManager->FillNtupleDColumn(NtupleID, 2, scatPosi[1]);
  analysisManager->FillNtupleDColumn(NtupleID, 3, scatPosi[2]);

  analysisManager->FillNtupleDColumn(NtupleID, 4, absoPosi[0]);
  analysisManager->FillNtupleDColumn(NtupleID, 5, absoPosi[1]);
  analysisManager->FillNtupleDColumn(NtupleID, 6, absoPosi[2]);

  analysisManager->FillNtupleDColumn(NtupleID, 7, scatEdep);
  analysisManager->FillNtupleDColumn(NtupleID, 8, absoEdep);

  analysisManager->AddNtupleRow(NtupleID);

}
