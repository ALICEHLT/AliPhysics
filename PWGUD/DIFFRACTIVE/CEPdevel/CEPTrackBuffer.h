#ifndef CEPTRACKBUFFER
#define CEPTRACKBUFFER

#include <TVector3.h>
#include <AliPID.h>

class CEPTrackBuffer : public TObject {

  private:
    // general information
    UInt_t   fTrackStatus;     // see AliCEPBase.h for definition of bits
    Double_t fTOFBunchCrossing;// TOF Bunch Crossing time
    Int_t    fChargeSign;      // charge sign
    Int_t    fITSncls;         // number of ITS clusters
    Int_t    fTPCncls;         // number of TPC clusters
    Int_t    fTRDncls;         // number of TRD clusters
    Int_t    fTPCnclsS;        // number of shared TPC clusters
    Double_t fZv;              // closest approach to vertex in z
    TVector3 fMomentum;        // momentum vector
   
    // PID information
    Float_t fPID;
    
    // ... from TPC
    Float_t fPIDTPCStatus;
    Float_t fPIDTPCSignal;
    Float_t fPIDTPCnSigma[AliPID::kSPECIES];
    Float_t fPIDTPCnSigmaProb[AliPID::kSPECIES];
    
    // ... from TOF
    Float_t fPIDTOFStatus;
    Float_t fPIDTOFSignal;
    Float_t fPIDTOFnSigma[AliPID::kSPECIES];
    Float_t fPIDTOFnSigmaProb[AliPID::kSPECIES];
    
    // ... Bayes
    Float_t fPIDBayesStatus;
    Float_t fPIDBayesProb[AliPID::kSPECIES];
    
    // MC truth
    Int_t    fMCPID;
    Float_t  fMCMass;
    TVector3 fMCMomentum;
      
  public:
    static const Int_t kdumval = -999;
    
    CEPTrackBuffer();
    ~CEPTrackBuffer();
    
    // Modifiers
    void Reset();
    
    void SetTrackStatus(UInt_t TTest) { fTrackStatus = TTest; }
    void SetTrackStatus(UInt_t TTest, Bool_t yn)
      { fTrackStatus = (fTrackStatus & ~TTest) | (yn*TTest); }
    void SetTOFBunchCrossing(Double_t tofbc) { fTOFBunchCrossing = tofbc; }

    void SetChargeSign(Int_t chs)    { fChargeSign = chs; }
    void SetITSncls(Int_t ncls)      { fITSncls = ncls; }
    void SetTPCncls(Int_t ncls)      { fTPCncls = ncls; }
    void SetTRDncls(Int_t ncls)      { fTRDncls = ncls; }
    void SetTPCnclsS(Int_t nclss)    { fTPCnclsS = nclss; }
    void SetZv(Int_t zv)             { fZv = zv; }
    void SetMomentum(TVector3 mom)   { fMomentum = mom; }
    
    void SetPID(Float_t pid)            { fPID = pid; }
    void SetPIDTPCStatus(Float_t stat)  { fPIDTPCStatus = stat; }
    void SetPIDTPCSignal(Float_t sig)   { fPIDTPCSignal = sig; }
    void SetPIDTPCnSigma(Int_t part, Float_t nsig);
    void SetPIDTPCProbability(Int_t part, Float_t prob);
    void SetPIDTOFStatus(Float_t stat)  { fPIDTOFStatus = stat; }
    void SetPIDTOFSignal(Float_t sig)   { fPIDTOFSignal = sig; }
    void SetPIDTOFnSigma(Int_t part, Float_t nsig);
    void SetPIDTOFProbability(Int_t part, Float_t prob);
    void SetPIDBayesStatus(Float_t stat)  { fPIDBayesStatus = stat; }
    void SetPIDBayesProbability(Int_t part, Float_t prob);
    
    void SetMCPID(Int_t pid)            { fMCPID = pid; } 
    void SetMCMass(Float_t mass)        { fMCMass = mass; }
    void SetMCMomentum(TVector3 mom)    { fMCMomentum = mom; }
    
    // Accessors
    UInt_t GetTrackStatus()       const { return fTrackStatus; }
    Bool_t TTisSet(UInt_t TTest)  const { return (fTrackStatus & TTest) == TTest; }
    Double_t GetTOFBunchCrossing()const { return fTOFBunchCrossing; }

    Int_t GetChargeSign()  const { return fChargeSign; }
    Int_t GetITSncls()     const { return fITSncls; }
    Int_t GetTPCncls()     const { return fTPCncls; }
    Int_t GetTRDncls()     const { return fTRDncls; }
    Int_t GetTPCnclsS()    const { return fTPCnclsS; }
    Double_t GetZv()       const { return fZv; }
    TVector3 GetMomentum() const { return fMomentum; }

    Float_t GetPID()            { return  fPID; }
    Float_t GetPIDTPCStatus()   { return  fPIDTPCStatus; }
    Float_t GetPIDTPCSignal()   { return  fPIDTPCSignal; }
    Float_t GetPIDTPCnSigma(Int_t part);
    Float_t GetPIDTPCProbability(Int_t part);
    Float_t GetPIDTOFStatus()   { return  fPIDTOFStatus; }
    Float_t GetPIDTOFSignal()   { return  fPIDTOFSignal; }
    Float_t GetPIDTOFnSigma(Int_t part);
    Float_t GetPIDTOFProbability(Int_t part);
    Float_t GetPIDBayesStatus() { return  fPIDBayesStatus; }
    Float_t GetPIDBayesProbability(Int_t part);

    Int_t GetMCPID()            const { return fMCPID; }
    Float_t GetMCMass()         const { return fMCMass; }
    TVector3 GetMCMomentum()    const { return fMCMomentum; }
    
  ClassDef(CEPTrackBuffer,1)     // CEP track buffer

};

#endif
