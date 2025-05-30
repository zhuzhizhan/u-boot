/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2017-2018 Intel Corporation <www.intel.com>
 *
 */

#ifndef _MAILBOX_S10_H_
#define _MAILBOX_S10_H_

/* user define Uboot ID */
#include <linux/bitfield.h>
#include <linux/bitops.h>
#define MBOX_CLIENT_ID_UBOOT	0xB
#define MBOX_ID_UBOOT		0x1

#define MBOX_CMD_DIRECT	0
#define MBOX_CMD_INDIRECT	1

#define MBOX_MAX_CMD_INDEX	2047
#define MBOX_CMD_BUFFER_SIZE	32
#define MBOX_RESP_BUFFER_SIZE	16

#define MBOX_HDR_CMD_LSB	0
#define MBOX_HDR_CMD_MSK	(BIT(11) - 1)
#define MBOX_HDR_I_LSB		11
#define MBOX_HDR_I_MSK		BIT(11)
#define MBOX_HDR_LEN_LSB	12
#define MBOX_HDR_LEN_MSK	0x007FF000
#define MBOX_HDR_ID_LSB		24
#define MBOX_HDR_ID_MSK		0x0F000000
#define MBOX_HDR_CLIENT_LSB	28
#define MBOX_HDR_CLIENT_MSK	0xF0000000

/* Interrupt flags */
#define MBOX_FLAGS_INT_COE	BIT(0)	/* COUT update interrupt enable */
#define MBOX_FLAGS_INT_RIE	BIT(1)	/* RIN update interrupt enable */
#define MBOX_FLAGS_INT_UAE	BIT(8)	/* Urgent ACK interrupt enable */
#define MBOX_ALL_INTRS		(MBOX_FLAGS_INT_COE | \
				 MBOX_FLAGS_INT_RIE | \
				 MBOX_FLAGS_INT_UAE)

/* Status */
#define MBOX_STATUS_UA_MSK	BIT(8)

#define MBOX_CMD_HEADER(client, id, len, indirect, cmd)     \
	((((cmd) << MBOX_HDR_CMD_LSB) & MBOX_HDR_CMD_MSK) | \
	(((indirect) << MBOX_HDR_I_LSB) & MBOX_HDR_I_MSK) | \
	(((len) << MBOX_HDR_LEN_LSB) & MBOX_HDR_LEN_MSK)  | \
	(((id) << MBOX_HDR_ID_LSB) & MBOX_HDR_ID_MSK)     | \
	(((client) << MBOX_HDR_CLIENT_LSB) & MBOX_HDR_CLIENT_MSK))

#define MBOX_RESP_ERR_GET(resp)				\
	(((resp) & MBOX_HDR_CMD_MSK) >> MBOX_HDR_CMD_LSB)
#define MBOX_RESP_LEN_GET(resp)			\
	(((resp) & MBOX_HDR_LEN_MSK) >> MBOX_HDR_LEN_LSB)
#define MBOX_RESP_ID_GET(resp)				\
	(((resp) & MBOX_HDR_ID_MSK) >> MBOX_HDR_ID_LSB)
#define MBOX_RESP_CLIENT_GET(resp)			\
	(((resp) & MBOX_HDR_CLIENT_MSK) >> MBOX_HDR_CLIENT_LSB)

#define MBOX_CFG_STATUS_MAJOR_ERR_MSK	GENMASK(31, 16)
#define MBOX_CFG_STATUS_MINOR_ERR_MSK	GENMASK(15, 0)

