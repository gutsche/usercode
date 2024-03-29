//--------------------------------------------------------
// Our histogram are named XX_YY_ZZ
// where XX=tt, WW, WZ, etc
//       YY refers to what is actually plotted
//       ZZ=em, ee, mm, all
//
// It is useful to get a list of all the YY's
//
// We can get this list by looking at all the existing hostograms.
//
// This list is returned here as TObjArray*
//
// The passed prefix should be, eg, "tt" and the passed postfix
// should be, eg, "ee", so that the list is built only
// from the "tt_YY_ee" histograms
//
// Claudio 4 Sep 2007
//--------------------------------------------------------

TObjArray* getMyHistosNames( char* prefix, char* postfix) {


  TObjArray* array = new TObjArray(); 

  // Get a list of object and their iterator 
  TList* list = gDirectory->GetList() ;
  TIterator* iter = list->MakeIterator();

  // Loop over objects
  TObject* obj;
  while(obj=iter->Next()) {

    // Only look at objects beginning with 'prefix' and ending with 'postfix'
    TString name = obj->GetName();
    if (name.BeginsWith(prefix) && name.EndsWith(postfix)) {

      // Only look at objects that inherit from TH1 or TH2
      if (obj->IsA()->InheritsFrom(TH1::Class()) ||
          obj->IsA()->InheritsFrom(TH2::Class())) {

	// Find the central key, ie, the YY
	TObjArray* t = name.Tokenize("_");
	TString z = TString(t->At(1)->GetName());
	// cout << t->At(1)->GetName() << endl;
	
	// add to the output array
	TObjString* zobj = new TObjString(z);
	array->Add(zobj);

      }
    }
  }

  return array;
}

