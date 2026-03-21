#include <stdio.h>

void loop_start()
{
    printf("loop_start_call\n");
}

void loop_end()
{
    printf("loop_end_call\n");
}

void simple_loop(int n) {
    printf("TEST: simple_loop\n");
    for (int i = 0; i < n; i++) {
        printf("loop_body\n");
    }
}

void nested_loops(int n, int m) {
    printf("TEST: nested_loops\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("loop_body\n");
        }
    }
}


void multi_exit(int n, int cond1) {
    printf("TEST: multi_exit\n");
    for (int i = 0; i < n; i++) {
        if (cond1) {
            printf("loop_body\n");
        }
    }
}

int main()
{

    simple_loop(2);
    nested_loops(2, 1);
    multi_exit(1, 1);

    return 0;
}
