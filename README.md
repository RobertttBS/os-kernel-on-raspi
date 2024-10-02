# Bare Metal OS Kernel on Raspi 3b+

This project involves developing a fully functional embedded operating system kernel from scratch on the Raspberry Pi 3B+. 

It implements several key system functions and hardware controls.

# Key Features
### Hardware Initialization and Low-level Communication
- Mailbox communication mechanism
- Mini UART shell
- Device tree parsing
- initramfs.cpio

### Interrupt and Exception Handling
- Exception level switch
- Exception handling
- UART interrupt
- Core timer interrupt
- Nested interrupt

### Memory Management
- memblock
- Buddy system
- Slab allocator
- Enable MMU
- 4-level page table walk

### Multitasking
- Threads management
- Scheduler
- System call
- POSIX signal

### File System
- Virtual file system
- FAT32 file system

### Development Environment and Tools
- Programming Languages: C, ARM Assembly
- GDB debugger
- QEMU emulator
- Arm GNU Toolchain

## Build kernel img

```
make
```

## Build user program and produce "initramfs.cpio"
```
./compile_user_program.sh
```

## Test kernel8.img with QEMU

```
make run
```

## Test tty with QEMU
* `make sendimg` and `make commu` is not applicable to this, since the port is not the same.
```
make tty
```

## Send image to Raspi 3 bootloader

```
make sendimg
```

## Communicate with Raspi 3 after `make sendimg`

```
make commu
```
