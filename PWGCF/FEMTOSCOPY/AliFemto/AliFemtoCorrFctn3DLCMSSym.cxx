///////////////////////////////////////////////////////////////////////////
//                                                                       //
// AliFemtoCorrFctn3DLCMSSym: a class to calculate 3D correlation        //
// for pairs of identical particles.                                     //
// In analysis the function should be first created in a macro, then     //
// added to the analysis, and at the end of the macro the procedure to   //
// write out histograms should be called.                                //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "AliFemtoCorrFctn3DLCMSSym.h"

#include <cstdio>

#ifdef __ROOT__ 
ClassImp(AliFemtoCorrFctn3DLCMSSym)
#endif

//____________________________
AliFemtoCorrFctn3DLCMSSym::AliFemtoCorrFctn3DLCMSSym(char* title, const int& nbins, const float& QHi)
  :
  AliFemtoCorrFctn(),
  fNumerator(0),
  fDenominator(0),
  fNumeratorW(0),
  fDenominatorW(0)
{
  // Basic constructor

  // set up numerator
  char tTitNum[101] = "Num";
  strncat(tTitNum,title, 100);
  fNumerator = new TH3F(tTitNum,title,nbins,-QHi,QHi,nbins,-QHi,QHi,nbins,-QHi,QHi);
  // set up denominator
  char tTitDen[101] = "Den";
  strncat(tTitDen,title, 100);
  fDenominator = new TH3F(tTitDen,title,nbins,-QHi,QHi,nbins,-QHi,QHi,nbins,-QHi,QHi);
//Weighted by qinv histos
  // set up numerator
  char tTitNumW[101] = "NumWqinv";
  strncat(tTitNumW,title, 100);
  fNumeratorW = new TH3F(tTitNumW,title,nbins,-QHi,QHi,nbins,-QHi,QHi,nbins,-QHi,QHi);
  // set up denominator
  char tTitDenW[101] = "DenWqinv";
  strncat(tTitDenW,title, 100);
  fDenominatorW = new TH3F(tTitDenW,title,nbins,-QHi,QHi,nbins,-QHi,QHi,nbins,-QHi,QHi);

  // to enable error bar calculation...
  fNumerator->Sumw2();
  fDenominator->Sumw2();
  fNumeratorW->Sumw2();
  fDenominatorW->Sumw2();
}

AliFemtoCorrFctn3DLCMSSym::AliFemtoCorrFctn3DLCMSSym(const AliFemtoCorrFctn3DLCMSSym& aCorrFctn) :
  AliFemtoCorrFctn(aCorrFctn),
  fNumerator(0),
  fDenominator(0),
  fNumeratorW(0),
  fDenominatorW(0)
{
  // Copy constructor
  fNumerator = new TH3F(*aCorrFctn.fNumerator);
  fDenominator = new TH3F(*aCorrFctn.fDenominator);
  fNumeratorW = new TH3F(*aCorrFctn.fNumeratorW);
  fDenominatorW = new TH3F(*aCorrFctn.fDenominatorW);
}
//____________________________
AliFemtoCorrFctn3DLCMSSym::~AliFemtoCorrFctn3DLCMSSym(){
  // Destructor
  delete fNumerator;
  delete fDenominator;
  delete fNumeratorW;
  delete fDenominatorW;
}
//_________________________
AliFemtoCorrFctn3DLCMSSym& AliFemtoCorrFctn3DLCMSSym::operator=(const AliFemtoCorrFctn3DLCMSSym& aCorrFctn)
{
  // assignment operator
  if (this == &aCorrFctn)
    return *this;

  if (fNumerator) delete fNumerator;
  fNumerator = new TH3F(*aCorrFctn.fNumerator);
  if (fDenominator) delete fDenominator;
  fDenominator = new TH3F(*aCorrFctn.fDenominator);
  if (fNumeratorW) delete fNumeratorW;
  fNumeratorW = new TH3F(*aCorrFctn.fNumeratorW);
  if (fDenominatorW) delete fDenominatorW;
  fDenominatorW = new TH3F(*aCorrFctn.fDenominatorW);
  return *this;
}

//_________________________
void AliFemtoCorrFctn3DLCMSSym::WriteOutHistos(){
  // Write out all histograms to file
  fNumerator->Write();
  fDenominator->Write();
  fNumeratorW->Write();
  fDenominatorW->Write();
}
//______________________________
TList* AliFemtoCorrFctn3DLCMSSym::GetOutputList()
{
  // Prepare the list of objects to be written to the output
  TList *tOutputList = new TList();

  tOutputList->Add(fNumerator); 
  tOutputList->Add(fDenominator);  
  tOutputList->Add(fNumeratorW); 
  tOutputList->Add(fDenominatorW);  

  return tOutputList;
}

//_________________________
void AliFemtoCorrFctn3DLCMSSym::Finish(){
  // here is where we should normalize, fit, etc...

}

//____________________________
AliFemtoString AliFemtoCorrFctn3DLCMSSym::Report(){
  // Construct the report
  string stemp = "LCMS Frame Bertsch-Pratt 3D Correlation Function Report:\n";
  char ctemp[100];
  snprintf(ctemp , 100, "Number of entries in numerator:\t%E\n",fNumerator->GetEntries());
  stemp += ctemp;
  snprintf(ctemp , 100, "Number of entries in denominator:\t%E\n",fDenominator->GetEntries());
  stemp += ctemp;

  if (fPairCut){
    snprintf(ctemp , 100, "Here is the PairCut specific to this CorrFctn\n");
    stemp += ctemp;
    stemp += fPairCut->Report();
  }
  else{
    snprintf(ctemp , 100, "No PairCut specific to this CorrFctn\n");
    stemp += ctemp;
  }

  //  
  AliFemtoString returnThis = stemp;
  return returnThis;
}
//____________________________
void AliFemtoCorrFctn3DLCMSSym::AddRealPair( AliFemtoPair* pair){
  // perform operations on real pairs
  if (fPairCut){
    if (!(fPairCut->Pass(pair))) return;
  }

  double qOut = (pair->QOutCMS());
  double qSide = (pair->QSideCMS());
  double qLong = (pair->QLongCMS());
  double qqinv = (pair->QInv());

    fNumerator->Fill(qOut,qSide,qLong);
    fNumeratorW->Fill(qOut,qSide,qLong,qqinv);


   
}
//____________________________
void AliFemtoCorrFctn3DLCMSSym::AddMixedPair( AliFemtoPair* pair){
  // perform operations on mixed pairs
  if (fPairCut){
    if (!(fPairCut->Pass(pair))) return;
  }

  double qOut = (pair->QOutCMS());
  double qSide = (pair->QSideCMS());
  double qLong = (pair->QLongCMS());
  double qqqinv = (pair->QInv());

    fDenominator->Fill(qOut,qSide,qLong,1.0);
    fDenominatorW->Fill(qOut,qSide,qLong,qqqinv);


 
}


