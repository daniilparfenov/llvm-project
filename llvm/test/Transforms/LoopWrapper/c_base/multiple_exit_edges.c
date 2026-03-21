
// Function to avoid loop removing
extern void side_effect();

void multi_exit(int n, int cond1) {
    for (int i = 0; i < n; i++) {
        if (cond1) {
            return;
        }
        side_effect();
    }
}
