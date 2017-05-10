#ifndef ALIEMCALFASTORMONITORTASK_H
#define ALIEMCALFASTORMONITORTASK_H
/* Copyright(c) 1998-2016, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

#include "AliAnalysisTaskSE.h"
#include "AliEMCALTriggerDataGrid.h"
#include <TString.h>

class AliEMCALGeometry;
class AliOADBContainer;
class THistManager;

/**
 * @class AliEmcalFastOrMonitorTask
 * @brief Simlple monitoring of EMCAL FastOr quantities
 * @author Markus Fasel <markus.fasel@cern.ch>, Oak Ridge National Laboratory
 * @since Sept 8, 2016
 * @ingroup EMCALFWTASKS
 *
 * Simple monitoring task for FastOr related quantities. Filling the following
 * distributions:
 * - Frequency
 * - Amplitude
 * - L0timeSum
 * - Number of LO times
 * - Position in the col-row space
 *
 * The wagon can be added to the train using the corresponding AddMacro (AddEmcalFastOrMonitorTask):
 *
 *  ~~~{.cxx}
 *  gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/AddEmcalFastOrMonitorTask.C");
 *  AddEmcalFastOrMonitorTask();
 *  ~~~
 */
class AliEmcalFastOrMonitorTask : public AliAnalysisTaskSE {
public:

  /**
   * @brief Default constructor.
   *
   * For ROOT I/O
   */
  AliEmcalFastOrMonitorTask();

  /**
   * @brief Named constructor
   * @param[in] name Name of the task
   */
  AliEmcalFastOrMonitorTask(const char *name);

  /**
   * @brief Destructor
   */
  virtual ~AliEmcalFastOrMonitorTask();

  /**
   * @brief Define trigger selection.
   *
   * It can consist of trigger bits and a trigger string
   * @param[in] triggerbits Trigger bit selection
   * @param[in] triggerstring Trigger string (i.e. EG1, EG2, DG1, ...)
   */
  void SetRequestTrigger(ULong_t triggerbits, TString triggerstring = "") {
    fRequestTrigger = triggerbits;
    fTriggerPattern = triggerstring;
  }

  /**
   * @brief Add masked fastor to the list of masked fastors.
   *
   * Masked fastors will be ignored in ADC spectrum.
   * @param[in] fastorID Abs ID of the fastor to be masked
   */
  void AddMaskedFastor(int fastorID){ fMaskedFastors.push_back(fastorID); }

  /**
   * @brief Specify location of the OADB container with masked FastORs
   * @param[in] oadbname Name of the OADB container with the masked fastors
   */
  void DefineMaskedFastorOADB(const char *oadbname) { fNameMaskedFastorOADB = oadbname; }

  /**
   * @brief Specify location of the OADB container with masked cells
   * @param[in] oadbname Name of the OADB container with the masked cells
   */
  void DefineMaskedCellOADB(const char *oadbname) { fNameMaskedCellOADB = oadbname; }

protected:

  /**
   * @brief Creating output objects
   *
   * For this task only the histogram handler is created.
   * Histograms are done inside the function ExecOnce.
   */
  virtual void UserCreateOutputObjects();

  /**
   * @brief Event loop
   *
   * Processing of events: Filling the monitoring histograms for each FastOR:
   * - Frequency
   * - Amplitude
   * - L0timeSum
   * - Number of LO times
   * - Position in the col-row space
   * @param
   */
  virtual void UserExec(Option_t *);

  /**
   * @brief Initialization of the task
   *
   * Performing initial initializations. In contrast to UserCreateOutputObjects,
   * which is called before the event loop, ExecOnce is called for the first event
   * within the event loop. At that step some basic event information is already
   * available,
   */
  virtual void ExecOnce();

  /**
   * @brief Run-dependent setup of the task
   *
   * Performing run-dependent initializations. Function is called each time the run number
   * of the current event differs to the run number of the previous event. This function is
   * useful i.e. to load parameters from the OCDB/OADB
   *
   * @param[in] newrun New run number
   */
  virtual void RunChanged(Int_t newrun);

  /**
   * @brief Calculate transverse L1 time sum of a FastOR.
   * @param fastorAbsID FastOR absolute ID (for position calculation)
   * @param adc FastOR ADC
   * @param vtx Position of the primary vertex
   * @return Transverse L1 time sum
   */
  Double_t GetTransverseTimeSum(Int_t fastorAbsID, Double_t adc, const Double_t *vtx) const;

  /**
   * @brief Load event-dependent cell data
   *
   * For easier processing EMCAL cell data is loaded into a trigger
   * data grid with the fastor granularity. Energy of the cells belonging
   * to the same fastor is summed. The data grid is allocated for with
   * the proper size once the first event is processed.
   */
  void LoadEventCellData();

  THistManager                            *fHistos;           //!<! Histogram handler
  AliEMCALGeometry                        *fGeom;             //!<! EMCAL Geometry object
  Bool_t                                  fLocalInitialized;  ///< Switch whether task is initialized (for ExecOnce)
  Int_t                                   fOldRun;            ///< Old Run (for RunChanged())

  ULong_t                                 fRequestTrigger;    ///< Trigger selection bits
  TString                                 fTriggerPattern;    ///< Trigger string pattern used in addition to the trigger selection bits

  AliEMCALTriggerDataGrid<double>         fCellData;          ///< Grid with summed cell data
  std::vector<int>                        fMaskedFastors;     ///< List of masked fastors
  std::vector<int>                        fMaskedCells;       ///< List of masked cells

  TString                                 fNameMaskedFastorOADB; ///< Name of the OADB container with masked fastors
  TString                                 fNameMaskedCellOADB;   ///< Name of the OADB container with masked cells
  AliOADBContainer                        *fMaskedFastorOADB; //!<! OADB container with masked fastors
  AliOADBContainer                        *fMaskedCellOADB;   //!<! OADB container with masked cells

  /// \cond CLASSIMP
  ClassDef(AliEmcalFastOrMonitorTask, 1);
  /// \endcond
};

#endif /* ALIEMCALFASTORMONITORTASK_H */
