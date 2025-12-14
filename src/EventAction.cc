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
/// \file B4/B4c/src/EventAction.cc
/// \brief Implementation of the B4c::EventAction class

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

namespace B4c
{

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

  // Get hits collections
  auto scatHC = GetHitsCollection(fScatHCID, event);
  auto absoHC = GetHitsCollection(fAbsoHCID, event);

  auto eventID = event->GetEventID();

  // Fill histograms, ntuple
  //

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

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

  analysisManager->FillNtupleDColumn(0, eventID);
  analysisManager->FillNtupleDColumn(1, scatPosi[0]);
  analysisManager->FillNtupleDColumn(2, scatPosi[1]);
  analysisManager->FillNtupleDColumn(3, scatPosi[2]);

  analysisManager->FillNtupleDColumn(4, absoPosi[0]);
  analysisManager->FillNtupleDColumn(5, absoPosi[1]);
  analysisManager->FillNtupleDColumn(6, absoPosi[2]);

  analysisManager->FillNtupleDColumn(7, scatEdep);
  analysisManager->FillNtupleDColumn(8, absoEdep);

  analysisManager->AddNtupleRow();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B4c
