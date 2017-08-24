void runFTManalysis()
{
  gROOT->ProcessLine(".L FTManalysis.C");
  TFile *f = TFile::Open("task.root");
  TTree *t4 = (TTree*)f->Get("MyRun");
  FTManalysis *t = new FTManalysis(t4);
  t->Loop();
  f->Close();
}
