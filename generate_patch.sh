#!/bin/sh
diff -Naur source/linux-5.0.8/arch/x86/kvm/x86.c linux-5.0.8/arch/x86/kvm/x86.c > kvm.patch
diff -Naur source/linux-5.0.8/include/uapi/linux/kvm_para.h linux-5.0.8/include/uapi/linux/kvm_para.h >> kvm.patch
