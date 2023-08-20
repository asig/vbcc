#include <exec/types.h>
#include <exec/tasks.h>

extern	int trapintercept;

#define NUM_OF_TRAPS 64

int *localf[NUM_OF_TRAPS]() = 0;

struct Task *FindTask();

char intercepting=0;
APTR oldtraphandler = 0;
APTR oldtrapdata = 0;

InterceptTrap(n,f)
int n;
int *f();
{
	if (intercepting == 0)
	{
		struct Task *t;
		t = FindTask(0);
		oldtraphandler = t->tc_TrapCode;
		oldtrapdata = t->tc_TrapData;
		t->tc_TrapCode = &trapintercept;
		/* we don't care about trap data */
	}
	localf[n] = f;
	intercepting++;
}

UnwindTrap(n)
int n;
{
	if (intercepting)
	{
		intercepting--;
		if (intercepting == 0)
		{	/* now remove interceptor */
			struct Task *t = FindTask(0);
			/* restore original trap handler */
			t->tc_TrapData = oldtrapdata;
			t->tc_TrapCode = oldtraphandler;
		}
		localf[n] = 0;
	}
	else
	{
		printf("bad Unwind call\n");
		exit(-1);
	}
}

/* supervisor code */
localtrapintercept()
{
}

