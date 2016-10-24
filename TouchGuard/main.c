//
//  main.c
//  TouchGuard
//
//  Created by SyntaxSoft 2016.
//

#include <stdio.h>
#include <sys/time.h>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <dispatch/dispatch.h>

#define MAJOR_VERSION  1
#define MINOR_VERSION  4


#define DISABLE_TAP_PB               1
#define DISABLE_DEBUG_MESSAGES       2
#define ENABLE_TAPENABLE_MESSSAGE    4
#define ENABLE_TAPDISABLE_MESSAGE    8
// 0x10 = 10 in base-16 (16 in base-10)
#define ENABLE_TAPIGNORE_MESSAGE   0x10


static int mgi_flag = 0;
static long timerInterval = 1;
static int consecutiveDisableCount = 0;
static int consecutiveIgnoreCount = 0;

void PrintCurrentTime()
{
    struct timeval  tv;
    struct tm* tm_info;
    char ac_buffer[256];
    
    gettimeofday(&tv, NULL);
    
    tm_info = localtime(&tv.tv_sec);
    
    strftime(ac_buffer, sizeof(ac_buffer), "%b %d %Y %H:%M:%S", tm_info);
    
    printf("%s %dmsec",ac_buffer, tv.tv_usec/1000);
    
}

static int64_t  dispatchCount = 0;

void dispatchCallBack(void * pv_context)
{
    int64_t count = (int64_t) pv_context;
    
    if(count == dispatchCount)
    {
        mgi_flag &= (~(DISABLE_TAP_PB));
        int countPrinted = 0;
        if((mgi_flag&DISABLE_DEBUG_MESSAGES) == 0)
        {
            if(consecutiveDisableCount && (mgi_flag&ENABLE_TAPDISABLE_MESSAGE))
            {
                printf("DisableCount %d ", consecutiveDisableCount);
                countPrinted = 1;
            }
            
            if(consecutiveIgnoreCount && (mgi_flag&ENABLE_TAPIGNORE_MESSAGE))
            {
                int tapCount = (consecutiveIgnoreCount/2)-1;
                if(tapCount)
                {
                    printf("IgnoreCount %d", tapCount);
                    countPrinted = 1;
                }
            }
            
            if(countPrinted)
            {
                printf("\n");
            }
            
            consecutiveDisableCount = 0;
            consecutiveIgnoreCount = 0;
            if(mgi_flag&ENABLE_TAPENABLE_MESSSAGE)
            {
                printf("Enabled Tap \n");
            }
        }
    }
}


void dispatchDisableTap()
{
    if((mgi_flag&(DISABLE_DEBUG_MESSAGES)) == 0)
    {
        if(mgi_flag&DISABLE_TAP_PB)
        {
            ++consecutiveDisableCount;
        }
        else
        {
            if(mgi_flag&ENABLE_TAPDISABLE_MESSAGE)
            {
                printf("Disabled tap\n");
            }
        }
    }
    mgi_flag |= (DISABLE_TAP_PB);
    ++dispatchCount;
    dispatch_time_t time = dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_MSEC*timerInterval);
    dispatch_after_f(time, dispatch_get_main_queue(), (void *)dispatchCount, dispatchCallBack);
}



CGEventRef eventCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
    //if (type != kCGEventMouseMoved)
    if(type == kCGEventKeyUp)
    {
        dispatchDisableTap();
    }
    else if( (type == kCGEventLeftMouseDown)||(type == kCGEventLeftMouseUp)||(type == kCGEventRightMouseDown)||(type == kCGEventRightMouseUp) )
    {
        // if tap is disabled return NULL
        if(mgi_flag&DISABLE_TAP_PB)
        {
            if((mgi_flag&DISABLE_DEBUG_MESSAGES) == 0)
            {
                if(consecutiveIgnoreCount == 0)
                {
                    if(mgi_flag&ENABLE_TAPIGNORE_MESSAGE)
                    {
                        PrintCurrentTime();
                        printf(": Ignoring tap\n");
                    }
                }
                ++consecutiveIgnoreCount;
            }
            
            return NULL;
        }
    }
    
    
    return event;
}

int main(int argc, const char * argv[])
{
    CFMachPortRef eventTap;
    CFRunLoopSourceRef eventRunLoop;
    int count = 1;
    
    mgi_flag |= ENABLE_TAPIGNORE_MESSAGE;
    
    while(count < argc)
    {
        if(strcasecmp("-nodebug", argv[count]) == 0)
        {
            mgi_flag |= DISABLE_DEBUG_MESSAGES; //bitwise or
        }
        else if(strcasecmp("-time", argv[count]) == 0)
        {
            float value = 0;
            ++count;
            if(count >= argc)
            {
                exit(1);
            }
            value = strtof(argv[count], NULL);
            if(value > 0)
            {
                // converting seconds to milliseconds
                timerInterval = value*1000;
            }
        }
        else if(strcasecmp("-version", argv[count]) == 0)
        {
            printf("Version %d.%d\n",MAJOR_VERSION,MINOR_VERSION);
        }
        else if(strcasecmp("-TapEnableMsg", argv[count]) == 0)
        {
            mgi_flag |= ENABLE_TAPENABLE_MESSSAGE;
        }
        else if(strcasecmp("-TapDisableMsg", argv[count]) == 0)
        {
            mgi_flag |= ENABLE_TAPDISABLE_MESSAGE;
        }
        count++;
    }
    
    if((mgi_flag&DISABLE_DEBUG_MESSAGES) == 0)
    {
        printf("Disable interval %ld milliSeconds\n",timerInterval);
    }
    
    
    eventTap = CGEventTapCreate(kCGHIDEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, kCGEventMaskForAllEvents, eventCallBack, NULL);
    
    if(!eventTap)
        exit(1);
    
    eventRunLoop = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), eventRunLoop, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    
    
    CFRunLoopRun();
    
    exit(0);
}

