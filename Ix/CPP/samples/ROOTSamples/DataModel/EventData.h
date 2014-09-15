#ifndef EventData_h
#define EventData_h


#include <vector>
#include "Math/Vector4D.h"
#include "Math/Vector3D.h"


enum ParticleType {
   Photon = 0,
   Electron,
   Unknown,
   Pion
};

class Particle {
public:
   Particle() { //memset(fTags, 0, sizeof(fTags));
 }

   ROOT::Math::XYZVector  fPosition;   // vertex position
   ROOT::Math::PtEtaPhiMVector fVector;     // particle vector
   int  fCharge;        // particle charge
   int fType;           // particle type (0=pho, 1 ele, 2...)
};

class TF1;

class EventData {
public:

   std::vector<Particle> fParticles; // particles of the event
   int fEventSize; // size (in bytes) of the event

   void SetSize() {
      fEventSize = sizeof(EventData) + fParticles.size() * sizeof(Particle);
   }
   void Clear() {
      fParticles.clear();
   }

   void AddParticle(const Particle& p) { fParticles.push_back(p); }

   void Generate();

   //ClassDef(EventData,1); // Data for an event
};


#ifdef __MAKECINT__
#pragma link C++ class Particle+;
#pragma link C++ class EventData+;
#pragma link C++ class std::vector<Particle>+;
#endif

#endif
