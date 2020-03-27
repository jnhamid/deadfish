#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#include "history.h"
#include "logger.h"


struct history_array histStruct = {0};

int mymotherfuckingcount = 0;

int commandCount = 1;

void hist_init(unsigned int limit)
{
    histStruct.limit = limit;
    histStruct.head = 0;
    struct command *commands = malloc(limit * sizeof(struct command));

    histStruct.commands = commands; 

}
void hist_destroy(void)
{
    for(int i =0; i < histStruct.tail; i++){
        free(&histStruct.commands[i].realCommand);
    }
}

void hist_add(const char *cmd)
{   


    if(histStruct.tail < histStruct.limit){

        histStruct.commands[mymotherfuckingcount].realCommand = strdup(cmd);

        histStruct.commands[mymotherfuckingcount].cmdID = commandCount;
        histStruct.tail++;
        commandCount++;
        ++mymotherfuckingcount;


    }else{
        int index = histStruct.tail++ % histStruct.limit; 
        histStruct.head++;
        histStruct.head %= histStruct.limit;
        histStruct.commands[index].realCommand = strdup(cmd);
        histStruct.commands[index].cmdID = commandCount;
        mymotherfuckingcount++;
        commandCount++;



        
    }


}

void hist_print(void)
{   
    int i;
    if(histStruct.head < histStruct.tail && histStruct.head == 0){
        for(i = 0; i < histStruct.tail; i++){
            printf("%u %s\n", histStruct.commands[i].cmdID, histStruct.commands[i].realCommand);
        }
    }else{
        LOG("%d\n",histStruct.head);
        LOG("%d\n",histStruct.tail%histStruct.limit);


        int j;
        for(i = histStruct.head; i < histStruct.limit ; i++){
            printf("%u %s\n", histStruct.commands[i].cmdID, histStruct.commands[i].realCommand);
        }
        for (j = 0; j < histStruct.tail%histStruct.limit; j++)
        {   
            printf("%u %s\n", histStruct.commands[j].cmdID, histStruct.commands[j].realCommand);
        }
    }

}

int getCount(void){
    return mymotherfuckingcount; 
}

char *hist_search_prefix(char *prefix)
{   
    LOG("%s\n", prefix);
    LOG("%d\n", histStruct.tail);
    LOG("%d\n", getCount());


    int i;
    if(histStruct.head == 0 && histStruct.tail < histStruct.limit){
        for(i= histStruct.tail -1; i >= 0; i--){
            if(strncmp(prefix, histStruct.commands[i].realCommand, strlen(prefix)) == 0){
                return strdup(histStruct.commands[i].realCommand);
            }
        }

    }else{
        for(i = (histStruct.tail-1) %histStruct.limit; i >= 0; i--){
            if(strncmp(prefix, histStruct.commands[i].realCommand, strlen(prefix)) == 0){
                return strdup(histStruct.commands[i].realCommand);
            }
        }
        for (i = histStruct.limit -1 ; i >= histStruct.head; i--)
        {
            if(strncmp(prefix, histStruct.commands[i].realCommand, strlen(prefix)) == 0){
                return strdup(histStruct.commands[i].realCommand);

            }
        }
    }
    return NULL;
}

char* hist_search_cnum(int command_number)
{
    // TODO: Retrieves a particular command number. Return NULL if no match
    // found.


    int i;
    if(histStruct.head == 0 && histStruct.tail < histStruct.limit){
        for(i= histStruct.tail -1; i >= 0; i--){
            if(histStruct.commands[i].cmdID == command_number + 1){

                return strdup(histStruct.commands[i].realCommand);
            }
        }

    }else{
        for(i = (histStruct.tail-1) %histStruct.limit; i >= 0; i--){
            if(histStruct.commands[i].cmdID == command_number + 1){

                return strdup(histStruct.commands[i].realCommand);
            }
        }
        for (i = histStruct.limit -1 ; i >= histStruct.head; i--)
        {
            if(histStruct.commands[i].cmdID == command_number + 1){

                return strdup(histStruct.commands[i].realCommand);
            }
        }
    }
    return NULL;

}

unsigned int hist_last_cnum(void)
{
    // TODO: Retrieve the most recent command number.
    return histStruct.commands[histStruct.tail%histStruct.limit].cmdID;
}
