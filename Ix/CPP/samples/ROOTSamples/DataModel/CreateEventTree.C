// Authod Lorenzo Moneta
// Adapted by Vassil Vassilev

#include "TTree.h"
#include "TFile.h"
#include "TRandom.h"
#include "TMath.h"
#include <vector>

#include "EventData.h"

void CreateEventTree(Long64_t numEvents = 200, int splitlevel = 99) {
   TFile* f = new TFile("eventdata_s99.root", "RECREATE");
   TTree* tree = new TTree("tree", "Tutorial tree");

   EventData* event = new EventData();
   tree->Branch("E", &event,32000,splitlevel);

   Particle p;

   for (Long64_t i = 0; i < numEvents; ++i) {
      event->Clear();
      event->Generate();
      event->SetSize();

      tree->Fill();
   }

   tree->Print();
   tree->Write();
   delete f;
}
