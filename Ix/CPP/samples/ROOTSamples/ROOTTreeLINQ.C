// Author Vassil Vassilev & Lorenzo Moneta

#include "DataModel/EventData.h"

#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TStopwatch.h"

#include <vector>
using namespace std;

#include "cpplinq/linq.hpp"

// void selectMany () {
//    using namespace cpplinq;
//    int_range range1(0, 3);
//    auto range2 =
//       from(range1)
//       .select_many(
//                    [](int x)
//                    {
//                       return int_range(0, x+1);
//                    });
// }

void Zad1() {
   // Select x and y pos of events which have at least 4 particles,
   // which have at least N GeV.
   unsigned N = 200;
   TFile *myFile = TFile::Open("DataModel/eventdata_s99.root");
   TTreeReader tree("tree", myFile);
   TTreeReaderArray<Particle> particles(tree, "fParticles");
   // std::vector<EventData> ED;  -> EventData == std::vector<Particle> pt;
   using namespace cpplinq;
   typedef decltype(from(particles)) t_particleItr;
   std::vector<int> bogus = {1, 2, 4};
   //int bogus[] = {1, 2, 4};
   typedef decltype(bogus) t_bogus;
   auto goodParticles =
      from(tree)
      .select([&](Long64_t entry) { return from(particles); })
      .where([&](const t_particleItr &plist) {
            return plist.where([&](const Particle &p){return p.fVector.E()>N;}).count() >= 4;})
      // .select_many([&](const t_particleItr &plist) {
      //       return plist.where([&](Particle &p){return p.fVector.E()>N;});
      //    })
      ;
   // from(bogus)
   //    .select_many([&](int r) {return bogus;} )
   //    .count();
   //typedef decltype(goodParticles) t_particles;
      //.count()
      //.select_many([&](const t_bogus &b) {return int_range(0,10)/*from(b).to_vector()*/; })
   //auto a = from (goodParticles)
   //.select_many([&](const t_particles &b) {return bogus; })


   //printf("Outer loop: %lu\n", std::distance(goodParticles.begin(), goodParticles.end()));

   int i = 0;
   int totalVals = 0;
   for (auto val : goodParticles) {
      //printf("Event N: %d\n", i++);
      for (auto p : val) {
         ++totalVals;
         //printf("X:%f Y:%f\n", p.fVector.X(), p.fVector.Y());
      }
   }

   //printf("Printf total iters: %d\n", totalVals);

        

      // .select([&](Long64_t entry) 
      //       return from(particles)
      //       .where([&](const Particle& p){ return p.fVector.E() > N; })

      //       //SELECT count(*) FROM particleList WHERE particle_count > 4 GROUP BY particle_count;
      //       .select([](const Particle& p) { return p.fPosition;})
      //       //.where([](const Particle& p));
      //       //printf("Size grouped: %d\n", grouped.to_vector().size());
      //       //return grouped.count() > 4;
            
      //       //.select([&](const Particle&p) {return p;})
      //       //.where([&](const Particle& p){return p.fVector.E() > N;})
      //       //.where([] (const Particle& p))
      //       ;
      //    }
      //    )
      // .where([&](linq_driver<linq_select<cpplinq::linq_where<cpplinq::iter_cursor<TTreeReaderArray<Particle>::Iterator_t>& p)
      //        { return p.count() > 4; })
      //.select([])
      // .where([&](auto entry) {
      //       return from(particles)
      //       .where([&](const Particle& p){return p.fVector.E() > N;})
      //       .count() > 4;
      //    }).first()
      ;

   //    printf("Size : %lu\n", XYPos.to_vector().size());
   // for (auto val : XYPos) {
      //printf("Val: %lu\n", val.size());
      //}
   //printf("Size:%lu\n", XYPos.size());
   // auto h = new TH1F("ptSum", "Sum p_T of events; p_T [GeV]", 200, 0, 500);
   // for (auto pt : PosPtSum) {
   //    h->Fill(pt);
   //    printf("val: %f\n", pt);
   // }
   // h->Draw();

   delete myFile;
   // printf("Positive PT Sum=%f, and negative PT Sum=%f\n", PosPtSum, NegPtSum);
}

void Zad1NoLINQ() {
   // Select x and y pos of events which have at least 4 particles,
   // which have at least N GeV.
   unsigned N = 200;
   TFile *myFile = TFile::Open("DataModel/eventdata_s99.root");
   TTreeReader tree("tree", myFile);
   TTreeReaderArray<Particle> particles(tree, "fParticles");
   std::vector<Particle> goodParticles;
   while (tree.Next()) {
      int particleCnt = 0;
      for(auto p : particles) {
         if (p.fVector.E() > N)
            ++particleCnt;

         if (particleCnt == 4) {
            for(auto p : particles)
               goodParticles.push_back(p);
            break;
         }
      }

   }
   //printf("Outer loop: %lu\n", std::distance(goodParticles.begin(), goodParticles.end()));
   delete myFile;
}


