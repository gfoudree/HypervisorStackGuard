## Files of Interest

- `arch/x86/kvm/x86.c`: Contains `kvm_emulate_hypercall()` which is used to handle the hypercalls in a switch statement.
- `include/uapi/linux/kvm_para.h`: Contains the macros for defining a new hypercall number.
- `arch/x86/include/asm/kvm_host.h`: Contains definitions for KVM registers

## Functions of Interest
- `kvm_register_read()`,`kvm_register_write()`: R/W registers
- `kvm_vcpu_read_guest()`: Read guest memory

## How to invoke hypercall
`$hypercall_no`: 0xB = function enter, 0xA = function exit

`mov $hypercall_no, %eax`

`vmcall`


## Determining return address
On function enter and exit, return address is going to be at `%(rbp+8)`
