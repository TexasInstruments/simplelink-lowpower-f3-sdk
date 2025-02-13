// ===========================================================================
// This file is autogenerated, please DO NOT modify!
//
// Generated on  2024-12-05 15:17:20
// by user:      developer
// on machine:   swtools
// CWD:          /home/developer/.conan/data/loki-lrf/8.40.00.09/library-lprf/ga/build/0c46501566d33cb4afdce9818f8c3e61ffe04c9a/build/highlrfbledig/iar/pbe/ble_cs
// Commandline:  /home/developer/.conan/data/loki-lrf/8.40.00.09/library-lprf/ga/build/0c46501566d33cb4afdce9818f8c3e61ffe04c9a/highlrfbledig/../tools/topsm/regtxtconv.pl -x /home/developer/.conan/data/f65lokihighlrfbledig/1.3.7-3/library-lprf/eng/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/source/ti.com_LOKI_LRFBLEDIG_1.0.xml -f acr --devices CC2745P10Q1:A (1.0) /home/developer/.conan/data/loki-lrf/8.40.00.09/library-lprf/ga/build/0c46501566d33cb4afdce9818f8c3e61ffe04c9a/highlrfbledig/pbe/ble_cs/doc/pbe_ble_cs_ram_regs.txt /home/developer/.conan/data/loki-lrf/8.40.00.09/library-lprf/ga/build/0c46501566d33cb4afdce9818f8c3e61ffe04c9a/highlrfbledig/pbe/ble_cs/doc/pbe_ble_cs_regdef_regs.txt
// C&P friendly: /home/developer/.conan/data/loki-lrf/8.40.00.09/library-lprf/ga/build/0c46501566d33cb4afdce9818f8c3e61ffe04c9a/highlrfbledig/../tools/topsm/regtxtconv.pl -x /home/developer/.conan/data/f65lokihighlrfbledig/1.3.7-3/library-lprf/eng/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/source/ti.com_LOKI_LRFBLEDIG_1.0.xml -f acr --devices CC2745P10Q1:A (1.0) /home/developer/.conan/data/loki-lrf/8.40.00.09/library-lprf/ga/build/0c46501566d33cb4afdce9818f8c3e61ffe04c9a/highlrfbledig/pbe/ble_cs/doc/pbe_ble_cs_ram_regs.txt /home/developer/.conan/data/loki-lrf/8.40.00.09/library-lprf/ga/build/0c46501566d33cb4afdce9818f8c3e61ffe04c9a/highlrfbledig/pbe/ble_cs/doc/pbe_ble_cs_regdef_regs.txt
//
// Relevant file version(s):
//
// /home/developer/.conan/data/loki-lrf/8.40.00.09/library-lprf/ga/build/0c46501566d33cb4afdce9818f8c3e61ffe04c9a/highlrfbledig/../tools/topsm/regtxtconv.pl
//   rcs-info: (file not managed or unknown revision control system)
//   git-hash: 68a752a8737845355f7bdb320d25a59eac685840
//
// /home/developer/.conan/data/loki-lrf/8.40.00.09/library-lprf/ga/build/0c46501566d33cb4afdce9818f8c3e61ffe04c9a/highlrfbledig/pbe/ble_cs/doc/pbe_ble_cs_regdef_regs.txt
//   rcs-info: (file not managed or unknown revision control system)
//   git-hash: 622b06341c4551904e7b7c1dc6b2c1df5d995fdb
//
// ===========================================================================


#ifndef __PBE_BLE_CS_REGDEF_REGS_H
#define __PBE_BLE_CS_REGDEF_REGS_H

//******************************************************************************
// REGISTER OFFSETS
//******************************************************************************
// PBE API Operation Register
#define PBE_BLE_CS_REGDEF_O_API                                      0x00000030U

//******************************************************************************
// Register: API
//******************************************************************************
// Field: [4:0] op
//
// PBE Operation
#define PBE_BLE_CS_REGDEF_API_OP_W                                            5U
#define PBE_BLE_CS_REGDEF_API_OP_M                                   0x0000001FU
#define PBE_BLE_CS_REGDEF_API_OP_S                                            0U
#define PBE_BLE_CS_REGDEF_API_OP_PING                                0x00000000U
#define PBE_BLE_CS_REGDEF_API_OP_STOP                                0x00000001U
#define PBE_BLE_CS_REGDEF_API_OP_BLE_CS                              0x00000002U
#define PBE_BLE_CS_REGDEF_API_OP_PRECAL                              0x00000003U


#endif // __PBE_BLE_CS_REGDEF_REGS_H
