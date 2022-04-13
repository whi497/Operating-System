#include "io.h"
#include "myPrintk.h"
#include "uart.h"
#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
#include "wallClock.h"
//todo//done
typedef struct myCommand {
    char name[80];
    char help_content[200];
    int (*func)(int argc, char (*argv)[8]);
}myCommand; 

extern myCommand cmd;//ahead declration
extern myCommand time;
extern myCommand help;

int func_cmd(int argc, char (*argv)[8]){
	if(argc != 1) myPrintf(14,"notice: cmd needs no args\n");
    myPrintf(0x07,cmd.name);myPrintf(0x07," ");
    myPrintf(0x07,time.name);myPrintf(0x07," ");
    myPrintf(0x07,help.name);
    myPrintf(0x07,"\n");
} 
myCommand cmd={"cmd\0","List all command\n\0",func_cmd};

int func_time(int argc, char (*argv)[8]){
    if(argc != 1) myPrintf(14,"notices: time needs no args\n");
    int h,m,s;
    h=0;m=0;s=0;
    getWallClock(&h,&m,&s);
    myPrintf(0x07,"%02d:%02d:%02d\n",h,m,s);
}
myCommand time={"time\0","show the current time\n",func_time};

int func_help(int argc, char (*argv)[8]){
    if(argc == 1) {
        myPrintf(0x02,help.help_content);
<<<<<<< HEAD
        // myPrintf(0x02,cmd.help_content);
=======
>>>>>>> e0891c2f15b59358978bf866dd91f33be68b871b
    }
    else{
        for (int i = 1; i < argc; i++)//help all command after the help
        {
            if(strcmp(argv[i],"help")) myPrintf(0x02,help.help_content);
            else if(strcmp(argv[i],"cmd")) myPrintf(0x02,cmd.help_content);
            else if(strcmp(argv[i],"time")) myPrintf(0x02,time.help_content);
        }
        
    }
}
myCommand help={"help\0","Usage: help [command]\nDisplay info about [command]\n\0",func_help};


/***********************************************************************************/

void cmd_get(const char* cmd_string, int cmd_len, int* argc, char (*argv) [8]);//get command parameters from command string
int strcmp(char* str1,char* str2);

void startShell(void){
//我们通过串口来实现数据的输入
char BUF[256]; //输入缓存区
int BUF_len=0;	//输入缓存区的长度
    
	int argc;
    char argv[8][8];
    int h,m,s = 0;
    getWallClock(&h,&m,&s);
    myPrintf(0x07,"Welcome to myOS! time now is %02d:%02d:%02d\n",h,m,s);
    do{
        BUF_len=0; 
<<<<<<< HEAD
        myPrintf(0x07,"PB20020586>>\0");
=======
        myPrintf(0x12,"PB20020586");
        myPrintf(0x07,">>");
>>>>>>> e0891c2f15b59358978bf866dd91f33be68b871b
        while((BUF[BUF_len]=uart_get_char())!='\r'){
            uart_put_char(BUF[BUF_len]);//将串口输入的数存入BUF数组中
            BUF_len++;  //BUF数组的长度加
        }
        BUF[BUF_len]='\0';
        uart_put_chars(" -pseudo_terminal\0");
        uart_put_char('\n');
        append2screen(BUF,0x07);
        append2screen("\n",0x07);
        // put_char2pos('\n',0x07,get_cursor_pos());
        //OK,助教已经帮助你们实现了“从串口中读取数据存储到BUF数组中”的任务，接下来你们要做
        //的就是对BUF数组中存储的数据进行处理(也即，从BUF数组中提取相应的argc和argv参
        //数)，再根据argc和argv，寻找相应的myCommand ***实例，进行***.func(argc,argv)函数
        //调用。

        //比如BUF中的内容为 “help cmd”
        //那么此时的argc为2 argv[0]为help argv[1]为cmd
        //接下来就是 help.func(argc, argv)进行函数调用即可
        cmd_get(BUF,BUF_len,&argc,argv);
        if(strcmp(argv[0],"help")) {
            help.func(argc,argv);
        }
        else if(strcmp(argv[0],"cmd")) {
            cmd.func(argc,argv);
        }
        else if(strcmp(argv[0],"time")) {
            time.func(argc,argv);
        }
        else myPrintf(12,"undeclared command\n");

    }while(1);

}

void cmd_get(const char* cmd_string, int cmd_len, int* argc, char (*argv)[8]){
    int index = 0;
    *argc = 0;
    for(int i = 0; i<cmd_len; i++) {
        if(cmd_string[i]>=33 && cmd_string[i]<=126) {//非控制字符
            (*argc)++;
            int j;
            for(j = i;j<cmd_len && cmd_string[j]>=33 && cmd_string[j]<=126 ; j++) {//get a string
                argv[*argc-1][index++] = cmd_string[j];
            }
            i = j;
            argv[*argc-1][index] = '\0';
            index = 0;
        }
    }
}


int strcmp(char* str1,char* str2){
    int i;
    for(i=0;str1[i]!='\0' && str2[i] !='\0';i++){
        if(str1[i]!=str2[i])return 0;
    }
    if(str1[i]!='\0' | str2[i]!='\0')return 0;
    else return 1;
}