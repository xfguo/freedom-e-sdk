// See LICENSE file for license details

#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdint.h>

#define __iomem
#define __deprecated    __attribute__((deprecated))
#define __must_check            __attribute__((warn_unused_result))

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int32_t  s32;

typedef unsigned int __u32; // to match ../../riscv-gnu-toolchain/linux-headers/include/asm-generic/int-ll64.h:26


typedef unsigned short          umode_t;

typedef unsigned int    __kernel_size_t;
typedef int             __kernel_ssize_t;
typedef long long       __kernel_loff_t;

typedef __kernel_size_t         size_t;
typedef __kernel_ssize_t        ssize_t;
typedef __kernel_loff_t         loff_t;

typedef _Bool                   bool;

#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif

#define I2C_NAME_SIZE	20
#define BITS_PER_LONG   32

// http://lxr.free-electrons.com/source/include/linux/types.h?v=4.6#L185
struct list_head {
        struct list_head *next, *prev;
};

// http://lxr.free-electrons.com/source/include/linux/lockdep.h?v=4.6
#define MAX_LOCKDEP_SUBCLASSES          8UL

/*
 * Lock-classes are keyed via unique addresses, by embedding the
 * lockclass-key into the kernel (or module) .data section. (For
 * static locks we use the lock address itself as the key.)
 */
struct lockdep_subclass_key {
        char __one_byte;
} __attribute__ ((__packed__));

struct lock_class_key {
        struct lockdep_subclass_key     subkeys[MAX_LOCKDEP_SUBCLASSES];
};

// arch/riscv/include/asm/spinlock_types.h
typedef struct {
	volatile unsigned int lock;
} arch_spinlock_t;

// http://lxr.free-electrons.com/source/include/linux/spinlock_types.h?v=4.6#L20
typedef struct raw_spinlock {
        arch_spinlock_t raw_lock;
#ifdef CONFIG_GENERIC_LOCKBREAK
        unsigned int break_lock;
#endif
#ifdef CONFIG_DEBUG_SPINLOCK
        unsigned int magic, owner_cpu;
        void *owner;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
        struct lockdep_map dep_map;
#endif
} raw_spinlock_t;

// http://lxr.free-electrons.com/source/include/linux/spinlock_types.h?v=4.6#L64
typedef struct spinlock {
        union {
                struct raw_spinlock rlock;

#ifdef CONFIG_DEBUG_LOCK_ALLOC
# define LOCK_PADSIZE (offsetof(struct raw_spinlock, dep_map))
                struct {
                        u8 __padding[LOCK_PADSIZE];
                        struct lockdep_map dep_map;
                };
#endif
        };
} spinlock_t;

// http://lxr.free-electrons.com/source/include/linux/types.h?v=4.6#L175
typedef struct {
        int counter;
} atomic_t;

// http://lxr.free-electrons.com/source/include/linux/kref.h?v=4.6#L23
struct kref {
        atomic_t refcount;
};

// http://lxr.free-electrons.com/source/include/uapi/asm-generic/posix_types.h?v=4.6#L47
#ifndef __kernel_uid32_t
typedef unsigned int    __kernel_uid32_t;
typedef unsigned int    __kernel_gid32_t;
#endif

// http://lxr.free-electrons.com/source/include/linux/types.h?v=4.6#L31
typedef __kernel_uid32_t        uid_t;
typedef __kernel_gid32_t        gid_t;

// http://lxr.free-electrons.com/source/include/uapi/linux/types.h?v=4.6#L21
#ifdef __CHECKER__
#define __bitwise__ __attribute__((bitwise))
#else
#define __bitwise__
#endif

 // http://lxr.free-electrons.com/source/include/linux/types.h?v=4.6#L157
typedef unsigned __bitwise__ gfp_t;

// http://lxr.free-electrons.com/source/include/linux/compiler-gcc.h?v=4.6#L119
#define __printf(a, b)          __attribute__((format(printf, a, b)))

// http://lxr.free-electrons.com/source/include/acpi/platform/acenv.h?v=4.6#L366
#ifndef _VALIST
#define _VALIST
typedef char *va_list;
#endif                          /* _VALIST */

// http://lxr.free-electrons.com/source/include/linux/compiler.h?v=4.6#L10
# define __force        __attribute__((force))


// http://lxr.free-electrons.com/source/include/linux/kernel.h?v=4.6#L816
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

// http://lxr.free-electrons.com/source/include/linux/types.h?v=4.6#L12
typedef __u32 __kernel_dev_t;

// http://lxr.free-electrons.com/source/include/linux/types.h?v=4.6#L15
typedef __kernel_dev_t          dev_t;

// include/linux/mmzone.h
enum {
        ZONELIST_FALLBACK,      /* zonelist with fallback */
#ifdef CONFIG_NUMA
        /*
         * The NUMA zonelists are doubled because we need zonelists that
         * restrict the allocations to a single node for __GFP_THISNODE.
         */
        ZONELIST_NOFALLBACK,    /* zonelist without fallback (__GFP_THISNODE) */
#endif
        MAX_ZONELISTS
};

// http://lxr.free-electrons.com/source/include/linux/types.h?v=4.6#L164
typedef u32 phys_addr_t;

// http://lxr.free-electrons.com/source/include/linux/types.h?v=4.6#L167
typedef phys_addr_t resource_size_t;

#endif
