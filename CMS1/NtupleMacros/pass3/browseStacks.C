// Make the stacks and then browse them interactively
// if (makePictures==true), saves all plots to out/stacks.ps
// Also: make out_stacks_xx.png where xx is the page number 
// on the stack.ps file
void browseStacks( bool makePictures=false, bool wait=true ) {


// Find out what the names of the existing histograms are
// The histogram names are XX_YY_ZZ, where XX is the sample,
// eg, "tt", YY is the actual name, ZZ is the final state, eg, "ee"
  TObjArray* myNames = getMyHistosNames("tt","ee");


// Now loop over histograms, and make stacks
   TCanvas *c = new TCanvas();
   c->Divide(2,2);
   char* suffix[4];
   suffix[0] = "ee";
   suffix[1] = "mm";
   suffix[2] = "em";
   suffix[3] = "all";
   if (makePictures) c->Print("out/stacks.ps[");
   for (int i=0; i<myNames->GetEntries(); i++) {
     //  cout << myNames->At(i)->GetName() << endl;
     for (int sample=0; sample<4; sample++) {
       hist::stack(Form("st_%s_%s",myNames->At(i)->GetName(),suffix[sample]),
                    Form("%s_%s$",myNames->At(i)->GetName(), suffix[sample]));
        THStack* thisStack = (THStack*) gROOT->FindObjectAny(
                              Form("st_%s_%s", myNames->At(i)->GetName(), suffix[sample]));
	TLegend* thisLeg = hist::legend(thisStack, "lpf", 0, 0);
        c->cd(sample+1);
        thisStack->Draw("hist");
        thisLeg->Draw();
        c->Update();
     }
     if (makePictures) {
       c->Print("out/stacks.ps");
       c->Print(Form("out/stacks_%d.png",i+1));
     }
     if (wait) {
       cout << "Enter carriage return for the next set of plots....q to quit" << endl;
       char in = getchar();
       if (in == 'q') break;
     }
   }
   if (makePictures) c->Print("out/stacks.ps]");
}
