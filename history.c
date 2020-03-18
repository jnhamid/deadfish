#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#include "history.h"
#include "logger.h"


struct history_array histStruct = {0};

int mymotherfuckingcount = 0;

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

        histStruct.commands[mymotherfuckingcount].cmdID = mymotherfuckingcount;
        histStruct.tail++;
        ++mymotherfuckingcount;


    }else{
        int index = histStruct.tail++ % histStruct.limit; 
        histStruct.head++;
        histStruct.commands[index].realCommand = strdup(cmd);
        histStruct.commands[index].cmdID = mymotherfuckingcount;
        mymotherfuckingcount++;


        
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


    // for (int i =  histStruct.tail%limit-1; i > 0; i--)
    // {   
    //     LOG("%d\n", i);

    //     LOG("%s\n", histStruct.commands[i].realCommand);
    //     if (strncmp(prefix, histStruct.commands[i].realCommand, strlen(prefix)) == 0)
    //     {   

    //         LOG("%s\n", histStruct.commands[i].realCommand);
    //         return strdup(histStruct.commands[i].realCommand);
    //     }
    // }
    // TODO: Retrieves the most recent command starting with 'prefix', or NULL
    // if no match found.
    int i;
    if(histStruct.head == 0 && histStruct.tail < histStruct.limit){
        for(i= histStruct.tail -1; i >= 0; i--){
            if(strncmp(prefix, histStruct.commands[i].realCommand, strlen(prefix)) == 0){
                return strdup(histStruct.commands[i].realCommand);
            }
        }
    }else if(histStruct.tail%histStruct.limit == 0){
        if(strncmp(prefix, histStruct.commands[0].realCommand, strlen(prefix)) == 0){
            return strdup(histStruct.commands[0].realCommand);

        }else{
            for(i = histStruct.limit; i >= 0; i--){
                if(strncmp(prefix, histStruct.commands[i].realCommand, strlen(prefix)) == 0){
                    return strdup(histStruct.commands[i].realCommand);

                }
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
        for(int i = 0; i< histStruct.limit; i++){
            if(histStruct.commands[i].cmdID == command_number){
                LOG("%s\n", histStruct.commands[i].realCommand);


                return strdup(histStruct.commands[i].realCommand);
            }
        }
        return NULL;
}

unsigned int hist_last_cnum(void)
{
    // TODO: Retrieve the most recent command number.
    return histStruct.commands[histStruct.tail%histStruct.limit].cmdID;
}