/* Response error list */
enum ALT_SDM_MBOX_RESP_CODE {
	/* CMD completed successfully, but check resp ARGS for any errors */
	MBOX_RESP_STATOK = 0,
	/* CMD is incorrectly formatted in some way */
	MBOX_RESP_INVALID_COMMAND = 1,
	/* BootROM Command code not undesrtood */
	MBOX_RESP_UNKNOWN_BR = 2,
	/* CMD code not recognized by firmware */
	MBOX_RESP_UNKNOWN = 3,
	/* Length setting is not a valid length for this CMD type */
	MBOX_RESP_INVALID_LEN = 4,
	/* Indirect setting is not valid for this CMD type */
	MBOX_RESP_INVALID_INDIRECT_SETTING = 5,
	/* HW source which is not allowed to send CMD type */
	MBOX_RESP_CMD_INVALID_ON_SRC = 6,
	/* Client with ID not associated with any running PR CMD tries to run
	 * RECONFIG_DATA RECONFIG_STATUS and accessing QSPI / SDMMC using ID
	 * without exclusive access
	 */
	MBOX_RESP_CLIENT_ID_NO_MATCH = 8,
	/* Address provided to the system is invalid (alignment, range
	 * permission)
	 */
	MBOX_RESP_INVALID_ADDR = 0x9,
	/* Signature authentication failed */
	MBOX_RESP_AUTH_FAIL = 0xA,
	/* CMD timed out */
	MBOX_RESP_TIMEOUT = 0xB,
	/* HW (i.e. QSPI) is not ready (initialized or configured) */
	MBOX_RESP_HW_NOT_RDY = 0xC,
	/* Invalid license for IID registration */
	MBOX_RESP_PUF_ACCCES_FAILED = 0x80,
	MBOX_PUF_ENROLL_DISABLE = 0x81,
	MBOX_RESP_PUF_ENROLL_FAIL = 0x82,
	MBOX_RESP_PUF_RAM_TEST_FAIL = 0x83,
	MBOX_RESP_ATTEST_CERT_GEN_FAIL = 0x84,
	/* Operation not allowed under current security settings */
	MBOX_RESP_NOT_ALLOWED_UNDER_SECURITY_SETTINGS = 0x85,
	MBOX_RESP_PUF_TRNG_FAIL = 0x86,
	MBOX_RESP_FUSE_ALREADY_BLOWN = 0x87,
	MBOX_RESP_INVALID_SIGNATURE = 0x88,
	MBOX_RESP_INVALID_HASH = 0x8b,
	MBOX_RESP_INVALID_CERTIFICATE = 0x91,
	/* Indicates that the device (FPGA or HPS) is not configured */
	MBOX_RESP_NOT_CONFIGURED = 0x100,
	/* Indicates that the device is busy */
	MBOX_RESP_DEVICE_BUSY = 0x1FF,
	/* Indicates that there is no valid response available */
	MBOX_RESP_NO_VALID_RESP_AVAILABLE = 0x2FF,
	/* General Error */
	MBOX_RESP_ERROR = 0x3FF,
};

/* Mailbox command list */
#define MBOX_RESTART		2
#define MBOX_CONFIG_STATUS	4
#define MBOX_RECONFIG		6
#define MBOX_RECONFIG_MSEL	7
#define MBOX_RECONFIG_DATA	8
#define MBOX_RECONFIG_STATUS	9
#define MBOX_VAB_SRC_CERT		11
#define MBOX_GET_USERCODE		19
#define MBOX_QSPI_OPEN		50
#define MBOX_QSPI_CLOSE		51
#define MBOX_QSPI_DIRECT	59
#define MBOX_REBOOT_HPS		71

/* Mailbox registers */
#define MBOX_CIN			0	/* command valid offset */
#define MBOX_ROUT			4	/* response output offset */
#define MBOX_URG			8	/* urgent command */
#define MBOX_FLAGS			0x0c	/* interrupt enables */
#define MBOX_COUT			0x20	/* command free offset */
#define MBOX_RIN			0x24	/* respond valid offset */
#define MBOX_STATUS			0x2c	/* mailbox status */
#define MBOX_CMD_BUF			0x40	/* circular command buffer */
#define MBOX_RESP_BUF			0xc0	/* circular response buffer */
#define MBOX_DOORBELL_TO_SDM		0x400	/* Doorbell to SDM */
#define MBOX_DOORBELL_FROM_SDM		0x480	/* Doorbell from SDM */

