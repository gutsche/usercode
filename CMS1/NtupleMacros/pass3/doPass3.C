{
// Load and compile something to allow proper treatment of vectors
// Not clear that it is needed
gROOT->LoadMacro("loader.C+");

// Load various tools
gROOT->ProcessLine(".x setup.C");

// Load and compile selection functions
gROOT->ProcessLine(".L selections.C+");

// Load and compile the looping code
gROOT->ProcessLine(".L myLoopingFunction.C+");

 //tt file:
TFile *ftt = TFile::Open("data/ntuplemaker_TTbar.root");
TTree *ttop  = (TTree*)(gROOT->FindObject("event"));           

//WW file
TFile *fww = TFile::Open("data/ntuplemaker_WW.root");
TTree *tww  = (TTree*)(gROOT->FindObject("event"));

//Drell Yan file
TFile *fDY = TFile::Open("data/ntuplemaker_DrellYan-ll_40.root");
TTree *tDY  = (TTree*)(gROOT->FindObject("event"));

//WZ file
TFile *fWZ = TFile::Open("data/ntuplemaker_WZ.root");
TTree *tWZ  = (TTree*)(gROOT->FindObject("event"));


// Define colors numbers:
gStyle->SetPalette(1);
enum EColor { kWhite, kBlack, kRed, kGreen, kBlue, kYellow, kMagenta, kCyan };

// Process files one at a time, and color them as needed
cout << "Processing ttbar.."<<endl;
ScanTree(ttop,"tt");
hist::color("tt", kYellow);

cout << "Processing WW.."<<endl;
ScanTree(tww,"ww");
hist::color("ww", kRed);

cout << "Processing WZ.."<<endl;
ScanTree(tww,"wz");
hist::color("wz", kBlue);

cout << "Processing DY->tautau" << endl;
ScanTree(tDY,"DYtt",2);
hist::color("DYtt", kBlack);

cout << "Processing DY->ee" << endl;
ScanTree(tDY,"DYee",0);
hist::color("DYee", kMagenta);

cout << "Processing DY->mm" << endl;
ScanTree(tDY,"DYmm",1);
hist::color("DYmm", kCyan);


//save all the histograms
saveHist("out/myHist.root");

}







