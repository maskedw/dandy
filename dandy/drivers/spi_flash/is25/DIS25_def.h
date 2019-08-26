/**
 *       @file  DIS25_def.h
 *      @brief
 *
 *    @details
 *    ISSI IS25Series Serial NOR Flash
 *    http://www.issi.com/JP/product-flash.shtml
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/07/14
 * ===================================================================
 */

#ifndef app_DIS25_def_h_
#define app_DIS25_def_h_


#define D_IS25_CMD_NORD         (0x03) /* Normal Read Mode */
#define D_IS25_CMD_4NORD        (0x13) /* 4-byte Address Normal Read Mode */
#define D_IS25_CMD_FRD          (0x0B) /* Fast Read Mode */
#define D_IS25_CMD_4FRD         (0x0C) /* 4-byte Address Fast Read Mode */
#define D_IS25_CMD_FRDIO        (0xBB) /* Fast Read Dual I/O */
#define D_IS25_CMD_4FRDIO       (0xBC) /* 4-byte Address Fast Read Dual I/O */
#define D_IS25_CMD_FRDO         (0x3B) /* Fast Read Dual Output */
#define D_IS25_CMD_4FRDO        (0x3C) /* 4-byte Address Fast Read Dual Output */
#define D_IS25_CMD_FRQIO        (0xEB) /* Fast Read Quad I/O */
#define D_IS25_CMD_4FRQIO       (0xEC) /* 4-byte Address Fast Read Quad I/O */
#define D_IS25_CMD_FRQO         (0x6B) /* Fast Read Quad Output */
#define D_IS25_CMD_4FRQO        (0x6C) /* 4-byte Address Fast Read Quad Output */
#define D_IS25_CMD_FRDTR        (0x0D) /* Fast Read DTR Mode */
#define D_IS25_CMD_4FRDTR       (0x0E) /* 4-byte Address Fast Read DTR Mode */
#define D_IS25_CMD_FRDDTR       (0xBD) /* Fast Read Dual I/O DTR */
#define D_IS25_CMD_4FRDDTR      (0xBE) /* 4-byte Address Fast Read Dual I/O DTR */
#define D_IS25_CMD_FRQDTR       (0xED) /* Fast Read Quad I/O DTR */
#define D_IS25_CMD_4FRQDTR      (0xEE) /* 4-byte Address Fast Read Quad I/O DTR */
#define D_IS25_CMD_PP           (0x02) /* Serial Input Page Program */
#define D_IS25_CMD_4PP          (0x12) /* 4-byte Address Serial Input Page Program */
#define D_IS25_CMD_PPQ          (0x32) /* Quad Input Page Program */
#define D_IS25_CMD_4PPQ         (0x34) /* 4-byte Address Quad Input Page Program */
#define D_IS25_CMD_WREN         (0x06) /* Write Enable */
#define D_IS25_CMD_WRDI         (0x04) /* Write Disable */
#define D_IS25_CMD_WRSR         (0x01) /* Write Status Register */
#define D_IS25_CMD_RDSR         (0x05) /* Read Status Register */
#define D_IS25_CMD_SER          (0xD7) /* Sector Erase 4KB (3-byte or 4-byte Address) */
#define D_IS25_CMD_4SER         (0x21) /* Sector Erase 4KB (4-byte Address) */
#define D_IS25_CMD_BER32        (0x52) /* Block Erase 32KB (3-byte or 4-byte Address) */
#define D_IS25_CMD_4BER32       (0x5C) /* Block Erase 32KB (4-byte Address) */
#define D_IS25_CMD_BER64        (0xD8) /* Block Erase 64KB (3-byte or 4-byte Address) */
#define D_IS25_CMD_4BER64       (0xDC) /* Block Erase 64KB (4-byte Address) */
#define D_IS25_CMD_CER          (0xC7) /* Chip Erase */
#define D_IS25_CMD_WRFR         (0x42) /* Write Function Register */
#define D_IS25_CMD_RDFR         (0x48) /* Read Function Register */
#define D_IS25_CMD_CLERP        (0x82) /* Clear Extended Read Register */
#define D_IS25_CMD_PERRSM       (0x7A) /* Program/Erase Resume */
#define D_IS25_CMD_PERSUS       (0x75) /* Program/Erase Suspend */
#define D_IS25_CMD_RDID         (0xAB) /* Read Manufacturer and Product ID */
#define D_IS25_CMD_SRPV         (0xC0) /* Set Read Parameters (Volatile) */
#define D_IS25_CMD_SERPV        (0x83) /* Set Extended Read Parameters (Volatile) */
#define D_IS25_CMD_RDRP         (0x61) /* Read Read Parameters (Volatile) */
#define D_IS25_CMD_RDERP        (0x81) /* Read Extended Read Parameters (Volatile) */
#define D_IS25_CMD_RDJDID       (0x9F) /* Read Manufacturer and Product ID by JEDEC ID Command */
#define D_IS25_CMD_RDMDID       (0x90) /* Read Manufacturer and Device ID */
#define D_IS25_CMD_RDJDIDQ      (0xAF) /* Read JEDEC ID QPI mode */
#define D_IS25_CMD_RDUID        (0x4B) /* Read Unique ID Number */
#define D_IS25_CMD_RDSFDP       (0x5A) /* SFDP Read */
#define D_IS25_CMD_NOP          (0x00) /* No Operation */
#define D_IS25_CMD_RSTEN        (0x66) /* Software reset enable */
#define D_IS25_CMD_RST          (0x99) /* Reset (Only along with RSTEN 66h) */
#define D_IS25_CMD_IRRD         (0x78) /* Read Information Row */
#define D_IS25_CMD_SECUNLOCK    (0x26) /* Sector Unlock */
#define D_IS25_CMD_4SECUNLOCK   (0x25) /* 4-byte Address Sector Unlock */
#define D_IS25_CMD_SECLOCK      (0x24) /* Sector Lock */
#define D_IS25_CMD_RDABR        (0x14) /* Read AutoBoot Register */
#define D_IS25_CMD_RDBR         (0x16) /* Read Bank Address Register */
#define D_IS25_CMD_WRBRV        (0x17) /* Write Volatile Bank Address Register */
#define D_IS25_CMD_EN4B         (0xB7) /* Enter 4-byte Address Mode */
#define D_IS25_CMD_EX4B         (0x29) /* Exit 4-byte Address Mode */
#define D_IS25_CMD_RDDYB        (0xFA) /* Read DYB */
#define D_IS25_CMD_4RDDYB       (0xE0) /* 4-byte Address Read DYB */
#define D_IS25_CMD_WRDYB        (0xFB) /* Write DYB */
#define D_IS25_CMD_4WRDYB       (0xE1) /* 4-byte Address Write DYB */
#define D_IS25_CMD_RDPPB        (0xFC) /* Read PPB */
#define D_IS25_CMD_4RDPPB       (0xE2) /* 4-byte Address Read PPB */

