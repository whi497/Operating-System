.global start
/*定义协议要求的参数*/
magic_ITEM_NAME = 0X1BADB002;#magic
flag_ITEM_NAME = 0X00000000;#flag
check_ITEM_NAME = -0X1BADB002;#check

.section ".multiboot_header"#对应link文件的文件名
.align 4
.long magic_ITEM_NAME
.long flag_ITEM_NAME
.long check_ITEM_NAME
/*与前面参数构建头结构*/

.text#代码段
.code32 #声明为32位代码
start:
        movl $0xf242f250, 0xB8000   #学号
        movl $0xf230f232, 0xB8004
        movl $0xf232f230, 0xB8008
        movl $0xf235f230, 0xB800C
        movl $0xf236f238, 0xB8010
        movl $0xf25f, 0xB8014       #下划线
        movl $0xf2B6f2D2, 0xB8016   #姓名
        movl $0xf2D3f2D7, 0xB801A
        movl $0xf2BAf2B0, 0xB801E
        # movl $0xf242f250, 0xB8000
        # movl $0xf242f250, 0xB8000
        # movl $0xf242f250, 0xB8000
        # movb $0x46, %al
        # movw $0x3F8, %dx
        # outb %al, %dx
        nop
        nop