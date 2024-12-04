# c-task-runtime
Simple runtime task implementation with C

## Example

code
```c
#include "executor.h"

void func(struct task *task) {
  uint64_t num;

  num = (uint64_t)task->private;
  printf("func%ld-1!\n", num);
  schedule(task);
  printf("func%ld-2!\n", num);
  schedule(task);
  printf("func%ld-3!\n", num);
  schedule(task);
  task->is_end = true;
}

int main() {
  struct executor e;
  uint64_t value;
  init_executor(&e);

  value = 1;
  create_task(&e, func, (void *)value);

  value = 2;
  create_task(&e, func, (void *)value);

  run_executor(&e);
  return 0;
}
```

output
```
func1-1!
func2-1!
func1-2!
func2-2!
func1-3!
func2-3!
```
