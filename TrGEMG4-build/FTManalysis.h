//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug 24 13:17:33 2017 by ROOT version 5.34/07
// from TTree MyRun/Run with awesome data
// found on file: task.root
//////////////////////////////////////////////////////////

#ifndef FTManalysis_h
#define FTManalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <string>
#include <vector>
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class FTManalysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        Edep[3];
   Bool_t          primaryAcceptance;
   Bool_t          eleGap;
   Bool_t          chargeGap;
   Double_t        primaryEne;
   Double_t        primaryPosX;
   Double_t        primaryPosY;
   Double_t        primaryPosZ;
   Double_t        primaryPX;
   Double_t        primaryPY;
   Double_t        primaryPZ;
   Double_t        primaryPdirX;
   Double_t        primaryPdirY;
   Double_t        primaryPdirZ;
   Double_t        zAbsorption;
   string          *volAbsorption;
   vector<int>     *TrackIndex;
   vector<int>     *TrackPart;
   vector<int>     *TrackParent;
   vector<int>     *TrackIsPrimary;
   vector<string>  *TrackGenProcess;
   vector<string>  *TrackGenVolume;
   vector<double>  *TrackGenZ;
   vector<string>  *TrackVolume;
   vector<double>  *TrackEne;
   vector<double>  *TrackPosX;
   vector<double>  *TrackPosY;
   vector<double>  *TrackPosZ;
   vector<double>  *TrackPX;
   vector<double>  *TrackPY;
   vector<double>  *TrackPZ;
   vector<double>  *TrackPdirX;
   vector<double>  *TrackPdirY;
   vector<double>  *TrackPdirZ;

   // List of branches
   TBranch        *b_Edep;   //!
   TBranch        *b_primaryAcceptance;   //!
   TBranch        *b_eleGap;   //!
   TBranch        *b_chargeGap;   //!
   TBranch        *b_primaryEne;   //!
   TBranch        *b_primaryPosX;   //!
   TBranch        *b_primaryPosY;   //!
   TBranch        *b_primaryPosZ;   //!
   TBranch        *b_primaryPX;   //!
   TBranch        *b_primaryPY;   //!
   TBranch        *b_primaryPZ;   //!
   TBranch        *b_primaryPdirX;   //!
   TBranch        *b_primaryPdirY;   //!
   TBranch        *b_primaryPdirZ;   //!
   TBranch        *b_zAbsorption;   //!
   TBranch        *b_volAbsorption;   //!
   TBranch        *b_TrackIndex;   //!
   TBranch        *b_TrackPart;   //!
   TBranch        *b_TrackParent;   //!
   TBranch        *b_TrackIsPrimary;   //!
   TBranch        *b_TrackGenProcess;   //!
   TBranch        *b_TrackGenVolume;   //!
   TBranch        *b_TrackGenZ;   //!
   TBranch        *b_TrackVolume;   //!
   TBranch        *b_TrackEne;   //!
   TBranch        *b_TrackPosX;   //!
   TBranch        *b_TrackPosY;   //!
   TBranch        *b_TrackPosZ;   //!
   TBranch        *b_TrackPX;   //!
   TBranch        *b_TrackPY;   //!
   TBranch        *b_TrackPZ;   //!
   TBranch        *b_TrackPdirX;   //!
   TBranch        *b_TrackPdirY;   //!
   TBranch        *b_TrackPdirZ;   //!

   FTManalysis(TTree *tree=0);
   virtual ~FTManalysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef FTManalysis_cxx
FTManalysis::FTManalysis(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("task.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("task.root");
      }
      f->GetObject("MyRun",tree);

   }
   Init(tree);
}

