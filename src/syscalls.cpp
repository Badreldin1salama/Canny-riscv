#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#undef errno
extern int errno;

extern "C" {

// دالة وسيطة لتنفيذ أمر ecall في الأسمبلي وتمرير أرقام الـ Syscalls
static inline long do_syscall(long a7, long a0, long a1, long a2, long a3) {
    register long reg_a7 asm("a7") = a7; // رقم الأمر
    register long reg_a0 asm("a0") = a0; // المعامل الأول
    register long reg_a1 asm("a1") = a1; // المعامل الثاني
    register long reg_a2 asm("a2") = a2; // المعامل الثالث
    register long reg_a3 asm("a3") = a3; // المعامل الرابع
    
    // تنفيذ النداء للمحاكي (QEMU)
    asm volatile ("ecall"
                  : "+r"(reg_a0)
                  : "r"(reg_a1), "r"(reg_a2), "r"(reg_a3), "r"(reg_a7)
                  : "memory");
    return reg_a0;
}

// 1. القراءة (Syscall 63)
int _read(int file, char *ptr, int len) {
    return do_syscall(63, file, (long)ptr, len, 0);
}

// 2. الكتابة والطباعة (Syscall 64)
int _write(int file, char *ptr, int len) {
    return do_syscall(64, file, (long)ptr, len, 0);
}

// 3. فتح الملفات (Syscall 56: openat)
int _open(const char *name, int flags, int mode) {
   
    
   
    int linux_flags = flags & 3; 

    // ترجمة أمر إنشاء وتفريغ الملفات
    if (flags & 0x0200) linux_flags |= 0100;  
    if (flags & 0x0400) linux_flags |= 01000;
    if (flags & 0x0008) linux_flags |= 02000; 

    
    if (linux_flags & 0100) {
        mode = 0666; 
    }

    // AT_FDCWD = -100
    return do_syscall(56, -100, (long)name, linux_flags, mode);
}

// 4. قفل الملفات 
int _close(int file) {
    return do_syscall(57, file, 0, 0, 0);
}

// 5. التنقل جوه الملف 
int _lseek(int file, int ptr, int dir) {
    return do_syscall(62, file, ptr, dir, 0);
}

// 6. حجز الميموري الديناميكي 
void *_sbrk(int incr) {
    static long heap_end = 0;
    if (heap_end == 0) {
        heap_end = do_syscall(214, 0, 0, 0, 0); // الحصول على نهاية الذاكرة الحالية
    }
    long prev_heap_end = heap_end;
    long new_heap_end = prev_heap_end + incr;
    long ret = do_syscall(214, new_heap_end, 0, 0, 0);
    if (ret == prev_heap_end && incr != 0) {
        errno = ENOMEM;
        return (void *)-1;
    }
    heap_end = new_heap_end;
    return (void *)prev_heap_end;
}

// 7. إنهاء البرنامج 
void _exit(int status) {
    do_syscall(93, status, 0, 0, 0);
    while (1);
}

int _fstat(int file, struct stat *st) { return 0; }
int _isatty(int file) { return 1; }
int _kill(int pid, int sig) { return -1; }
int _getpid() { return 1; }

} 