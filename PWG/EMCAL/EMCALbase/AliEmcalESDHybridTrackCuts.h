/************************************************************************************
 * Copyright (C) 2017, Copyright Holders of the ALICE Collaboration                 *
 * All rights reserved.                                                             *
 *                                                                                  *
 * Redistribution and use in source and binary forms, with or without               *
 * modification, are permitted provided that the following conditions are met:      *
 *     * Redistributions of source code must retain the above copyright             *
 *       notice, this list of conditions and the following disclaimer.              *
 *     * Redistributions in binary form must reproduce the above copyright          *
 *       notice, this list of conditions and the following disclaimer in the        *
 *       documentation and/or other materials provided with the distribution.       *
 *     * Neither the name of the <organization> nor the                             *
 *       names of its contributors may be used to endorse or promote products       *
 *       derived from this software without specific prior written permission.      *
 *                                                                                  *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND  *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED    *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
 * DISCLAIMED. IN NO EVENT SHALL ALICE COLLABORATION BE LIABLE FOR ANY              *
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES       *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;     *
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND      *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS    *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     *
 ************************************************************************************/
#ifndef ALIEMCALESDHYBRIDTRACKCUTS_H
#define ALIEMCALESDHYBRIDTRACKCUTS_H

#include "AliEmcalCutBase.h"

class AliESDtrackCuts;

namespace PWG {

namespace EMCAL {

/**
 * @class AliEmcalESDHybridTrackCuts
 * @brief Track cuts object selecting hybrid tracks from ESDs
 * @ingroup EMCALCOREFW
 * @author Markus Fasel <markus.fasel@cern.ch>, Oak Ridge National Laboratory
 * @since Dec 6, 2017
 * 
 * Hybrid track selection as cut object, implemented as AliVCuts object. Focusing
 * on the usage in virtual track selections. Hybrid tracks are defined in three
 * cathegories (global, constrained, complementary), and a track is selected 
 * as hybrid track if at least one of the three cases is fulfilled.
 */
class AliEmcalESDHybridTrackCuts : public AliEmcalCutBase {
public:
  
  /**
   * @enum HybridDefinition_t
   * @brief Definition of various hybrid track selections
   */
  enum HybridDefinition_t {
    kDef2010,   //!< Definition used for 2010 pass1-2 and LHC11a
    kDef2011    //!< Definition used since 2011 (LHC11h)
  };

  /**
   * @brief Dummy constructor
   */
  AliEmcalESDHybridTrackCuts();

  /**
   * @brief Main constructor
   * 
   * @param name Name of the cut object
   * @param def Hybrid track definition
   */
  AliEmcalESDHybridTrackCuts(const char *name, HybridDefinition_t def);

  /**
   * @brief Destructor
   */
  virtual ~AliEmcalESDHybridTrackCuts();

  /**
   * @brief Test whether track is accepted as hybrid track
   * 
   * @param o Track to be tested
   * @return true Track is accepted as hybrid track
   * @return false Track is not accepted as hybrid track or track is not an AliESDtrack
   */
  virtual AliEmcalTrackSelResultPtr IsSelected(TObject *o);

  /**
   * @brief Set the hybrid track definition used in the hybrid track selection
   * 
   * @param def Hybrid track definition to be used
   */
  void SetHybridDefinition(HybridDefinition_t def);

  /**
   * @brief Define whether to use non-ITStrefit tracks
   * 
   * @param doUse If true non-ITS refit tracks will be used (independent of the hybrid track definition)
   */
  void SetUseNoITSrefitTracks(bool doUse) { fSelectNonRefitTracks = doUse; }

protected:

  /**
   * @brief  Steer initialization of track cuts objects
   * 
   * Track cuts for various categories are created based on the hybrid
   * track defintion.
   */
  void Init();

  /**
   * @brief Initialize hybrid track selection using the 2010 definition
   * 
   * Cuts defined as
   * - Global tracks with SPD requirement
   * - Constrained tracks without SPD requirement
   * - Complementary tracks (no SPD, no refit) - optional
   */
  void InitHybridTracks2010();

  /**
   * @brief Initialize hybrid track selection using the 2011 definition
   * 
   * Cuts defined as:
   * - Global tracks with SPD requirement
   * - Constrained tracks with SPD requirement
   * - Complementary tracks (no SPD, no refit) - optional
   */
  void InitHybridTracks2011();

private:
  bool                  fLocalInitialized;                  ///< Local init status flag steering lazy initialization
  bool                  fSelectNonRefitTracks;              ///< Select tracks which did not pass ITS refit
  HybridDefinition_t    fHybridTrackDefinition;             ///< Setting for hybrid track definition
  AliESDtrackCuts       *fHybridTrackCutsGlobal;            ///< Track cuts for global hybrid tracks
  AliESDtrackCuts       *fHybridTrackCutsConstrained;       ///< Track cuts for constrained hybrid tracks
  AliESDtrackCuts       *fHybridTrackCutsNoItsRefit;        ///< Track cuts for complementary hybrid tracks (constrained without ITSrefit)

  /// \cond CLASSIMP
  ClassDef(AliEmcalESDHybridTrackCuts, 1);
  /// \endif
};

}

}

#endif