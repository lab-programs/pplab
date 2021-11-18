#ifndef PPLAB_PROC_COM_H
#define PPLAB_PROC_COM_H

#include "stdio.h"
#include "stdlib.h"
#include "signal.h"
#include "sys/types.h"
#include "unistd.h"

static inline
void sighup( int );
static inline
void sigint( int );
static inline
void sigquit( int );

static inline
void proc_com()
{
    int pid = fork();
    if( pid < 0 )
    {
        perror("process not found");
        exit(1);
    }

    if( pid == 0 )
    {
        /* child process */
        signal( SIGHUP, sighup );
        signal( SIGINT, sigint );
        signal( SIGQUIT, sigquit );
        for(;;);
    }
    else
    {
        /* parent process */
        printf("parent process sent sighup signal\n");
        kill( pid, SIGHUP );
        sleep(3);

        printf("parent process sent sigint signal\n");
        kill( pid, SIGINT );
        sleep(3);
        
        printf("parent process sent sigquit signal\n");
        kill( pid, SIGQUIT );
        sleep(3);
    }
}

static inline
void sighup( int pid )
{
    signal( SIGHUP, sighup );
    printf("child process received sighup signal\n");
}

static inline
void sigint( int pid )
{
    signal( SIGINT, sigint );
    printf("child process received sigint signal\n");
}

static inline
void sigquit( int pid )
{
    signal( SIGQUIT, sigquit );
    printf("child process got destroyed\n");
}

#endif