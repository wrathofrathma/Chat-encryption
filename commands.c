int Command(char *cmdStr, struct userInfo *use_info)
{
    char cmd[20];
    char param[20];
    int reval;
    int cmdres;

    cmdres = commandParser (cmdStr, cmd, param);
    if(strcmp(cmd,"user")==0)
    {
        reval=changeUser(param, use_info);
    }
    else if(strcmp(cmd,"quit")==0)
    {
        reval=uQuit(use_info);
    }
    return reval;
}
int uQuit(struct userInfo *use_info)
{
    int use=findItem(begin,use_info->username);
    if(use>0)
    {
        Send(use,"Shutting down your connection, press enter to quit \n",use_info);
        shutdown(use,2);
        printf("Terminated user socket %s\n", use_info->username);
        return -3;

    }
}

int changeUser(char *user, struct userInfo *use_info)
{
    int buf=findItem(begin,user);
    if(buf>0)
    {
        use_info->fat_person=buf;
        return 0;
    }
    else if(buf==-1)
    {
        int use=findItem(begin,use_info->username);
        Send(use,"Username not found \n", use_info);
        return 1;
    }
    else
    {
        printf("Um something went wrong in changeUser\n");
        return 2;

    }
}

