# HypervisorStackGuard

# Installing

## Patching kernel
- Download & extract Linux Kernel 4.19.24
- Run `patch -p0 < kvm.patch` from outside of the kernel folder
- Build and install Kernel

## Reloading KVM kernel module
`make modules -j 2 && make modules_install && rmmod kvm_intel && rmmod kvm && modprobe kvm && modprobe kvm_intel`

# Running

## 16-bit kernel
- `cd simple_kernel && make && make run`
- The hypercall should be logged in the VMM's `dmesg`

# Troubleshooting

`mount: /vagrant: unknown filesystem type 'vboxsf'.``
  - Likely because your VirtualBox Guest Additions are out of date. See this article for more details: https://discoposse.com/2016/05/23/autoupdating-virtualbox-guest-additions-with-vagrant-vbguest/
  - Try running `vagrant plugin install vagrant-vbguest` followed by `vagrant up`


make -C . M=arch/x86/kvm && make modules_install M=arch/x86/kvm
