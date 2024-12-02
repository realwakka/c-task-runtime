#include <stdarg.h>
#include <stdbool.h>
#include <ucontext.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>

struct task {
  struct ucontext_t context;
  char stack[1024];
  bool is_end;
  void *private;
};

struct executor {
  struct ucontext_t context;
  struct task user_tasks[256];
  size_t user_task_len;
};

void run_executor(struct executor *e) {
  int i=0;
  bool all_end;

  while(1) {
    all_end = true;
    for(i=0; i<e->user_task_len; ++i) {
      if (e->user_tasks[i].is_end) {
	continue;
      }
      swapcontext(&e->context, &e->user_tasks[i].context);
      all_end = false;
    }

    if (all_end)
      break;
  }
}

void create_task(struct executor *e, void (*func) (struct task*), void* private)  {
  struct ucontext_t *context;
  struct task *user_task;

  user_task = &e->user_tasks[e->user_task_len];
  user_task->is_end = false;
  user_task->private = private;

  context = &user_task->context;
  context->uc_stack.ss_sp = e->user_tasks[e->user_task_len].stack;
  context->uc_stack.ss_size = 1024;
  context->uc_link = &e->context;
  makecontext(context, (void(*))func, 1, &e->user_tasks[e->user_task_len]);
  e->user_task_len++;
}

void init_executor(struct executor *e) {
  getcontext(&e->context);
}

void schedule(struct task *current_task) {
  swapcontext(&current_task->context, current_task->context.uc_link);
}
