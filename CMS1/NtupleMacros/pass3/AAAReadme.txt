Instructions to use the Pass3 Macros
====================================

Preliminaries
-------------
- Make a symlink "data" to the directory that contains the 
  rootuples to be analyzed
- Create a subdirectory "out".  This is where output files will go.

To run pass3
------------
unix> root -l
root> .x doPass3.C
root> .q

(If this does not work, see "Idiosyncracies" item at the end 
of this file)

This makes a bunch of histograms that are saved in out/myHist.root

To look at the pass3 histograms, make stack plots, etc
------------------------------------------------------
unix> root -l
root> .x setup.C
root> loadHist("out/myHist.root")

Now you have the histograms loaded into root.  There are
several histograms, named as "XX_blah_YY".  
"XX" refers to the sample, eg, XX=tt or WW for ttbar or WW;
"blah" is a title, eg, blah=hnJet for number of jets
"YY" = "ee" "em" "mm" "all"  is the dilepton type.

You can browse all these histograms as stacked plots with

root> browseStacks()

browseStacks takes two parameters:
   // Make the stacks and then browse them interactively
   // if (makePictures==true), saves all plots to out/stacks.ps
   // Also: make out_stacks_xx.png where xx is the page number
   // on the stack.ps file
   void browseStacks( bool makePictures=false, bool wait=true )

How to add histograms
----------------------
Add booking and filling code to myLoopingFunction.C
Read the comments and look at what is already been done

How to change cuts
------------------
Change myLoopingFunction.C.  
Note that many of the selection functions are collected in 
selections.C.  That is a good place to put a new selection
that you have just dreamed up.

Idiosyncracies
--------------
The ntuples use vectors of lorentz vectors, and this can be 
painful to make it work.  Basically, it works on some 
environments and does not work on others.  

It is more likely to work on a 32 bit machine than a 64 bit machine.

It does not seem to work using a 1_5_x CMSSW environment at FNAL, 
but it works with 1_6_0.

You can also try to comment out the line 
gROOT->LoadMacro("loader.C+");
in doPass3.C and see if that fixes your problem.




