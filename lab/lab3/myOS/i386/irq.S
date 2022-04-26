.text
.code32
_start:
	.globl enable_interrupt
enable_interrupt:
# todo//done
	sti
	ret
	
	.globl disable_interrupt
disable_interrupt:
# todo//done
	cli
	ret
