/**
 *       @file  DSST26_def.h
 *      @brief
 *
 *    @details
 *    Microchip  Serial SPI/SQI™ NOR Flash Devices
 *    https://www.microchip.com/design-centers/memory/serial-parallel-flash/superflash-technology-features/serial-flash
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/06/04
 * ===================================================================
 */

#ifndef dandy_DSST26_def_h_
#define dandy_DSST26_def_h_

/* DataSheet TABLE 5-1: DEVICE OPERATION INSTRUCTIONS FOR SST26VF064B/064BA */
/* Configuration */
#define D_SST26_CMD_NOP           (0x00) /* No Operation */
#define D_SST26_CMD_RSTEN         (0x66) /* Reset Enable */
#define D_SST26_CMD_RST           (0x99) /* Reset Memory */
#define D_SST26_CMD_EQIO          (0x38) /* Enable Quad I/O */
#define D_SST26_CMD_RSTQIO        (0xFF) /* Reset Quad I/O */
#define D_SST26_CMD_RDSR          (0x05) /* Read Status Register */
#define D_SST26_CMD_WRSR          (0x01) /* Write Status Register */
#define D_SST26_CMD_RDCR          (0x35) /* Read Configuration Register */
/* Read */
#define D_SST26_CMD_Read          (0x03) /* Read Memory */
#define D_SST26_CMD_HighSpeedRead (0x0B) /* Read Memory at Higher Speed */
#define D_SST26_CMD_SQOR          (0x6B) /* SPI Quad Output Read */
#define D_SST26_CMD_SQIOR         (0xEB) /* SPI Quad I/O Read */
#define D_SST26_CMD_SDOR          (0x3B) /* SPI Dual Output Read */
#define D_SST26_CMD_SDIOR         (0xBB) /* SPI Dual I/O Read */
#define D_SST26_CMD_SB            (0xC0) /* Set Burst Length */
#define D_SST26_CMD_RBSQI         (0x0C) /* SQI Read Burst with Wrap */
#define D_SST26_CMD_RBSPI         (0xEC) /* SPI Read Burst with Wrap */
#define D_SST26_CMD_ReadBurst     (0x0C) /* nB Burst with Wrap */
#define D_SST26_CMD_ReadPI        (0x08) /* Jump to address within 256 Byte page indexed by n */
#define D_SST26_CMD_ReadI         (0x09) /* Jump to address within block indexed by n */
#define D_SST26_CMD_ReadBI        (0x10) /* Jump to block indexed by n */
/* Identification */
#define D_SST26_CMD_JEDEC_ID      (0x9F) /* JEDEC-ID Read */
#define D_SST26_CMD_QuadJ_ID      (0xAF) /* Quad J-ID Read */
#define D_SST26_CMD_SFDP          (0x5A) /* Serial Flash Discoverable Parameters */
/* Write */
#define D_SST26_CMD_WREN          (0x06) /* Write Enable */
#define D_SST26_CMD_WRDI          (0x04) /* Write Disable */
#define D_SST26_CMD_SE            (0x20) /* Erase 4KBytes of Memory Array */
#define D_SST26_CMD_BE            (0xD8) /* Erase 64, 32 or 8KBytes of Memory Array */
#define D_SST26_CMD_CE            (0xC7) /* Erase Full Array */
#define D_SST26_CMD_PP            (0x02) /* Page Program */
#define D_SST26_CMD_SPIQuadPP     (0x32) /* SQI Quad Page Program */
#define D_SST26_CMD_WRSU          (0xB0) /* Suspends Program/Erase */
#define D_SST26_CMD_WRRE          (0x30) /* Resumes Program/Erase */
/* Protection */
#define D_SST26_CMD_RBPR          (0x72) /* Read Block-Protection Register */
#define D_SST26_CMD_WBPR          (0x42) /* Write Block-Protection Register */
#define D_SST26_CMD_LBPR          (0x8D) /* Lock Down Block-Protection Register */
#define D_SST26_CMD_nVWLDR        (0xE8) /* non-Volatile Write Lock-Down Register */
#define D_SST26_CMD_ULBPR         (0x98) /* Global Block Protection Unlock */
#define D_SST26_CMD_RSID          (0x88) /* Read Security ID */
#define D_SST26_CMD_PSID          (0xA5) /* Program User Security ID area */
#define D_SST26_CMD_LSID          (0x85) /* Lockout Security ID Programming */

/* DataSheet TABLE4-2: STATUS RAGISTER */
#define D_SST26_SR_BIT_BUSY       (1UL << 0)
#define D_SST26_SR_BIT_WEL        (1UL << 1)
#define D_SST26_SR_BIT_WSE        (1UL << 2)
#define D_SST26_SR_BIT_WSP        (1UL << 3)
#define D_SST26_SR_BIT_WPLD       (1UL << 4)
#define D_SST26_SR_BIT_SEC        (1UL << 5)
#define D_SST26_SR_BIT_RE         (1UL << 6)
#define D_SST26_SR_BIT_BUSY2      (1UL << 7)

/* DataSheet TABLE4-3: CONFIGURATION RAGISTER */
#define D_SST26_CR_BIT_RES1       (1UL << 0)
#define D_SST26_CR_BIT_IOC        (1UL << 1)
#define D_SST26_CR_BIT_RES2       (1UL << 2)
#define D_SST26_CR_BIT_BPNV       (1UL << 3)
#define D_SST26_CR_BIT_RES3       (1UL << 4)
#define D_SST26_CR_BIT_RES4       (1UL << 5)
#define D_SST26_CR_BIT_RES5       (1UL << 6)
#define D_SST26_CR_BIT_WPEN       (1UL << 7)

#define D_SST26_ERASE_SIZE_4KB    (1024UL * 4)
#define D_SST26_ERASE_SIZE_8KB    (1024UL * 8)
#define D_SST26_ERASE_SIZE_32KB   (1024UL * 32)
#define D_SST26_ERASE_SIZE_64KB   (1024UL * 64)
#define D_SST26_ERASE_SIZE_MIN    (D_SST26_ERASE_SIZE_4KB)

#define D_SST26XX016_ADDRESS_BEGIN (0x0000000)
#define D_SST26XX016_ADDRESS_END   (0x2000000)
#define D_SST26XX016_SIZE          (D_SST26XX016_ADDRESS_END - D_SST26XX016_ADDRESS_BEGIN) /* 2MB */
#define D_SST26XX032_ADDRESS_BEGIN (0x0000000)
#define D_SST26XX032_ADDRESS_END   (0x4000000)
#define D_SST26XX032_SIZE          (D_SST26XX032_ADDRESS_END - D_SST26XX032_ADDRESS_BEGIN) /* 4MB */
#define D_SST26XX064_ADDRESS_BEGIN (0x0000000)
#define D_SST26XX064_ADDRESS_END   (0x8000000)
#define D_SST26XX064_SIZE          (D_SST26XX064_ADDRESS_END - D_SST26XX064_ADDRESS_BEGIN) /* 8MB */

#endif /* end of include guard: dandy_DSST26_def_h_ */
