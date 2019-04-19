## Files of Interest

- `arch/x86/kvm/x86.c`: Contains `kvm_emulate_hypercall()` which is used to handle the hypercalls in a switch statement.
- `include/uapi/linux/kvm_para.h`: Contains the macros for defining a new hypercall number.
- `arch/x86/include/asm/kvm_host.h`: Contains definitions for KVM registers

## Functions of Interest
- `kvm_register_read()`,`kvm_register_write()`: R/W registers
- `kvm_vcpu_read_guest()`: Read guest memory

## How to invoke hypercall
`$hypercall_no`: 0x1 = function enter, 0x2 = function exit

`mov $0xB, %eax`

`mov $hypercall_no, %ebx`

`vmcall`


## Determining return address
On function enter and exit, return address is going to be at `%(rbp+8)`


## Measuring performance

`perf kvm stat live`
