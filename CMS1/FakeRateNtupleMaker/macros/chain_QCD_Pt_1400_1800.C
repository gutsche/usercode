{
	TChain *chain = new TChain("event");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_10.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_14.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_15.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_16.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_17.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_18.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_21.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_22.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_23.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_24.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_25.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_28.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_2.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_32.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_33.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_35.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_3.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_42.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_43.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_44.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_4.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_5.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_7.root");
	chain->Add("dcache:/pnfs/cms/WAX/resilient/gutsche/condor/071104_01_16/ntuple_8.root");
	cout << "Chain contains: " << chain->GetEntries() << " events." << endl;
}