FTManalysis::~FTManalysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t FTManalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t FTManalysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void FTManalysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   volAbsorption = 0;
   TrackIndex = 0;
   TrackPart = 0;
   TrackParent = 0;
   TrackIsPrimary = 0;
   TrackGenProcess = 0;
   TrackGenVolume = 0;
   TrackGenZ = 0;
   TrackVolume = 0;
   TrackEne = 0;
   TrackPosX = 0;
   TrackPosY = 0;
   TrackPosZ = 0;
   TrackPX = 0;
   TrackPY = 0;
   TrackPZ = 0;
   TrackPdirX = 0;
   TrackPdirY = 0;
   TrackPdirZ = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Edep", Edep, &b_Edep);
   fChain->SetBranchAddress("primaryAcceptance", &primaryAcceptance, &b_primaryAcceptance);
   fChain->SetBranchAddress("eleGap", &eleGap, &b_eleGap);
   fChain->SetBranchAddress("chargeGap", &chargeGap, &b_chargeGap);
   fChain->SetBranchAddress("primaryEne", &primaryEne, &b_primaryEne);
   fChain->SetBranchAddress("primaryPosX", &primaryPosX, &b_primaryPosX);
   fChain->SetBranchAddress("primaryPosY", &primaryPosY, &b_primaryPosY);
   fChain->SetBranchAddress("primaryPosZ", &primaryPosZ, &b_primaryPosZ);
   fChain->SetBranchAddress("primaryPX", &primaryPX, &b_primaryPX);
   fChain->SetBranchAddress("primaryPY", &primaryPY, &b_primaryPY);
   fChain->SetBranchAddress("primaryPZ", &primaryPZ, &b_primaryPZ);
   fChain->SetBranchAddress("primaryPdirX", &primaryPdirX, &b_primaryPdirX);
   fChain->SetBranchAddress("primaryPdirY", &primaryPdirY, &b_primaryPdirY);
   fChain->SetBranchAddress("primaryPdirZ", &primaryPdirZ, &b_primaryPdirZ);
   fChain->SetBranchAddress("zAbsorption", &zAbsorption, &b_zAbsorption);
   fChain->SetBranchAddress("volAbsorption", &volAbsorption, &b_volAbsorption);
   fChain->SetBranchAddress("TrackIndex", &TrackIndex, &b_TrackIndex);
   fChain->SetBranchAddress("TrackPart", &TrackPart, &b_TrackPart);
   fChain->SetBranchAddress("TrackParent", &TrackParent, &b_TrackParent);
   fChain->SetBranchAddress("TrackIsPrimary", &TrackIsPrimary, &b_TrackIsPrimary);
   fChain->SetBranchAddress("TrackGenProcess", &TrackGenProcess, &b_TrackGenProcess);
   fChain->SetBranchAddress("TrackGenVolume", &TrackGenVolume, &b_TrackGenVolume);
   fChain->SetBranchAddress("TrackGenZ", &TrackGenZ, &b_TrackGenZ);
   fChain->SetBranchAddress("TrackVolume", &TrackVolume, &b_TrackVolume);
   fChain->SetBranchAddress("TrackEne", &TrackEne, &b_TrackEne);
   fChain->SetBranchAddress("TrackPosX", &TrackPosX, &b_TrackPosX);
   fChain->SetBranchAddress("TrackPosY", &TrackPosY, &b_TrackPosY);
   fChain->SetBranchAddress("TrackPosZ", &TrackPosZ, &b_TrackPosZ);
   fChain->SetBranchAddress("TrackPX", &TrackPX, &b_TrackPX);
   fChain->SetBranchAddress("TrackPY", &TrackPY, &b_TrackPY);
   fChain->SetBranchAddress("TrackPZ", &TrackPZ, &b_TrackPZ);
   fChain->SetBranchAddress("TrackPdirX", &TrackPdirX, &b_TrackPdirX);
   fChain->SetBranchAddress("TrackPdirY", &TrackPdirY, &b_TrackPdirY);
   fChain->SetBranchAddress("TrackPdirZ", &TrackPdirZ, &b_TrackPdirZ);
   Notify();
}

Bool_t FTManalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void FTManalysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t FTManalysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef FTManalysis_cxx