/* Status and bit information returned by RECONFIG_STATUS */
#define RECONFIG_STATUS_RESPONSE_LEN			6
#define RECONFIG_STATUS_STATE				0
#define RECONFIG_STATUS_PIN_STATUS			2
#define RECONFIG_STATUS_SOFTFUNC_STATUS			3

/* Macros for specifying number of arguments in mailbox command */
#define MBOX_NUM_ARGS(n, b)				(((n) & 0xFF) << (b))
#define MBOX_DIRECT_COUNT(n)				MBOX_NUM_ARGS((n), 0)
#define MBOX_ARG_DESC_COUNT(n)				MBOX_NUM_ARGS((n), 8)
#define MBOX_RESP_DESC_COUNT(n)				MBOX_NUM_ARGS((n), 16)

#define MBOX_CFGSTAT_STATE_IDLE				0x00000000
#define MBOX_CFGSTAT_STATE_CONFIG			0x10000000
#define MBOX_CFGSTAT_STATE_FAILACK			0x08000000
#define MBOX_CFGSTAT_STATE_ERROR_INVALID		0xf0000001
#define MBOX_CFGSTAT_STATE_ERROR_CORRUPT		0xf0000002
#define MBOX_CFGSTAT_STATE_ERROR_AUTH			0xf0000003
#define MBOX_CFGSTAT_STATE_ERROR_CORE_IO		0xf0000004
#define MBOX_CFGSTAT_STATE_ERROR_HARDWARE		0xf0000005
#define MBOX_CFGSTAT_STATE_ERROR_FAKE			0xf0000006
#define MBOX_CFGSTAT_STATE_ERROR_BOOT_INFO		0xf0000007
#define MBOX_CFGSTAT_STATE_ERROR_QSPI_ERROR		0xf0000008

enum MBOX_CFGSTAT_MAJOR_ERR_CODE {
	MBOX_CFGSTATE_MAJOR_ERR_UNK = -1,
	MBOX_CFGSTATE_MAJOR_ERR_WRONG_BL31_VER = 0x0,
	MBOX_CFGSTATE_MAJOR_ERR_STATE_CONFIG = 0x1000,
	MBOX_CFGSTATE_MAJOR_ERR_BITSTREAM_ERR = 0xf001,
	MBOX_CFGSTATE_MAJOR_ERR_EXT_HW_ACCESS_FAIL,
	MBOX_CFGSTATE_MAJOR_ERR_BITSTREAM_CORRUPTION,
	MBOX_CFGSTATE_MAJOR_ERR_INTERNAL_ERR,
	MBOX_CFGSTATE_MAJOR_ERR_DEVICE_ERR,
	MBOX_CFGSTATE_MAJOR_ERR_HPS_WDT,
	MBOX_CFGSTATE_MAJOR_ERR_INTERNAL_UNKNOWN_ERR,
	MBOX_CFGSTATE_MAJOR_ERR_SYSTEM_INIT_ERR,
	MBOX_CFGSTATE_MAJOR_ERR_DECRYPTION_ERR,
	MBOX_CFGSTATE_MAJOR_ERR_VERIFY_IMAGE_ERR = 0xf00b
};

