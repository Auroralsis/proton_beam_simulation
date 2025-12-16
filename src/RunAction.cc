#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

RunAction::RunAction()
{
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Create analysis manager
  // The choice of the output format is done via the specified
  // file extension.
  auto analysisManager = G4AnalysisManager::Instance();

  // Create directories
  // analysisManager->SetHistoDirectoryName("histograms");
  // analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //

  // Creating histograms
  analysisManager->CreateH1("Escat", "Edep in scatter", 10000, 0., 2 * MeV);
  analysisManager->CreateH1("Eabso", "Edep in absorber", 10000, 0., 2 * MeV);
  analysisManager->CreateH1("Energy", "Energy of prompt gamma", 10000, 0., 10. * MeV);

  // Creating ntuple for detector record
  //
  fDetectionNtupleID = analysisManager->CreateNtuple("Detection", "Edep and position in scatter and absorber");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleDColumn("scatPosiX");
  analysisManager->CreateNtupleDColumn("scatPosiY");
  analysisManager->CreateNtupleDColumn("scatPosiZ");
  analysisManager->CreateNtupleDColumn("absoPosiX");
  analysisManager->CreateNtupleDColumn("absoPosiY");
  analysisManager->CreateNtupleDColumn("absoPosiZ");
  analysisManager->CreateNtupleDColumn("scatEdep");
  analysisManager->CreateNtupleDColumn("absoEdep");
  analysisManager->FinishNtuple();

  // Creating ntuple for prompt gamma record
  //

  fPromptNtupleID = analysisManager->CreateNtuple("Prompt gamma", "Energy and position of prompt gamma");
  analysisManager->CreateNtuple("Prompt gamma", "Energy and position of prompt gamma appearing");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleDColumn("Energy");
  analysisManager->CreateNtupleDColumn("PosiX");
  analysisManager->CreateNtupleDColumn("PosiY");
  analysisManager->CreateNtupleDColumn("PosiZ");
  analysisManager->FinishNtuple();
  
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // inform the runManager to save random number seed
  // G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "simulation.root";
  // Other supported output types:
  // G4String fileName = "B4.csv";
  // G4String fileName = "B4.hdf5";
  // G4String fileName = "B4.xml";
  analysisManager->OpenFile(fileName);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  // if (analysisManager->GetH1(1)) {
  //   G4cout << G4endl << " ----> print histograms statistic ";
  //   if (isMaster) {
  //     G4cout << "for the entire run " << G4endl << G4endl;
  //   }
  //   else {
  //     G4cout << "for the local thread " << G4endl << G4endl;
  //   }

  //   G4cout << " EAbs : mean = " << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy")
  //          << " rms = " << G4BestUnit(analysisManager->GetH1(0)->rms(), "Energy") << G4endl;

  //   G4cout << " EGap : mean = " << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy")
  //          << " rms = " << G4BestUnit(analysisManager->GetH1(1)->rms(), "Energy") << G4endl;

  //   G4cout << " LAbs : mean = " << G4BestUnit(analysisManager->GetH1(2)->mean(), "Length")
  //          << " rms = " << G4BestUnit(analysisManager->GetH1(2)->rms(), "Length") << G4endl;

  //   G4cout << " LGap : mean = " << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length")
  //          << " rms = " << G4BestUnit(analysisManager->GetH1(3)->rms(), "Length") << G4endl;
  // }

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}
