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
        /*the output of VGA*/
        movl $0xf242f250, 0xB8000   #学号
        movl $0xf230f232, 0xB8004
        movl $0xf232f230, 0xB8008
        movl $0xf235f230, 0xB800C
        movl $0xf236f238, 0xB8010
        movw $0xf200, 0xB8014       #下划线
        movl $0xf265f268, 0xB8016 #hello,myos!
        movl $0xf26cf26c, 0xB801A
        movl $0xf22cf26f, 0xB801E
        movl $0xf279f26d, 0xB8022
        movl $0xf253f24f, 0xB8026
        movl $0xf200f221, 0xB802A 
        movl $0xf200f200, 0xB802E #覆盖原有内容(ubuntu...)
        movl $0xf200f200, 0xB8032
        movl $0xf200f200, 0xB8036
        movl $0xf200f200, 0xB803A
        movl $0xf200f200, 0xB803E
        movl $0xf200f200, 0xB8042
        movl $0xf200f200, 0xB8046
        movl $0xf200f200, 0xB804A
        movl $0xf200f200, 0xB804E
        movl $0xf200f200, 0xB8052
        movl $0xf200f200, 0xB8056
        /*the output of uart*/
        movb $0x68, %al           #hello,yeziang
        movw $0x3F8, %dx
        outb %al, %dx
        movb $0x65, %al
        outb %al, %dx
        movb $0x6c, %al
        outb %al, %dx
        movb $0x6c, %al
        outb %al, %dx
        movb $0x6f, %al
        outb %al, %dx
        movb $0x2c, %al
        outb %al, %dx
        movb $0x79, %al
        outb %al, %dx
        movb $0x65, %al
        outb %al, %dx
        movb $0x7a, %al
        outb %al, %dx
        movb $0x69, %al
        outb %al, %dx
        movb $0x61, %al
        outb %al, %dx
        movb $0x6e, %al
        outb %al, %dx
        movb $0x67, %al
        outb %al, %dx
        hlt
        nop
        nop