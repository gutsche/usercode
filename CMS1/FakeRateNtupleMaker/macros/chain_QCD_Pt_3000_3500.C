{
	TChain *chain = new TChain("event");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_10.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_11.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_12.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_13.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_14.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_17.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_18.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_19.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_1.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_20.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_21.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_23.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_2.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_3.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_4.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_5.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_6.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_7.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_8.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_20/ntuple_9.root");
	cout << "Chain contains: " << chain->GetEntries() << " events." << endl;
}
