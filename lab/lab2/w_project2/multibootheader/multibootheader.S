.globl start
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
        call _start
        hlt