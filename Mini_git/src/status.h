//Design Principle
//status should never change state
//It only reads :
//.mygit /HEAD
//.mygit /branches / <branch>
//.mygit /index
#ifndef STATUS_H
#define STATUS_H

    class Status
{
public:
    static void show();
};

#endif
