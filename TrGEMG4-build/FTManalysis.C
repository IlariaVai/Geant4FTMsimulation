#define FTManalysis_cxx
#include "FTManalysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
 
int absFR4_1; //# gamma absorbed in FR4_1
int absCu;    //# gamma absorbed in Cu
int absFR4_2; //# gamma absorbed in FR4_2
double FabsFR4_1;  //% gamma absorbed in FR4_1
double FabsCu;	   //% gamma absorbed in Cu
double FabsFR4_2;  //% gamma absorbed in FR4_2

TH1F *hEneCu;
TH1F *hEneFakeTop;

void FTManalysis::Loop()
{
//   Define Histograms

hEneCu= new TH1F("hEneCu"," ;energy of primary e- generated in the Cu layer (keV); ",2000,0,20);
hEneFakeTop= new TH1F("hEneFakeTop"," ;energy of primary e- generated in the Cu layer and reaching FakeTop (keV); ",100,0,100);


//   In a ROOT session, you can do:
//      Root > .L FTManalysis.C
//      Root > FTManalysis t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
      
      absFR4_1=0;
      absCu=0;   
      absFR4_2=0;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      if (*volAbsorption=="FR4_1") absFR4_1++;
      if (*volAbsorption=="Cu")    absCu++;
      if (*volAbsorption=="FR4_2") absFR4_2++;
      
      //Loop on the  steps
      for(int h=0; h<TrackPart->size(); h++){
         
	 //look for primary electrons steps
        if ( (*TrackPart)[h] == 11 && (*TrackIsPrimary)[h] == 1) { 
	   
	   //register energy of primary electrons produced in Cu 
	   if ( (*TrackVolume)[h] == "Cu" && (*TrackGenVolume)[h] == "CuLog") 	   hEneCu->Fill((*TrackEne)[h]*1e3 );
	   
	   //register energy of primary electrons produced in Cu after they reach FakeTop (i.e. after they have passed FR4_2)
	   if ( (*TrackVolume)[h] == "FakeTop" && (*TrackGenVolume)[h] == "CuLog") hEneFakeTop->Fill((*TrackEne)[h]*1e3 );
	 
	 } 
      }
      
   }
   
   FabsFR4_1=double(absFR4_1)/double(nentries);
   FabsCu   =double(absCu)   /double(nentries);
   FabsFR4_2=double(absFR4_2)/double(nentries);
   cout<<"Gammas absorbed in FR4_1: "<<FabsFR4_1*100<<" %"<<endl;
   cout<<"Gammas absorbed in Cu: "   <<FabsCu*100<<" %"<<endl;
   cout<<"Gammas absorbed in FR4_2: "<<FabsFR4_2*100<<" %"<<endl;
   
   //Save Histograms
   TFile *fHistos;
   fHistos = new TFile("FTManalysis.root","RECREATE");
   hEneCu->Write();
   hEneFakeTop->Write();
   
   gApplication->Terminate();
}
