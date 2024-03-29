//shell.c --- malloc version
#include "../myOS/userInterface.h"

#define NULL (void*)0

int getCmdline(unsigned char *buf, int limit){
    unsigned char *ptr = buf;
    int n = 0;
    while (n < limit) {
        *ptr = uart_get_char();        
        if (*ptr == 0xd) {
            *ptr++ = '\n';
            *ptr = '\0';
            uart_put_char('\r');
            uart_put_char('\n');
            return n+2;
        }
        uart_put_char(*ptr);
        ptr++;
        n++;
    }     
    
    return n;
}

typedef struct cmd {//all dynamic
    unsigned char* cmd;
    int (*func)(int argc, unsigned char **argv);
    void (*help_func)(void);
    unsigned char* description;
    struct cmd * nextCmd;
} CMD;

#define cmd_size sizeof(struct cmd)

struct cmd *ourCmds = NULL;

int listCmds(int argc, unsigned char **argv){
    struct cmd *tmpCmd = ourCmds;
    myPrintf(0x7, "list all registered commands:\n");
    myPrintf(0x7, "command name: description\n");

    while (tmpCmd != NULL) {
        myPrintf(0x7,"% 12s: %s\n",tmpCmd->cmd, tmpCmd->description);
        tmpCmd = tmpCmd->nextCmd;
    }
    return 0;
}

void addNewCmd(	unsigned char *cmd, 
	int (*func)(int argc, unsigned char **argv), 
	void (*help_func)(void), 
	unsigned char* description){
	//本函数需要实现！！！
    /*功能：增加命令
    1.使用malloc创建一个cm的结构体，新增命令。
    2.同时还需要维护一个表头为ourCmds的链表。
    */
    CMD* new_cmd = (CMD*)malloc(sizeof(CMD));//malloc space for cmd data
    unsigned char* namecmd = (unsigned char*)malloc(strLength(cmd));
    unsigned char* descripcmd = (unsigned char*)malloc(strLength(description));
    strncpy(cmd,namecmd,strLength(cmd));
    strncpy(description,descripcmd,strLength(description));
    new_cmd->cmd = namecmd;
    new_cmd->func = func;
    new_cmd->help_func = help_func;
    new_cmd->description = descripcmd;
    new_cmd->nextCmd = NULL;
    CMD* temp = ourCmds;
    if(temp==NULL) ourCmds = new_cmd;//尾插法
    else{
        while(temp->nextCmd!=NULL) 
            temp = temp->nextCmd;
        temp->nextCmd = new_cmd;
    }
}

void help_help(void){
    myPrintf(0x7,"USAGE: help [cmd]\n\n");
}

int help(int argc, unsigned char **argv){
    int i;
    struct cmd *tmpCmd = ourCmds;
    help_help();

    if (argc==1) return listCmds(argc,argv);
    if (argc>2) return 1;
    
    while (tmpCmd != NULL) {            
        if (strcmp(argv[1],tmpCmd->cmd)==0) {
            if (tmpCmd->help_func!=NULL)
                tmpCmd->help_func();
            else myPrintf(0x7,"%s\n",tmpCmd->description);
            break;
        }
        tmpCmd = tmpCmd->nextCmd;
    }
    return 0;
}

int func_time(int argc, char **argv){
    if(argc != 1) myPrintf(14,"notices: time needs no args\n");
    int h,m,s;
    h=0;m=0;s=0;
    getWallClock(&h,&m,&s);
    myPrintf(0x07,"%02d:%02d:%02d\n",h,m,s);
}

struct cmd *findCmd(unsigned char *cmd){
        struct cmd * tmpCmd = ourCmds;
	int found = 0;
        while (tmpCmd != NULL) {  //at lease 2 cmds            
            if (strcmp(cmd,tmpCmd->cmd)==0){
            myPrintf(0x7,"%s %s\n",cmd,tmpCmd->cmd);
		    found=1;
		    break;
	    }
            tmpCmd = tmpCmd->nextCmd;
        }
	return found?tmpCmd:NULL;
}

int split2Words(unsigned char *cmdline, unsigned char **argv, int limit){
    unsigned char c, *ptr = cmdline;
    int argc=0;    
    int inAWord=0;

    while ( c = *ptr ) {  // not '\0'
        if (argc >= limit) {
            myPrintf(0x7,"cmdline is tooooo long\n");
            break;
        }
        switch (c) {
            case ' ':  *ptr = '\0'; inAWord = 0; break; //skip white space
            case '\n': *ptr = '\0'; inAWord = 0; break; //end of cmdline?
            default:  //a word
             if (!inAWord) *(argv + argc++) = ptr;
             inAWord = 1;             
             break;
        }   
        ptr++;     
    }
    return argc;
}

void initShell(void){
    addNewCmd("cmd\0",listCmds,NULL,"list all registered commands\0");
    addNewCmd("help\0",help,help_help,"help [cmd]\0");
    addNewCmd("time\0",func_time,NULL,"show current time\0");
    //TODO: may be we can add a new command exit or quit
}

unsigned char cmdline[100];
void startShell(void){    
    unsigned char *argv[10];  //max 10
    int argc;    
    struct cmd *tmpCmd;
    //myPrintf(0x7,"StartShell:\n");     
    
    while(1) {
        myPrintf(0x3,"Ang_yz >>:");
        getCmdline(&cmdline[0],100);
        myPrintf(0x7,cmdline);

        argc = split2Words(cmdline,&argv[0],10); 
        if (argc == 0) continue;

	    tmpCmd = findCmd(argv[0]);
        if (tmpCmd)   
	        tmpCmd->func(argc, argv);
	    else
            myPrintf(0x7,"UNKOWN command: %s\n",argv[0]);
    }
}