enum MBOX_CFGSTAT_MINOR_ERR_CODE {
	MBOX_CFGSTATE_MINOR_ERR_UNK = -1,
	MBOX_CFGSTATE_MINOR_ERR_BASIC_ERR = 0x0,
	MBOX_CFGSTATE_MINOR_ERR_CNT_RESP_ERR,
	MBOX_CFGSTATE_MINOR_ERR_QSPI_DEV_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_INV,
	MBOX_CFGSTATE_MINOR_ERR_BS_INCOMPATIBLE,
	MBOX_CFGSTATE_MINOR_ERR_BS_INV_SHA,
	MBOX_CFGSTATE_MINOR_ERR_ROUTE_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_GO_BIT_ALREADY_SET,
	MBOX_CFGSTATE_MINOR_ERR_CPU_BLK_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_SKIP_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_MCAST_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_IND_SZ_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_IF_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_PIN_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_FUSEFLTR_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_GENERIC_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_DATA_STARVE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_CNT_RAM_INIT_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_SETUP_S4,
	MBOX_CFGSTATE_MINOR_ERR_WIPE_DATA_STARVE,
	MBOX_CFGSTATE_MINOR_ERR_FUSE_RD_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_AUTH_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_SHA_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_SKIP_DATA_RAM_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_FIXED_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_MCAST_FLTR_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_SECTOR_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_HASH_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_DECOMP_SETUP_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_INTERNAL_OS_ERR,
	MBOX_CFGSTATE_MINOR_ERR_WIPE_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_CNOC_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_RESUME_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_PMF_RUN_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_PMF_PAUSE_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_RET_INT_ASSERT_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_STATE_MACHINE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_CMF_TRANSITION_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_SHA_SETUP_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_WR_DMA_TIMEOUT,
	MBOX_CFGSTATE_MINOR_ERR_MEM_ALLOC_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_SYNC_RD_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_CHK_CFG_REQ_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_HPS_CFG_REQ_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_CFG_HANDLE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_INV_ACTION_ITEM,
	MBOX_CFGSTATE_MINOR_ERR_SKIP_DATA_PREBUF_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MBOX_TIMEOUT,
	MBOX_CFGSTATE_MINOR_ERR_AVST_FIFO_OVERFLOW_ERR,
	MBOX_CFGSTATE_MINOR_ERR_RD_DMA_TIMEOUT,
	MBOX_CFGSTATE_MINOR_ERR_PMF_INIT_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_SHUTDOWN_ERR,
	MBOX_CFGSTATE_MINOR_ERR_BITSTREAM_INTERRUPTED,
	MBOX_CFGSTATE_MINOR_ERR_FPGA_MBOX_WIPE_TIMEOUT,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_TYPE_INV,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_VERSION_INV,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_DEVICE_TYPE_INV,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_DESIGN_HASH_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_EXT_REF_CLK_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_PWR_TBL_INV,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_PIN_TBL_OFST_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_PIN_TBL_INV,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_NO_PIN_TBL,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_CFG_CLK_PLL_FAILED,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_AS_CLK_FAILED,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_POF_ID_FAILED,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_PW_TBL_OFST_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_PP_TBL_OFST_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_PP_TBL_INV,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_SP_TBL_OFST_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_SP_TBL_INV,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_SU_TBL_OFST_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_SU_TBL_INV,
	MBOX_CFGSTATE_MINOR_ERR_MBOX_TASK_CRYPTO_SRC_CLR_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MBOX_TASK_EVENT_GROUP_POST_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRNG_TEST_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_MBOX_TASK_ANTI_DOS_TMR_INIT_ERR,
	MBOX_CFGSTATE_MINOR_ERR_OS_STK_CHK_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MBOX_TASK_INIT,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_COMPAT_ID_MATCH_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_COMPAT_ID_INV,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_AES_ECRYPT_CHK_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_KEY_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_KEY_CHALLENGE_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_MBOX_TASK_MSGQ_DEQUEUE_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_SECT_COMPAT_CHK_ERR,
	MBOX_CFGSTATE_MINOR_ERR_SECT_COMPAT_UPDATE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_SECT_SEC_CHK_FAILED,
	MBOX_CFGSTATE_MINOR_ERR_CNT_RAM_ECC_ERR_UNRECOVERABLE,
	MBOX_CFGSTATE_MINOR_ERR_MBOX_REFORMAT_INPUT_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MBOX_REFORMAT_OUTPUT_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MBOX_WLBL_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MBOX_HOOK_CB_ERR,
	MBOX_CFGSTATE_MINOR_ERR_CMF_RLD_DECOMP_LOAD_ERR,
	MBOX_CFGSTATE_MINOR_ERR_CMF_RLD_DECOMP_RUN_ERR,
	MBOX_CFGSTATE_MINOR_ERR_CNT_PERIPH_ECC_ERR_UNRECOVERABLE,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_SECT_ADDR_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_SCRAMBLE_RATIO_CHK_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_TAMPER_EVENT_TRIGGERED,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_ANTI_TAMPER_TBL_INV,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_EXT_CLCK_MODE_DISALLOWED,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_SEC_OPTIONS_INIT_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_EN_USR_CAN_FUSE_INV,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_AS_DEVICE_NO_SGX_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_POF_ID_LIMIT_EXCEED_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PROVISION_CMF_INV_STATE,
	MBOX_CFGSTATE_MINOR_ERR_PROVISION_CMF_FATAL_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PROVISION_CMF_SM_EXIT_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_PROVISION_CMF_SM_ENTRY_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACTION_DATA_UNSUPPORTED_CTX,
	MBOX_CFGSTATE_MINOR_ERR_CMF_EXCEPTION,
	MBOX_CFGSTATE_MINOR_ERR_ECC_INIT_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_DEFAULT_UNREGISTERED_ISR,
	MBOX_CFGSTATE_MINOR_ERR_GENERAL_TIMEOUT,
	MBOX_CFGSTATE_MINOR_ERR_ACT_OPERATION_CLK_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_ACT_VERIFY_HASH_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_CFG_STATE_UPDATE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_READ_DDR_HASH_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_CVP_FLOW_ERR,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_KEYED_HASH_ERR,
	MBOX_CFGSTATE_MINOR_ERR_CMF_DESC_BAD_JTAG_ID = 0x7a,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_PMF_NOT_SUPPORTED = 0x7d,
	MBOX_CFGSTATE_MINOR_ERR_MAIN_DESC_ANTI_TAMPER_NOT_SUPPORTED,
	MBOX_CFGSTATE_MINOR_ERR_ACT_RECOVERY_FAIL = 0x80,
	MBOX_CFGSTATE_MINOR_ERR_COLD_RESET_CMF_CORRUPTED,
	MBOX_CFGSTATE_MINOR_ERR_COLD_RESET_IO_HPS_CORRUPTED,
	MBOX_CFGSTATE_MINOR_ERR_COLD_RESET_FPGA_CORRUPTED,
	MBOX_CFGSTATE_MINOR_ERR_CRC_CHK_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_COMPAT_TBL_SFIXED_VALUE_INV,
	MBOX_CFGSTATE_MINOR_ERR_FEATURE_EN_FUSE_NOT_BLOWN = 0x87,
	MBOX_CFGSTATE_MINOR_ERR_UIB_REFCLK_MISSING,
	MBOX_CFGSTATE_MINOR_ERR_UIB_REFCLK_TIMEOUT,
	MBOX_CFGSTATE_MINOR_ERR_UIB_REFCLK_TIMEOUT_MISSING,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_SYNC_BLCK_ERR = 0xc001,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_SSBL_SHA_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_BLCK0_SHA_MISMATCH_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_BLCK0_AUTH_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_TRAMP_LOAD_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_CMF_SIZE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_TRANSITION_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_SYNC_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_LOAD_CERT_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_LOAD_NOT_ALLOWED_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_FUSE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_INPUT_BUFFER_ERR = 0xc00d,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_CMF_TYPE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_TRAMP_QSPI_INDR_READ_START_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_I2C_COMM_ERR = 0xc801,
	MBOX_CFGSTATE_MINOR_ERR_PMF_TARGET_VOLTAGE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_HANDSHAKE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_ITD_OUT_OF_RANGE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_PWR_TABLE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_EFUSE_DECODE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_VCCL_PWRGOOD_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_CLR_FAULTS_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_VOUT_MODE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_PAGE_COMMAND_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_VOUT_COMMAND_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_READ_VOUT_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_LTM4677_DEFAULT_ADC_CTRL_ERR,
	MBOX_CFGSTATE_MINOR_ERR_PMF_FIRST_I2C_CMD_FAILED_ERR,
	MBOX_CFGSTATE_MINOR_ERR_RSU_CMF_AUTH_ERR = 0xd001,
	MBOX_CFGSTATE_MINOR_ERR_RSU_USER_AUTH_ERR,
	MBOX_CFGSTATE_MINOR_ERR_RSU_CMF_DESC_SHA_MISMATCH,
	MBOX_CFGSTATE_MINOR_ERR_RSU_POINTERS_NOT_FOUND_ERR,
	MBOX_CFGSTATE_MINOR_ERR_RSU_QSPI_FREQ_CHANGE,
	MBOX_CFGSTATE_MINOR_ERR_RSU_FACTORY_IMG_FAILED,
	MBOX_CFGSTATE_MINOR_ERR_RSU_CMF_TYPE_ERR,
	MBOX_CFGSTATE_MINOR_ERR_RSU_UCMF_SIG_DESC_ERR,
	MBOX_CFGSTATE_MINOR_ERR_RSU_UCMF_INTERNAL_AUTH_ERR,
	MBOX_CFGSTATE_MINOR_ERR_RSU_UCMF_COPY_FAILED,
	MBOX_CFGSTATE_MINOR_ERR_RSU_UCMF_ERASE_FAILED,
	MBOX_CFGSTATE_MINOR_ERR_RSU_RM_UCMF_FROM_CPB_FAILED,
	MBOX_CFGSTATE_MINOR_ERR_RSU_UCMF_COMBINED_APP_AUTH_ERR,
	MBOX_CFGSTATE_MINOR_ERR_RSU_UCMF_FLASH_ACCESS_ERR,
	MBOX_CFGSTATE_MINOR_ERR_RSU_DCMF_DCIO_CORRUPTED,
	MBOX_CFGSTATE_MINOR_ERR_RSU_DCMF_CPB0_CORRUPTED,
	MBOX_CFGSTATE_MINOR_ERR_RSU_DCMF_CPB1_CORRUPTED,
	MBOX_CFGSTATE_MINOR_ERR_RSU_PROVISION_COMPLETE,
	MBOX_CFGSTATE_MINOR_ERR_RSU_PROVISION_ERR,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_EFUSE_INIT_FAIL = 0xe001,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_SEC_PROT_ERR,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_EFUSE_LCK_ERR,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_SEC_BBRAM_CLEAN_ERR,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_ENG_LOAD_DIMK_ERR,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_SEC_UKV_CLEAN_ERR,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_EFUSE_ZERO_ERR,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_ENG_LOAD_ERR,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_PERST_INIT_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_DIMK_INIT_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_PERST_SECONDARY_INIT_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_BR_INFO_INIT_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_CMF_DESC_FAIL,
	MBOX_CFGSTATE_MINOR_ERR_SYSINIT_DRNG_INIT_FAIL
};