void Zad2() {
   // Calculate the pt (momentum) distribution for all positive and negative
   // charged particles and compare the values.

   TFile *myFile = TFile::Open("DataModel/eventdata_s99.root");
   TTreeReader tree("tree", myFile);
   TTreeReaderArray<Particle> particles(tree, "fParticles");

   using namespace cpplinq;
   auto PosPtSum =
      from(tree).select([&](Long64_t entry) {
            tree.Next();
            return from(particles)
            .where([](const Particle& p) { return p.fCharge > 0; })
            .select([](const Particle& p) { return p.fVector.Pt(); })
            .sum();
         });
   ///printf("Size:%lu\n", PosPtSum.size());
   auto h = new TH1F("ptSum", "Sum p_T of events; p_T [GeV]", 200, 0, 500);
   for (auto pt : PosPtSum) {
      h->Fill(pt);
      printf("val: %f\n", pt);
   }
   h->Draw();


   // double NegPtSum =
   //    from(tree).select([&particles](Long64_t entry) {
   //          return from(particles).
   //          where([] (const Particle& p) {return p.fCharge > 0; }).
   //          select([](const Particle& p) {
   //                return p.fVector.Pt();
   //             }).sum();
   //       }).sum();
   delete myFile;
   // printf("Positive PT Sum=%f, and negative PT Sum=%f\n", PosPtSum, NegPtSum);
}

void Zad2NoLINQ() {
   // Calculate the pt (momentum) distribution for all positive and negative
   // charged particles and compare the values.

   TFile *myFile = TFile::Open("DataModel/eventdata_s99.root");
   TTreeReader tree("tree", myFile);
   TTreeReaderArray<Particle> particles(tree, "fParticles");
   double sumPos = 0;
   double sumNeg = 0;
   while (tree.Next()) {
      for (auto p : particles) {
         if (p.fCharge > 0)
            sumPos += p.fVector.Pt();
         else if (p.fCharge < 0)
            sumNeg += p.fVector.Pt();
      }
   }
   delete myFile;
}


void Time(void (*f)()) {
   TStopwatch s;
   s.Start();
   for(unsigned i = 0; i < 1000; ++i)
      f();

   s.Stop();
   s.Print();
}

void Zad3() {
   // Display all events which have at least 2 pions and plot the pt of the
   // leading electron.

   TFile *myFile = TFile::Open("DataModel/eventdata_s99.root");
   TTreeReader tree("tree", myFile);
   TTreeReaderArray<Particle> particles(tree, "fParticles");

   using namespace cpplinq;

   auto result =
      from(from(tree).where([&particles](Long64_t entry) {
            bool hasAtLeast2 =
              from(particles).
                where([](const Particle& p) { return p.fType == Pion; }).
                  count() > 2;

            auto electrons =
              from(from(particles).
                 where([](const Particle& p) { return p.fType == Electron; })).
            select([](const Particle& p){return p.fVector.Pt();});

            // Missing implementation of default_if_empty
            if (hasAtLeast2 && !electrons.empty())
               return electrons.max();
            return 0.;
            })).where([](double pt){return pt != 0;}).count();
   delete myFile;
   // auto h = new TH1F("pt", "pt of leading e; p_T [GeV]", 50, 0, 250);
   // for (double pt : result) {
   //    h->Fill(pt);
   // }
   // h->Draw();
   //printf("Events with more than 2 pions: %lu\n", result);
}

void Zad3NoLINQ() {
   // Display all events which have at least 2 pions and plot the pt of the
   // leading electron.

   TFile *myFile = TFile::Open("DataModel/eventdata_s99.root");
   TTreeReader tree("tree", myFile);
   TTreeReaderArray<Particle> particles(tree, "fParticles");

   std::vector<double> leadingEPts;
   while (tree.Next()) {
      unsigned PionCnt = 0;
      for (auto& p: particles) {
         if (p.fType == Pion)
            ++PionCnt;
         if (PionCnt > 2) {
            double leadingEPt = 0;
            for (auto& p: particles) {
               if (p.fType == Electron && p.fVector.Pt() > leadingEPt)
                  leadingEPt = p.fVector.Pt();
            }
            leadingEPts.push_back(leadingEPt);
            break;
         }
      }
   }

   delete myFile;
   //printf("Events with more than 2 pions: %lu\n", leadingEPts.size());

}

void ROOTTreeLINQ() {
   EventData e; // Workaround usual ROOT bugs.
   printf("Executing and timing Zad1...\n");
   Time(&Zad1);
   printf("Executing and timing Zad1NoLINQ...\n");
   Time(&Zad1NoLINQ);

   printf("Executing and timing Zad2...\n");
   Time(&Zad2);
   printf("Executing and timing Zad2NoLINQ...\n");
   Time(&Zad2NoLINQ);

   printf("Executing and timing Zad3...\n");
   Time(&Zad3);
   printf("Executing and timing Zad3NoLINQ...\n");
   Time(&Zad3NoLINQ);
}
