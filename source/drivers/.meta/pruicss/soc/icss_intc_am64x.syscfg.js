/**
 *  @brief PRU_ICSSG Interrupt Requests Mapping
 *
 *  The PRU_ICSSGn Interrupt Controller (where n = 0 or 1) lines 0 through 63 are mapped to internal events which
 *  are generated by PRU_ICSSG integrated modules
 *
 *  The IRQ input lines 64 through 159 receive interrupts which come from various device peripherals located
 *  outside PRU_ICSSG system. They are delivered on the PRU_ICSSG Interrupt Controller inputs (64 through 159).
 *
 *  NOTE:
 *  Set polarity and type of event through the Interrupt Polarity Registers and the Interrupt Type Registers.
 *  Polarity of all interrupts is always high. Type of all interrupts is always pulse (after the processing block).
 */

 let ICSSG0_INTC_INTERNAL_SIGNALS = [
    { eventNumber: "63", interruptSignal: "Any sd_fd_zero/one_max/min(of 72)" },
    { eventNumber: "62", interruptSignal: "pr0_iep1_sync0_uint_intr_pend", source: "PRU_ICSSG0 IEP1" },
    { eventNumber: "61", interruptSignal: "pr0_iep1_sync1_uint_intr_pend", source: "PRU_ICSSG0 IEP1" },
    { eventNumber: "60", interruptSignal: "pr0_iep1_latch0_uint_intr_pend", source: "PRU_ICSSG0 IEP1" },
    { eventNumber: "59", interruptSignal: "pr0_iep1_latch1_uint_intr_pend", source: "PRU_ICSSG0 IEP1" },
    { eventNumber: "58", interruptSignal: "pr0_iep1_pdi_wd_exp_pend", source: "PRU_ICSSG0 IEP1" },
    { eventNumber: "57", interruptSignal: "pr0_iep1_pd_wd_exp_pend", source: "PRU_ICSSG0 IEP1" },
    { eventNumber: "56", interruptSignal: "pr0_iep1_any_cmp_cap_pend", source: "PRU_ICSSG0 IEP1" },
    { eventNumber: "55", interruptSignal: "pr0_mii1_col and pr0_mii1_txen (external)", },
    { eventNumber: "54", interruptSignal: "PRU1_RX_EOF", },
    { eventNumber: "53", interruptSignal: "MDIO_MII_LINK[1]", },
    { eventNumber: "52", interruptSignal: "PORT1_TX_OVERFLOW", },
    { eventNumber: "51", interruptSignal: "PORT1_TX_UNDERFLOW", },
    { eventNumber: "50", interruptSignal: "PRU1_RX_OVERFLOW", },
    { eventNumber: "49", interruptSignal: "PRU1_RX_NIBBLE_ODD", },
    { eventNumber: "48", interruptSignal: "PRU1_RX_CRC", },
    { eventNumber: "47", interruptSignal: "PRU1_RX_SOF", },
    { eventNumber: "46", interruptSignal: "PRU1_RX_SFD", },
    { eventNumber: "45", interruptSignal: "PRU1_RX_ERR32", },
    { eventNumber: "44", interruptSignal: "PRU1_RX_ERR", },
    { eventNumber: "43", interruptSignal: "pr0_mii0_col and pr0_mii0_txen (external)", },
    { eventNumber: "42", interruptSignal: "PRU0_RX_EOF", },
    { eventNumber: "41", interruptSignal: "MDIO_MII_LINK[0]", },
    { eventNumber: "40", interruptSignal: "PORT0_TX_OVERFLOW", },
    { eventNumber: "39", interruptSignal: "PORT0_TX_UNDERFLOW", },
    { eventNumber: "38", interruptSignal: "PRU0_RX_OVERFLOW", },
    { eventNumber: "37", interruptSignal: "PRU0_RX_NIBBLE_ODD", },
    { eventNumber: "36", interruptSignal: "PRU0_RX_CRC", },
    { eventNumber: "35", interruptSignal: "PRU0_RX_SOF", },
    { eventNumber: "34", interruptSignal: "PRU0_RX_SFD", },
    { eventNumber: "33", interruptSignal: "PRU0_RX_ERR32", source: "PRU_ICSSG0 PRU0" },
    { eventNumber: "32", interruptSignal: "PRU0_RX_ERR", source: "PRU_ICSSG0 PRU0" },
    { eventNumber: "31", interruptSignal: "pr0_pru_mst_intr[15]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "30", interruptSignal: "pr0_pru_mst_intr[14]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "29", interruptSignal: "pr0_pru_mst_intr[13]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "28", interruptSignal: "pr0_pru_mst_intr[12]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "27", interruptSignal: "pr0_pru_mst_intr[11]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "26", interruptSignal: "pr0_pru_mst_intr[10]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "25", interruptSignal: "pr0_pru_mst_intr[9]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "24", interruptSignal: "pr0_pru_mst_intr[8]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "23", interruptSignal: "pr0_pru_mst_intr[7]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "22", interruptSignal: "pr0_pru_mst_intr[6]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "21", interruptSignal: "pr0_pru_mst_intr[5]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "20", interruptSignal: "pr0_pru_mst_intr[4]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "19", interruptSignal: "pr0_pru_mst_intr[3]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "18", interruptSignal: "pr0_pru_mst_intr[2]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "17", interruptSignal: "pr0_pru_mst_intr[1]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "16", interruptSignal: "pr0_pru_mst_intr[0]_intr_req", source: "PRU_ICSSG0 PRU0, PRU_ICSSG0 PRU1" },
    { eventNumber: "15", interruptSignal: "pr0_ecap_intr_req", source: "PRU_ICSSG0 ECAP0" },
    { eventNumber: "14", interruptSignal: "pr0_sync0_out_pend", source: "PRU_ICSSG0 IEP0" },
    { eventNumber: "13", interruptSignal: "pr0_sync1_out_pend", source: "PRU_ICSSG0 IEP0" },
    { eventNumber: "12", interruptSignal: "pr0_latch0_in (input to PRU_ICSSG0)", source: "PRU_ICSSG0 IEP0" },
    { eventNumber: "11", interruptSignal: "pr0_latch1_in (input to PRU_ICSSG0)", source: "PRU_ICSSG0 IEP0" },
    { eventNumber: "10", interruptSignal: "pr0_pdi_wd_exp_pend", source: "PRU_ICSSG0 IEP0" },
    { eventNumber: "9",  interruptSignal: "pr0_pd_wd_exp_pend", source: "PRU_ICSSG0 IEP0" },
    { eventNumber: "8",  interruptSignal: "pr0_digio_event_req", source: "PRU_ICSSG0 IEP0" },
    { eventNumber: "7",  interruptSignal: "pr0_iep_tim_cap_cmp_pend", source: "PRU_ICSSG0 IEP0" },
    { eventNumber: "6",  interruptSignal: "pr0_uart0_uint_intr_req", source: "PRU_ICSSG0 UART0" },
    { eventNumber: "5",  interruptSignal: "pr0_uart0_utxevt_intr_req", source: "PRU_ICSSG0 UART0" },
    { eventNumber: "4",  interruptSignal: "pr0_uart0_urxevt_intr_req", source: "PRU_ICSSG0 UART0" },
    { eventNumber: "3",  interruptSignal: "pr0_rst_reset_iso_req", source: "PRU_ICSSG0 Reset Isolation Requested" },
    { eventNumber: "2",  interruptSignal: "pr0_pru1_r31_status_cnt16", source: "PRU_ICSSG0 PRU1 (Shift Capture)" },
    { eventNumber: "1",  interruptSignal: "pr0_pru0_r31_status_cnt16", source: "PRU_ICSSG0 PRU0 (Shift Capture)" },
    { eventNumber: "0",  interruptSignal: "pr0_ecc_err_intr", source: "PRU_ICSSG0 ECC Logic" },
];

let ICSSG1_INTC_INTERNAL_SIGNALS = [
    { eventNumber: "63", interruptSignal: "Any sd_fd_zero/one_max/min(of 72)" },
    { eventNumber: "62", interruptSignal: "pr1_iep1_sync0_uint_intr_pend", source: "PRU_ICSSG1 IEP1" },
    { eventNumber: "61", interruptSignal: "pr1_iep1_sync1_uint_intr_pend", source: "PRU_ICSSG1 IEP1" },
    { eventNumber: "60", interruptSignal: "pr1_iep1_latch0_uint_intr_pend", source: "PRU_ICSSG1 IEP1" },
    { eventNumber: "59", interruptSignal: "pr1_iep1_latch1_uint_intr_pend", source: "PRU_ICSSG1 IEP1" },
    { eventNumber: "58", interruptSignal: "pr1_iep1_pdi_wd_exp_pend", source: "PRU_ICSSG1 IEP1" },
    { eventNumber: "57", interruptSignal: "pr1_iep1_pd_wd_exp_pend", source: "PRU_ICSSG1 IEP1" },
    { eventNumber: "56", interruptSignal: "pr1_iep1_any_cmp_cap_pend", source: "PRU_ICSSG1 IEP1" },
    { eventNumber: "55", interruptSignal: "pr1_mii1_col and pr1_mii1_txen (external)", },
    { eventNumber: "54", interruptSignal: "PRU1_RX_EOF", },
    { eventNumber: "53", interruptSignal: "MDIO_MII_LINK[1]", },
    { eventNumber: "52", interruptSignal: "PORT1_TX_OVERFLOW", },
    { eventNumber: "51", interruptSignal: "PORT1_TX_UNDERFLOW", },
    { eventNumber: "50", interruptSignal: "PRU1_RX_OVERFLOW", },
    { eventNumber: "49", interruptSignal: "PRU1_RX_NIBBLE_ODD", },
    { eventNumber: "48", interruptSignal: "PRU1_RX_CRC", },
    { eventNumber: "47", interruptSignal: "PRU1_RX_SOF", },
    { eventNumber: "46", interruptSignal: "PRU1_RX_SFD", },
    { eventNumber: "45", interruptSignal: "PRU1_RX_ERR32", },
    { eventNumber: "44", interruptSignal: "PRU1_RX_ERR", },
    { eventNumber: "43", interruptSignal: "pr1_mii0_col and pr1_mii0_txen (external)", },
    { eventNumber: "42", interruptSignal: "PRU0_RX_EOF", },
    { eventNumber: "41", interruptSignal: "MDIO_MII_LINK[0]", },
    { eventNumber: "40", interruptSignal: "PORT0_TX_OVERFLOW", },
    { eventNumber: "39", interruptSignal: "PORT0_TX_UNDERFLOW", },
    { eventNumber: "38", interruptSignal: "PRU0_RX_OVERFLOW", },
    { eventNumber: "37", interruptSignal: "PRU0_RX_NIBBLE_ODD", },
    { eventNumber: "36", interruptSignal: "PRU0_RX_CRC", },
    { eventNumber: "35", interruptSignal: "PRU0_RX_SOF", },
    { eventNumber: "34", interruptSignal: "PRU0_RX_SFD", },
    { eventNumber: "33", interruptSignal: "PRU0_RX_ERR32", source: "PRU_ICSSG1 PRU0" },
    { eventNumber: "32", interruptSignal: "PRU0_RX_ERR", source: "PRU_ICSSG1 PRU0" },
    { eventNumber: "31", interruptSignal: "pr1_pru_mst_intr[15]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "30", interruptSignal: "pr1_pru_mst_intr[14]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "29", interruptSignal: "pr1_pru_mst_intr[13]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "28", interruptSignal: "pr1_pru_mst_intr[12]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "27", interruptSignal: "pr1_pru_mst_intr[11]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "26", interruptSignal: "pr1_pru_mst_intr[10]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "25", interruptSignal: "pr1_pru_mst_intr[9]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "24", interruptSignal: "pr1_pru_mst_intr[8]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "23", interruptSignal: "pr1_pru_mst_intr[7]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "22", interruptSignal: "pr1_pru_mst_intr[6]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "21", interruptSignal: "pr1_pru_mst_intr[5]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "20", interruptSignal: "pr1_pru_mst_intr[4]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "19", interruptSignal: "pr1_pru_mst_intr[3]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "18", interruptSignal: "pr1_pru_mst_intr[2]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "17", interruptSignal: "pr1_pru_mst_intr[1]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "16", interruptSignal: "pr1_pru_mst_intr[0]_intr_req", source: "PRU_ICSSG1 PRU0, PRU_ICSSG1 PRU1" },
    { eventNumber: "15", interruptSignal: "pr1_ecap_intr_req", source: "PRU_ICSSG1 ECAP0" },
    { eventNumber: "14", interruptSignal: "pr1_sync0_out_pend", source: "PRU_ICSSG1 IEP0" },
    { eventNumber: "13", interruptSignal: "pr1_sync1_out_pend", source: "PRU_ICSSG1 IEP0" },
    { eventNumber: "12", interruptSignal: "pr1_latch0_in (input to PRU_ICSSG1)", source: "PRU_ICSSG1 IEP0" },
    { eventNumber: "11", interruptSignal: "pr1_latch1_in (input to PRU_ICSSG1)", source: "PRU_ICSSG1 IEP0" },
    { eventNumber: "10", interruptSignal: "pr1_pdi_wd_exp_pend", source: "PRU_ICSSG1 IEP0" },
    { eventNumber: "9",  interruptSignal: "pr1_pd_wd_exp_pend", source: "PRU_ICSSG1 IEP0" },
    { eventNumber: "8",  interruptSignal: "pr1_digio_event_req", source: "PRU_ICSSG1 IEP0" },
    { eventNumber: "7",  interruptSignal: "pr1_iep_tim_cap_cmp_pend", source: "PRU_ICSSG1 IEP0" },
    { eventNumber: "6",  interruptSignal: "pr1_uart0_uint_intr_req", source: "PRU_ICSSG1 UART0" },
    { eventNumber: "5",  interruptSignal: "pr1_uart0_utxevt_intr_req", source: "PRU_ICSSG1 UART0" },
    { eventNumber: "4",  interruptSignal: "pr1_uart0_urxevt_intr_req", source: "PRU_ICSSG1 UART0" },
    { eventNumber: "3",  interruptSignal: "pr1_rst_reset_iso_req", source: "PRU_ICSSG1 Reset Isolation Requested" },
    { eventNumber: "2",  interruptSignal: "pr1_pru1_r31_status_cnt16", source: "PRU_ICSSG1 PRU1 (Shift Capture)" },
    { eventNumber: "1",  interruptSignal: "pr1_pru0_r31_status_cnt16", source: "PRU_ICSSG1 PRU0 (Shift Capture)" },
    { eventNumber: "0",  interruptSignal: "pr1_ecc_err_intr", source: "PRU_ICSSG1 ECC Logic" },
];

ICSSG0_INTC_INTERNAL_SIGNALS.reverse();
ICSSG1_INTC_INTERNAL_SIGNALS.reverse();

/**
 *  Add 64 through 159 receive interrupts to the list of event signals
 */
let ICSSG0_INTC_EXTERNAL_SIGNALS = [
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_0",  interruptId: "0",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_152" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_1",  interruptId: "1",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_153" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_2",  interruptId: "2",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_154" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_3",  interruptId: "3",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_155" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_4",  interruptId: "4",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_156" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_5",  interruptId: "5",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_157" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_6",  interruptId: "6",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_158" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_7",  interruptId: "7",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_159" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_8",  interruptId: "8",  source: "ECAP0_ECAP_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_9",  interruptId: "9",  source: "ECAP1_ECAP_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_10", interruptId: "10", source: "ECAP2_ECAP_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_12", interruptId: "12", source: "EPWM0_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_13", interruptId: "13", source: "EPWM0_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_14", interruptId: "14", source: "EPWM1_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_15", interruptId: "15", source: "EPWM1_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_16", interruptId: "16", source: "EPWM2_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_17", interruptId: "17", source: "EPWM2_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_18", interruptId: "18", source: "EPWM3_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_19", interruptId: "19", source: "EPWM3_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_20", interruptId: "20", source: "EPWM4_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_21", interruptId: "21", source: "EPWM4_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_22", interruptId: "22", source: "EPWM5_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_23", interruptId: "23", source: "EPWM5_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_24", interruptId: "24", source: "EPWM6_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_25", interruptId: "25", source: "EPWM6_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_26", interruptId: "26", source: "EPWM7_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_27", interruptId: "27", source: "EPWM7_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_28", interruptId: "28", source: "EPWM8_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_29", interruptId: "29", source: "EPWM8_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_30", interruptId: "30", source: "EQEP0_EQEP_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_31", interruptId: "31", source: "EQEP1_EQEP_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_32", interruptId: "32", source: "EQEP2_EQEP_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_33", interruptId: "33", source: "FSIRX0_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_34", interruptId: "34", source: "FSIRX0_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_35", interruptId: "35", source: "FSIRX1_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_36", interruptId: "36", source: "FSIRX1_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_37", interruptId: "37", source: "FSIRX2_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_38", interruptId: "38", source: "FSIRX2_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_39", interruptId: "39", source: "FSIRX3_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_40", interruptId: "40", source: "FSIRX3_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_41", interruptId: "41", source: "FSIRX4_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_42", interruptId: "42", source: "FSIRX4_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_43", interruptId: "43", source: "FSIRX5_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_44", interruptId: "44", source: "FSIRX5_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_45", interruptId: "45", source: "FSITX0_FSI_TX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_46", interruptId: "46", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_38" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_47", interruptId: "47", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_39" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_48", interruptId: "48", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_40" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_49", interruptId: "49", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_41" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_50", interruptId: "50", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_42" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_51", interruptId: "51", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_43" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_52", interruptId: "52", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_44" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_53", interruptId: "53", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_45" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_54", interruptId: "54", source: "FSITX0_FSI_TX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_55", interruptId: "55", source: "FSITX1_FSI_TX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_56", interruptId: "56", source: "FSITX1_FSI_TX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_57", interruptId: "57", source: "MCSPI0_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_58", interruptId: "58", source: "MCSPI1_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_59", interruptId: "59", source: "MCSPI3_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_60", interruptId: "60", source: "MCSPI4_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_61", interruptId: "61", source: "MCU_MCSPI0_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_62", interruptId: "62", source: "MCU_MCSPI1_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_63", interruptId: "63", source: "UART0_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_64", interruptId: "64", source: "UART1_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_65", interruptId: "65", source: "UART2_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_66", interruptId: "66", source: "UART3_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_67", interruptId: "67", source: "UART4_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_68", interruptId: "68", source: "UART5_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_69", interruptId: "69", source: "UART6_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_70", interruptId: "70", source: "MCU_UART0_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_71", interruptId: "71", source: "MCU_UART1_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_72", interruptId: "72", source: "MCSPI2_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_73", interruptId: "73", source: "GLUELOGIC_SOCA_INT_GLUE_SOCA_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_74", interruptId: "74", source: "GLUELOGIC_SOCB_INT_GLUE_SOCB_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_75", interruptId: "75", source: "I2C0_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_76", interruptId: "76", source: "I2C1_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_77", interruptId: "77", source: "I2C2_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_78", interruptId: "78", source: "I2C3_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_79", interruptId: "79", source: "MCU_I2C0_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_80", interruptId: "80", source: "MCU_I2C1_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_81", interruptId: "81", source: "MCAN0_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_82", interruptId: "82", source: "MCAN0_MCANSS_MCAN_LVL_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_83", interruptId: "83", source: "MCAN0_MCANSS_MCAN_LVL_INT_1" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_84", interruptId: "84", source: "MCAN1_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_85", interruptId: "85", source: "MCAN1_MCANSS_MCAN_LVL_INT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_86", interruptId: "86", source: "MCAN1_MCANSS_MCAN_LVL_INT_1" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_87", interruptId: "87", source: "PRU_ICSSG1_PR1_HOST_INTR_PEND_5" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_88", interruptId: "88", source: "PRU_ICSSG1_PR1_HOST_INTR_PEND_6" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_89", interruptId: "89", source: "PRU_ICSSG1_PR1_HOST_INTR_PEND_7" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_90", interruptId: "90", source: "GPMC0_GPMC_SINTERRUPT_0" },
    { interruptSignal: "PRU_ICSSG0_PR1_SLV_IN_91", interruptId: "91", source: "ADC0_GEN_LEVEL_0" },
];
/*
Remaining signals:
{ interruptSignal: "PRU_ICSSG0_PR1_IEP0_CAP_INTR_REQ0", interruptId: "0", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_18" },
{ interruptSignal: "PRU_ICSSG0_PR1_IEP0_CAP_INTR_REQ1", interruptId: "1", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_19" },
{ interruptSignal: "PRU_ICSSG0_PR1_IEP0_CAP_INTR_REQ2", interruptId: "2", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_20" },
{ interruptSignal: "PRU_ICSSG0_PR1_IEP0_CAP_INTR_REQ3", interruptId: "3", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_21" },
{ interruptSignal: "PRU_ICSSG0_PR1_IEP0_CAP_INTR_REQ4", interruptId: "4", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_22" },
{ interruptSignal: "PRU_ICSSG0_PR1_IEP0_CAP_INTR_REQ5", interruptId: "5", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_23" },
{ interruptSignal: "PRU_ICSSG0_PR1_IEP1_CAP_INTR_REQ0", interruptId: "0", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_24" },
{ interruptSignal: "PRU_ICSSG0_PR1_IEP1_CAP_INTR_REQ1", interruptId: "1", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_25" },
{ interruptSignal: "PRU_ICSSG0_PR1_IEP1_CAP_INTR_REQ2", interruptId: "2", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_26" },
{ interruptSignal: "PRU_ICSSG0_PR1_IEP1_CAP_INTR_REQ3", interruptId: "3", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_27" },
{ interruptSignal: "PRU_ICSSG0_PR1_IEP1_CAP_INTR_REQ4", interruptId: "4", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_28" },
Given in TRM:
{ interruptSignal: "PRU_ICSSG0_PR1_IEP0_CAP_INTR_REQ5", interruptId: "5", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_29" },
Should be:
{ interruptSignal: "PRU_ICSSG0_PR1_IEP1_CAP_INTR_REQ5", interruptId: "5", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_29" },
*/

let ICSSG1_INTC_EXTERNAL_SIGNALS = [
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_0",  interruptId: "0",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_160" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_1",  interruptId: "1",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_161" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_2",  interruptId: "2",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_162" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_3",  interruptId: "3",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_163" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_4",  interruptId: "4",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_164" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_5",  interruptId: "5",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_165" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_6",  interruptId: "6",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_166" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_7",  interruptId: "7",  source: "DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_167" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_8",  interruptId: "8",  source: "ECAP0_ECAP_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_9",  interruptId: "9",  source: "ECAP1_ECAP_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_10", interruptId: "10", source: "ECAP2_ECAP_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_12", interruptId: "12", source: "EPWM0_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_13", interruptId: "13", source: "EPWM0_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_14", interruptId: "14", source: "EPWM1_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_15", interruptId: "15", source: "EPWM1_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_16", interruptId: "16", source: "EPWM2_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_17", interruptId: "17", source: "EPWM2_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_18", interruptId: "18", source: "EPWM3_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_19", interruptId: "19", source: "EPWM3_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_20", interruptId: "20", source: "EPWM4_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_21", interruptId: "21", source: "EPWM4_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_22", interruptId: "22", source: "EPWM5_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_23", interruptId: "23", source: "EPWM5_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_24", interruptId: "24", source: "EPWM6_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_25", interruptId: "25", source: "EPWM6_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_26", interruptId: "26", source: "EPWM7_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_27", interruptId: "27", source: "EPWM7_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_28", interruptId: "28", source: "EPWM8_EPWM_ETINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_29", interruptId: "29", source: "EPWM8_EPWM_TRIPZINT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_30", interruptId: "30", source: "EQEP0_EQEP_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_31", interruptId: "31", source: "EQEP1_EQEP_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_32", interruptId: "32", source: "EQEP2_EQEP_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_33", interruptId: "33", source: "FSIRX0_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_34", interruptId: "34", source: "FSIRX0_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_35", interruptId: "35", source: "FSIRX1_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_36", interruptId: "36", source: "FSIRX1_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_37", interruptId: "37", source: "FSIRX2_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_38", interruptId: "38", source: "FSIRX2_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_39", interruptId: "39", source: "FSIRX3_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_40", interruptId: "40", source: "FSIRX3_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_41", interruptId: "41", source: "FSIRX4_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_42", interruptId: "42", source: "FSIRX4_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_43", interruptId: "43", source: "FSIRX5_FSI_RX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_44", interruptId: "44", source: "FSIRX5_FSI_RX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_45", interruptId: "45", source: "FSITX0_FSI_TX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_46", interruptId: "46", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_46" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_47", interruptId: "47", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_47" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_48", interruptId: "48", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_48" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_49", interruptId: "49", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_49" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_50", interruptId: "50", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_50" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_51", interruptId: "51", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_51" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_52", interruptId: "52", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_52" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_53", interruptId: "53", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_53" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_54", interruptId: "54", source: "FSITX0_FSI_TX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_55", interruptId: "55", source: "FSITX1_FSI_TX_OINT1_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_56", interruptId: "56", source: "FSITX1_FSI_TX_OINT2_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_57", interruptId: "57", source: "MCSPI0_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_58", interruptId: "58", source: "MCSPI1_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_59", interruptId: "59", source: "MCSPI3_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_60", interruptId: "60", source: "MCSPI4_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_61", interruptId: "61", source: "MCU_MCSPI0_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_62", interruptId: "62", source: "MCU_MCSPI1_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_63", interruptId: "63", source: "UART0_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_64", interruptId: "64", source: "UART1_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_65", interruptId: "65", source: "UART2_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_66", interruptId: "66", source: "UART3_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_67", interruptId: "67", source: "UART4_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_68", interruptId: "68", source: "UART5_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_69", interruptId: "69", source: "UART6_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_70", interruptId: "70", source: "MCU_UART0_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_71", interruptId: "71", source: "MCU_UART1_USART_IRQ_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_72", interruptId: "72", source: "MCSPI2_INTR_SPI_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_73", interruptId: "73", source: "GLUELOGIC_SOCA_INT_GLUE_SOCA_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_74", interruptId: "74", source: "GLUELOGIC_SOCB_INT_GLUE_SOCB_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_75", interruptId: "75", source: "I2C0_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_76", interruptId: "76", source: "I2C1_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_77", interruptId: "77", source: "I2C2_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_78", interruptId: "78", source: "I2C3_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_79", interruptId: "79", source: "MCU_I2C0_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_80", interruptId: "80", source: "MCU_I2C1_POINTRPEND_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_81", interruptId: "81", source: "MCAN0_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_82", interruptId: "82", source: "MCAN0_MCANSS_MCAN_LVL_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_83", interruptId: "83", source: "MCAN0_MCANSS_MCAN_LVL_INT_1" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_84", interruptId: "84", source: "MCAN1_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_85", interruptId: "85", source: "MCAN1_MCANSS_MCAN_LVL_INT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_86", interruptId: "86", source: "MCAN1_MCANSS_MCAN_LVL_INT_1" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_87", interruptId: "87", source: "PRU_ICSSG0_PR1_HOST_INTR_PEND_5" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_88", interruptId: "88", source: "PRU_ICSSG0_PR1_HOST_INTR_PEND_6" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_89", interruptId: "89", source: "PRU_ICSSG0_PR1_HOST_INTR_PEND_7" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_90", interruptId: "90", source: "GPMC0_GPMC_SINTERRUPT_0" },
    { interruptSignal: "PRU_ICSSG1_PR1_SLV_IN_91", interruptId: "91", source: "ADC0_GEN_LEVEL_0" },
];

/*
Remaining signals:
{ interruptSignal: "PRU_ICSSG1_PR1_IEP0_CAP_INTR_REQ0", interruptId: "0", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_18" },
{ interruptSignal: "PRU_ICSSG1_PR1_IEP0_CAP_INTR_REQ1", interruptId: "1", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_19" },
{ interruptSignal: "PRU_ICSSG1_PR1_IEP0_CAP_INTR_REQ2", interruptId: "2", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_20" },
{ interruptSignal: "PRU_ICSSG1_PR1_IEP0_CAP_INTR_REQ3", interruptId: "3", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_21" },
{ interruptSignal: "PRU_ICSSG1_PR1_IEP0_CAP_INTR_REQ4", interruptId: "4", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_22" },
{ interruptSignal: "PRU_ICSSG1_PR1_IEP0_CAP_INTR_REQ5", interruptId: "5", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_23" },
{ interruptSignal: "PRU_ICSSG1_PR1_IEP1_CAP_INTR_REQ0", interruptId: "0", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_24" },
{ interruptSignal: "PRU_ICSSG1_PR1_IEP1_CAP_INTR_REQ1", interruptId: "1", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_25" },
{ interruptSignal: "PRU_ICSSG1_PR1_IEP1_CAP_INTR_REQ2", interruptId: "2", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_26" },
{ interruptSignal: "PRU_ICSSG1_PR1_IEP1_CAP_INTR_REQ3", interruptId: "3", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_27" },
{ interruptSignal: "PRU_ICSSG1_PR1_IEP1_CAP_INTR_REQ4", interruptId: "4", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_28" },
Given in TRM:
{ interruptSignal: "PRU_ICSSG1_PR1_IEP0_CAP_INTR_REQ5", interruptId: "5", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_29" },
Should be:
{ interruptSignal: "PRU_ICSSG1_PR1_IEP1_CAP_INTR_REQ5", interruptId: "5", source: "MAIN_GPIOMUX_INTROUTER0_OUTP_29" },
*/

/* add evenNumber property for external events */
let i = 64;
for (let option of ICSSG0_INTC_EXTERNAL_SIGNALS) {
    option.eventNumber = `${i}`;
    i++;
}
i = 64;
for (let option of ICSSG1_INTC_EXTERNAL_SIGNALS) {
    option.eventNumber = `${i}`;
    i++;
}

let ICSSG0_INTC_SIGNALS = ICSSG0_INTC_INTERNAL_SIGNALS.concat(ICSSG0_INTC_EXTERNAL_SIGNALS);
let ICSSG1_INTC_SIGNALS = ICSSG1_INTC_INTERNAL_SIGNALS.concat(ICSSG1_INTC_EXTERNAL_SIGNALS);

let ICSSG_INTC_CHANNELS   = [...Array(20).keys()].map(String);     /* 0 to 19 */
let ICSSG_INTC_HOST_INTRS = [...Array(20).keys()].map(String);     /* 0 to 19 */

/**
 *
 * @param {*} instance
 * @param {*} type
 * @returns options array
 */
function getEventConfigOptions (instance, type = "all") {
    let intc_signals;
    if (type === "all")
        intc_signals = instance === "ICSSG1" ? ICSSG1_INTC_SIGNALS : ICSSG0_INTC_SIGNALS;
    if (type === "internal")
        intc_signals = instance === "ICSSG1" ? ICSSG1_INTC_INTERNAL_SIGNALS : ICSSG0_INTC_INTERNAL_SIGNALS;
    if (type === "external")
        intc_signals = instance === "ICSSG1" ? ICSSG1_INTC_EXTERNAL_SIGNALS : ICSSG0_INTC_EXTERNAL_SIGNALS;
    let options = [];
    for (let signal of intc_signals) {
        let description = `Event no.: ${signal.eventNumber}`;
        if (signal.source)  description += `, Source: ${signal.source}`
        let option = {
            name: signal.eventNumber,
            displayName: signal.eventNumber + ": " + signal.interruptSignal,
            description,
        }
        options.push(option);
    }
    return options;
}

function getChannelConfigOptions () {
    let options = [];
    for (let channel of ICSSG_INTC_CHANNELS) {
        let description = `Channel no.: ${channel}`;
        let option = {
            name: channel,
            displayName: `Channel ${channel}`,
            description,
        }
        options.push(option);
    }
    return options;
}

function getHostConfigOptions () {
    let options = [];
    for (let hostInterrupt of ICSSG_INTC_HOST_INTRS) {
        let description = `Host Interrupt no.: ${hostInterrupt}`;
        let option = {
            name: hostInterrupt,
            displayName: `Host Interrupt ${hostInterrupt}`,
            description,
        }
        options.push(option);
    }
    return options;
}

function getDisabledOptionsSubset (allOptions, filter, reason) {
    let disabledOptions = [];
    allOptions.forEach(option => {
        if (option.displayName.includes(filter))
            disabledOptions.push({
                name: option.name,
                reason,
            });
    });
    return disabledOptions;
}

function getDisabledOptionsSubsetNot (allOptions, filter, reason) {
    let disabledOptions = [];
    allOptions.forEach(option => {
        if (!option.displayName.includes(filter))
            disabledOptions.push({
                name: option.name,
                reason,
            });
    });
    return disabledOptions;
}

function getDisabledOptionsMtoN (allOptions, m, n, reason) {
    let disabledOptions = [];
    if (m > n) return [];
    for (let i = m; i <= n; i++) {
        disabledOptions.push({
            name: allOptions[i].name,
            reason,
        });
    }
    return disabledOptions;
}

exports = {
    getEventConfigOptions,
    getChannelConfigOptions,
    getHostConfigOptions,
    getDisabledOptionsSubset,
    getDisabledOptionsSubsetNot,
    getDisabledOptionsMtoN,
};