#define RCF_SOFTFUNC_STATUS_CONF_DONE			BIT(0)
#define RCF_SOFTFUNC_STATUS_INIT_DONE			BIT(1)
#define RCF_SOFTFUNC_STATUS_SEU_ERROR			BIT(3)
#define RCF_PIN_STATUS_NSTATUS				BIT(31)

int mbox_send_cmd(u8 id, u32 cmd, u8 is_indirect, u32 len, u32 *arg, u8 urgent,
		  u32 *resp_buf_len, u32 *resp_buf);
int mbox_send_cmd_psci(u8 id, u32 cmd, u8 is_indirect, u32 len, u32 *arg,
		       u8 urgent, u32 *resp_buf_len, u32 *resp_buf);
int mbox_send_cmd_only(u8 id, u32 cmd, u8 is_indirect, u32 len, u32 *arg);
int mbox_send_cmd_only_psci(u8 id, u32 cmd, u8 is_indirect, u32 len, u32 *arg);
int mbox_rcv_resp(u32 *resp_buf, u32 resp_buf_max_len);
int mbox_rcv_resp_psci(u32 *resp_buf, u32 resp_buf_max_len);
int mbox_init(void);

#ifdef CONFIG_CADENCE_QSPI
int mbox_qspi_close(void);
int mbox_qspi_open(void);
#endif

int mbox_reset_cold(void);
int mbox_get_fpga_config_status(u32 cmd);
int mbox_get_fpga_config_status_psci(u32 cmd);
#endif /* _MAILBOX_S10_H_ */
