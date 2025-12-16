// ROOT macro file for plotting example B4 histograms
//
// Can be run from ROOT session:
// root[0] .x plotHisto.C

{
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  // Draw histos filled by Geant4 simulation
  //

  // Open file filled by Geant4 simulation
  TFile f("simulation.root");

  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(2,2);

  // Draw Eabs histogram in the pad 1
  c1->cd(1);
  TH1D* hist1 = (TH1D*)f.Get("Escat");
  hist1->Draw("HIST");

  // Draw Labs histogram in the pad 2
  c1->cd(2);
  TH1D* hist2 = (TH1D*)f.Get("Eabso");
  hist2->Draw("HIST");

  // Draw Egap histogram in the pad 3

  TH1D* hist3 = (TH1D*)f.Get("Energy");
  c1->cd(3);
  // set logarithmic scale for y  
  //gPad->SetLogy(1);
  hist3->Draw("HIST");

  // set logarithmic scale for y  
  //gPad->SetLogy(1);  
}
