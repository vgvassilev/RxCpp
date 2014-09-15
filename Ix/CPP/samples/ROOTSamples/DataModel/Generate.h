// Authod Lorenzo Moneta
// Adapted by Vassil Vassilev

#include "EventData.h"
#include "TRandom.h"
#include "TGenPhaseSpace.h"
#include "Math/Vector4D.h"
#include "TLorentzVector.h"

// generate the events

// parameters

int NTRACKS = 40;
// parameter for pt distribution
double PtAvg = 20;
int NTYPES = 8;
double masses[] = { 0, 0.0005, 0.105, 0.135, 0.139, 0.4937, 0.4976, 3.096 };
int charge [] = { 0, 1, 1, 0, 1, 1, 0, 0 };
double fractions[] = { 0.1, 0.12, 0.08, 0.05, 0.5, 0.1, 0.049 , 0.01 };
double sigmax = 10.E-6;
double sigmay = 10.E-6;
double sigmaz = 5.;

using namespace ROOT::Math;

ROOT::Math::PtEtaPhiMVector SmearVector(const ROOT::Math::XYZTVector & v) {
   double x = v.X()*(1. + gRandom->Gaus(0, 0.1) );
   double y = v.Y()*(1. + gRandom->Gaus(0, 0.1) );
   double z = v.Z()*(1. + gRandom->Gaus(0, 0.1) );
   ROOT::Math::PxPyPzMVector tmp(x,y,z,v.M() );
   return PtEtaPhiMVector(tmp);
}


void EventData::Generate()  {
   // get expected value for each type
   for (int i = 0; i < NTYPES; ++i) {
      double nexp = fractions[i] * NTRACKS;

      int np = gRandom->Poisson(nexp);
      for (int j = 0; j < np; ++j) {
         Particle p;
         p.fPosition = XYZVector( gRandom->Gaus(0,sigmax), gRandom->Gaus(0, sigmay), gRandom->Gaus(0, sigmaz) );
         double pt = gRandom->Exp(PtAvg);
         double eta = gRandom->Uniform(-3,3);
         double phi = gRandom->Uniform(-TMath::Pi(), TMath::Pi() );
         double mass = masses[i];
         p.fVector = PtEtaPhiMVector(pt, eta, phi, mass);
         p.fType = i;
         p.fCharge = charge[i];
         if (p.fCharge) {
            int tmp = gRandom->Integer(2);
            if (tmp == 0) p.fCharge = -1;
         }
         // special case for decays
         if (i == 3 ) {
            // pi0
            TGenPhaseSpace evt;
            double m[2] = {0,0};
            TLorentzVector W( p.fVector.X(), p.fVector.Y(), p.fVector.Z(), p.fVector.E() );
            evt.SetDecay(W, 2, m);
            evt.Generate();
            TLorentzVector * v1 = evt.GetDecay(0);
            TLorentzVector * v2 = evt.GetDecay(1);
            Particle p1;
            Particle p2;
            p1.fPosition = p.fPosition;
            p2.fPosition = p.fPosition;
            p1.fCharge = 0;
            p2.fCharge = 0;
            p1.fType = 0;
            p2.fType = 0;

            p1.fVector = SmearVector(XYZTVector(v1->X(), v1->Y(), v1->Z(), v1->E() ));
            p2.fVector = SmearVector(XYZTVector(v2->X(), v2->Y(), v2->Z(), v2->E() ));
            AddParticle(p1);
            AddParticle(p2);
         }
         if (i == 6) {
            // Ks
            TGenPhaseSpace evt;
            double m[2] = {0.139,0.139};
            TLorentzVector W( p.fVector.X(), p.fVector.Y(), p.fVector.Z(), p.fVector.E() );
            evt.SetDecay(W, 2, m);
            evt.Generate();
            TLorentzVector * v1 = evt.GetDecay(0);
            TLorentzVector * v2 = evt.GetDecay(1);
            double ctau = 2.6844 * p.fVector.Gamma();
            double disp = gRandom->Exp(ctau);
            double dispX = disp * p.fVector.X()/p.fVector.P();
            double dispY = disp * p.fVector.Y()/p.fVector.P();
            double dispZ = disp * p.fVector.Z()/p.fVector.P();
            Particle p1;
            Particle p2;
            p1.fPosition = XYZVector( p.fPosition.X() + dispX,  p.fPosition.Y() + dispY,  p.fPosition.Z() + dispZ);
            p2.fPosition = p1.fPosition;
            p1.fCharge = 1;
            p2.fCharge = -1;
            p1.fType = 4;
            p2.fType = 4;

            p1.fVector = SmearVector(XYZTVector(v1->X(), v1->Y(), v1->Z(), v1->E() ));
            p2.fVector = SmearVector(XYZTVector(v2->X(), v2->Y(), v2->Z(), v2->E() ));
            AddParticle(p1);
            AddParticle(p2);
         }
         if (i == 7 ) {
            // J/psi
            TGenPhaseSpace evt;
            double m1[2] = {0.0005,0.0005};
            double m2[2] = {0.105,0.105};
            int tmp = gRandom->Integer(2);
            TLorentzVector W( p.fVector.X(), p.fVector.Y(), p.fVector.Z(), p.fVector.E() );
            if (tmp == 0)
               evt.SetDecay(W, 2, m1);
            else
               evt.SetDecay(W, 2, m2);

            evt.Generate();

            TLorentzVector * v1 = evt.GetDecay(0);
            TLorentzVector * v2 = evt.GetDecay(1);
            Particle p1;
            Particle p2;
            p1.fPosition = p.fPosition;
            p2.fPosition = p.fPosition;
            p1.fCharge = 1;
            p2.fCharge = -1;
            p1.fType = 1;
            p2.fType = 1;
            if (tmp == 1) {
               p1.fType = 2;
               p2.fType = 2;
            }

            p1.fVector = SmearVector(XYZTVector(v1->X(), v1->Y(), v1->Z(), v1->E() ));
            p2.fVector = SmearVector(XYZTVector(v2->X(), v2->Y(), v2->Z(), v2->E() ));
            AddParticle(p1);
            AddParticle(p2);
         }
         else
            AddParticle(p);
      }
   }
}
