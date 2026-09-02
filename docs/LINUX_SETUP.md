# Linux Setup

## Ubuntu/Debian packages

```bash
sudo apt update
sudo apt install -y build-essential git cmake ninja-build make openocd gdb-multiarch gcc-arm-none-eabi binutils-arm-none-eabi
```

## ST-Link permission problem

ถ้า `openocd` เปิด ST-Link ไม่ได้ ให้เช็คก่อน:

```bash
lsusb
```

จากนั้นเพิ่ม udev rule สำหรับ ST-Link ตาม distro ที่ใช้งาน หรือใช้แพ็กเกจ `stlink-tools` เพื่อดึง rule มาช่วยได้:

```bash
sudo apt install -y stlink-tools
sudo udevadm control --reload-rules
sudo udevadm trigger
```

ถอดเสียบ ST-Link ใหม่หลัง reload rule