#define D_IS25_ERASE_SIZE_4KB   (1024UL * 4)
#define D_IS25_ERASE_SIZE_32KB  (1024UL * 32)
#define D_IS25_ERASE_SIZE_64KB  (1024UL * 64)
#define D_IS25_ERASE_SIZE_MIN   (D_IS25_ERASE_SIZE_4KB)

#define D_IS25_SR_BIT_WIP       (1UL << 0)
#define D_IS25_SR_BIT_WEL       (1UL << 1)
#define D_IS25_SR_BIT_BP0       (1UL << 2)
#define D_IS25_SR_BIT_BP1       (1UL << 3)
#define D_IS25_SR_BIT_BP2       (1UL << 4)
#define D_IS25_SR_BIT_BP3       (1UL << 5)
#define D_IS25_SR_BIT_QE        (1UL << 6)
#define D_IS25_SR_BIT_SRWD      (1UL << 7)

#define D_IS25xx32_ADDRESS_BEGIN  (0x00000000)
#define D_IS25xx32_ADDRESS_END    (0x00400000)
#define D_IS25xx32_SIZE           (D_IS25xx32_ADDRESS_END - D_IS25xx32_ADDRESS_BEGIN) /* 4MB */

#define D_IS25xx64_ADDRESS_BEGIN  (0x00000000)
#define D_IS25xx64_ADDRESS_END    (0x00800000)
#define D_IS25xx64_SIZE           (D_IS25xx64_ADDRESS_END - D_IS25xx64_ADDRESS_BEGIN) /* 8MB */

#define D_IS25xx128_ADDRESS_BEGIN (0x00000000)
#define D_IS25xx128_ADDRESS_END   (0x01000000)
#define D_IS25xx128_SIZE          (D_IS25xx128_ADDRESS_END - D_IS25xx128_ADDRESS_BEGIN) /* 16MB */

#define D_IS25xx256_ADDRESS_BEGIN (0x00000000)
#define D_IS25xx256_ADDRESS_END   (0x02000000)
#define D_IS25xx256_SIZE          (D_IS25xx256_ADDRESS_END - D_IS25xx256_ADDRESS_BEGIN) /* 32MB */

#define D_IS25xx512_ADDRESS_BEGIN (0x00000000)
#define D_IS25xx512_ADDRESS_END   (0x04000000)
#define D_IS25xx512_SIZE          (D_IS25xx512_ADDRESS_END - D_IS25xx512_ADDRESS_BEGIN) /* 64MB */


#endif /* end of include guard: app_DIS25_def_h_ */
