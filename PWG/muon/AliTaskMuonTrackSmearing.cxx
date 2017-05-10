/**************************************************************************
 * Copyright(c) 1998-2007, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#define AliTaskMuonTrackSmearing_cxx

#include "AliTaskMuonTrackSmearing.h"

// ROOT includes
#include "TROOT.h"
#include "TRootIOCtor.h"
#include "TClonesArray.h"

// STEER includes
#include "AliVEvent.h"
#include "AliAODEvent.h"
#include "AliAODTrack.h"
#include "AliESDMuonTrack.h"
#include "AliMCEvent.h"

// ANALYSIS includes
#include "AliAnalysisMuonUtility.h"

/// \cond CLASSIMP
ClassImp(AliTaskMuonTrackSmearing) // Class implementation in ROOT context
/// \endcond


//________________________________________________________________________
AliTaskMuonTrackSmearing::AliTaskMuonTrackSmearing( TRootIOCtor* ioCtor ) :
AliAnalysisTaskSE(),
fMuonTrackSmearing(ioCtor)
{
  /// Default ctor.
}

//________________________________________________________________________
AliTaskMuonTrackSmearing::AliTaskMuonTrackSmearing ( const char *name, Int_t chosenFunc ) :
AliAnalysisTaskSE(name),
fMuonTrackSmearing(chosenFunc)
{
  //
  /// Constructor.
  //
}


//________________________________________________________________________
AliTaskMuonTrackSmearing::~AliTaskMuonTrackSmearing()
{
  //
  /// Destructor
  //
}

//___________________________________________________________________________
void AliTaskMuonTrackSmearing::UserCreateOutputObjects()
{
  /// Just print some useful information
  AliAnalysisMuonUtility::SetUseSmearedTracks(kTRUE);
  fMuonTrackSmearing.Print();
}

//________________________________________________________________________
void AliTaskMuonTrackSmearing::UserExec ( Option_t * /*option*/ )
{
  //
  /// Smear the generated tracks according to the parameterised muon resolution
  /// The results are stored on the InputEvent, so that they can be used later on
  /// CAVEAT: all classes using AliAnalysisMuonUtility to get the NTracks and the MuonTracks
  /// will automatically read the smeared tracks instead of the standard ones
  //
  AliVParticle* track = 0x0, *genParticle = 0x0;
  Double_t charge = 0.;
  Bool_t isAOD = ( InputEvent()->IsA() == AliAODEvent::Class() );

  TObjArray* smearedTrackList = static_cast<TObjArray*>(InputEvent()->FindListObject(AliAnalysisMuonUtility::GetSmearedTrackListName()));
  if ( ! smearedTrackList ) {
    TString objName = ( isAOD ) ? "AliAODTrack" : "AliESDMuonTrack";
    smearedTrackList = new TObjArray();
    smearedTrackList->SetName(AliAnalysisMuonUtility::GetSmearedTrackListName());
    smearedTrackList->SetOwner();
    InputEvent()->AddObject(smearedTrackList);
  }
  smearedTrackList->Delete();

  AliAnalysisMuonUtility::SetUseSmearedTracks(kFALSE,kFALSE);
  Int_t nTracks = AliAnalysisMuonUtility::GetNTracks(InputEvent());
  for (Int_t itrack = 0; itrack < nTracks; itrack++) {
    track = AliAnalysisMuonUtility::GetTrack(itrack,InputEvent());
    if ( track->GetLabel() < 0 ) continue;
    genParticle = MCEvent()->GetTrack(track->GetLabel());
    TLorentzVector smearedTrack = fMuonTrackSmearing.GetRecoTrack(genParticle->P(),genParticle->Eta(),genParticle->Phi(),genParticle->Charge(),charge);
    if ( isAOD ) { // AOD
      AliAODTrack* aodTrack = static_cast<AliAODTrack*>(track->Clone());
      aodTrack->SetPt(smearedTrack.Pt());
      aodTrack->SetPhi(smearedTrack.Phi());
      aodTrack->SetTheta(smearedTrack.Theta());
      aodTrack->SetCharge(charge);
      smearedTrackList->Add(aodTrack);
    }
    else { // ESD
      AliESDMuonTrack* esdTrack = static_cast<AliESDMuonTrack*>(track->Clone());
      Double_t pz = smearedTrack.Pz();
      Double_t slopeY = smearedTrack.Py()/pz;
      Double_t invMomentum = -1./(pz*TMath::Sqrt(1.+slopeY*slopeY));
//      if ( charge < 0. ) invMomentum *= -1.;
      esdTrack->SetInverseBendingMomentum(invMomentum);
      esdTrack->SetThetaX(TMath::ATan(smearedTrack.Px()/pz));
      esdTrack->SetThetaY(TMath::ATan(slopeY));
      smearedTrackList->Add(esdTrack);
    }
  } // loop on tracks
  smearedTrackList->Compress();
  AliAnalysisMuonUtility::SetUseSmearedTracks(kTRUE,kFALSE);
//  fMuonTrackSmearing.ClearRecoTrackList();
}
