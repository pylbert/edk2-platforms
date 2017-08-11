/** @file
  Provide FSP wrapper platform related function.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under
the terms and conditions of the BSD License that accompanies this distribution.
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/FspWrapperApiLib.h>
#include <Library/FspPolicyUpdateLib.h>

#include <FspEas.h>
#include <FspmUpd.h>
#include <FspsUpd.h>

/**
  Performs FSP Misc UPD initialization.

  @param[in][out]  FspmUpd             Pointer to FSPM_UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
**/
EFI_STATUS
EFIAPI
PeiFspMiscUpdUpdatePreMem (
  IN OUT FSPM_UPD    *FspmUpd
  );

/**
  Performs FSP PCH PEI Policy pre mem initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyUpdatePreMem (
  IN OUT FSPM_UPD    *FspmUpd
  );

/**
  Performs FSP PCH PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyUpdate (
  IN OUT FSPS_UPD    *FspsUpd
  );

/**
  Performs FSP SA PEI Policy initialization in pre-memory.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyUpdatePreMem (
  IN OUT FSPM_UPD    *FspmUpd
  );

/**
  Performs FSP SA PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyUpdate (
  IN OUT FSPS_UPD    *FspsUpd
  );

VOID
InternalPrintVariableData (
  IN UINT8   *Data8,
  IN UINTN   DataSize
  )
{
  UINTN      Index;

  for (Index = 0; Index < DataSize; Index++) {
    if (Index % 0x10 == 0) {
      DEBUG ((DEBUG_INFO, "\n%08X:", Index));
    }
    DEBUG ((DEBUG_INFO, " %02X", *Data8++));
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

/**
  Performs FSPM UPD Policy update.

  A platform may use this API to update the FSPM UPD policy initialized
  by the silicon module or the default UPD data.
  The output of FSPM UPD data from this API is the final UPD data.

  @param[in, out] FspmUpd       Pointer to FSPM UPD data.
**/
VOID
EFIAPI
FspmPolicyUpdate (
  IN OUT VOID    *FspmUpd
  )
{
  FSPM_UPD              *FspmUpdDataPtr;

  FspmUpdDataPtr = FspmUpd;
  PeiFspSaPolicyUpdatePreMem (FspmUpdDataPtr);
  PeiFspPchPolicyUpdatePreMem (FspmUpdDataPtr);
  PeiFspMiscUpdUpdatePreMem (FspmUpdDataPtr);

  InternalPrintVariableData ((VOID *)FspmUpdDataPtr, sizeof(FSPM_UPD));
}

/**
  Performs FSPS UPD Policy update.

  A platform may use this API to update the FSPS UPD policy initialized
  by the silicon module or the default UPD data.
  The output of FSPS UPD data from this API is the final UPD data.

  @param[in, out] FspsUpd       Pointer to FSPS UPD data.
**/
VOID
EFIAPI
FspsPolicyUpdate (
  IN OUT VOID    *FspsUpd
  )
{
  FSPS_UPD              *FspsUpdDataPtr;

  FspsUpdDataPtr = FspsUpd;
  PeiFspSaPolicyUpdate (FspsUpdDataPtr);
  PeiFspPchPolicyUpdate (FspsUpdDataPtr);
  
  InternalPrintVariableData ((VOID *)FspsUpdDataPtr, sizeof(FSPS_UPD));
}

