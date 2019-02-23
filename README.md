# HypervisorStackGuard

# Installing

## Patching kernel
- Download & extract Linux Kernel 4.19.24
- Run `patch -p0 < kvm.patch` from outside of the kernel folder
- Build and install Kernel

# Running

## 16-bit kernel
- `cd simple_kernel && make && make run`
- The hypercall should be logged in the VMM's `dmesg`
