ielftool.exe --bin "%2.out" "%2.bin"
 %1/tools/common/mcuboot/imgtool sign --header-size 0x100 --align 4 --slot-size %3 --version 1.0.0 --pad-header --pad --key %1/source/third_party/mcuboot/root-ec-p256.pem "%2.bin" "%2.bin"
